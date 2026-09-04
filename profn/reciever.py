from flask import Flask, request, jsonify, send_file
from flask_cors import CORS

import requests
from datetime import datetime
import os


# =====================================
# FLASK APP
# =====================================

app = Flask(__name__)

CORS(app)


# =====================================
# ESP32 SETTINGS
# =====================================

ESP32_IP = "192.168.137.38"

FOUND_URL = f"http://{ESP32_IP}/found"

NOTFOUND_URL = f"http://{ESP32_IP}/notfound"

RESET_URL = f"http://{ESP32_IP}/reset"

REQUEST_TIMEOUT = 5


# =====================================
# DASHBOARD DATA
# =====================================

events = []

latest_state = "WAITING"


# =====================================
# DASHBOARD PAGE
# =====================================

@app.route("/")
def dashboard():

    file_path = os.path.join(
        app.root_path,
        "index.html"
    )

    return send_file(file_path)


# =====================================
# RECEIVE STATE
# =====================================

@app.route("/state", methods=["POST"])
def receive_state():

    global latest_state


    # Get JSON data

    data = request.get_json(
        silent=True
    )


    # Validate data

    if not data or "state" not in data:

        return jsonify({

            "success": False,

            "error": "No state received"

        }), 400


    # Convert state

    state = str(

        data["state"]

    ).upper().strip()


    # Validate state

    if state not in [

        "FOUND",

        "NOTFOUND"

    ]:

        return jsonify({

            "success": False,

            "error": "Invalid state"

        }), 400


    print()

    print(

        "=============================="

    )

    print(

        "NEW STATE RECEIVED:",

        state

    )

    print(

        "=============================="

    )


    # Update state

    latest_state = state


    # =================================
    # SAVE EVENT
    # =================================

    event = {

        "state": state,

        "time": datetime.now().isoformat()

    }


    events.append(event)


    # Keep latest 100 events

    if len(events) > 100:

        events.pop(0)


    # =================================
    # SEND TO ESP32
    # =================================

    try:


        if state == "FOUND":

            print(

                "Sending FOUND to ESP32..."

            )


            response = requests.get(

                FOUND_URL,

                timeout=REQUEST_TIMEOUT

            )


        else:

            print(

                "Sending NOTFOUND to ESP32..."

            )


            response = requests.get(

                NOTFOUND_URL,

                timeout=REQUEST_TIMEOUT

            )


        response.raise_for_status()


        print(

            "ESP32 RESPONSE:",

            response.text

        )


        return jsonify({

            "success": True,

            "state": state,

            "esp32_success": True,

            "esp32_response": response.text

        })


    except requests.exceptions.RequestException as error:


        print()

        print(

            "ESP32 ERROR:"

        )

        print(error)


        return jsonify({

            "success": True,

            "state": state,

            "esp32_success": False,

            "esp32_error": str(error)

        })


# =====================================
# DASHBOARD STATUS
# =====================================

@app.route("/api/status")
def api_status():


    # Count FOUND events

    found_count = sum(

        1

        for event in events

        if event["state"] == "FOUND"

    )


    # Count NOTFOUND events

    notfound_count = sum(

        1

        for event in events

        if event["state"] == "NOTFOUND"

    )


    return jsonify({


        "latest_state":

            latest_state,


        "total":

            len(events),


        "found":

            found_count,


        "notfound":

            notfound_count,


        "last_event":

            events[-1]

            if events

            else None,


        "events":

            events[-50:]

    })


# =====================================
# RESET SESSION
# =====================================

@app.route(

    "/api/reset",

    methods=["POST"]

)
def reset():

    global events
    global latest_state


    print()

    print(

        "RESET REQUEST RECEIVED"

    )


    # =================================
    # RESET FLASK DATA
    # =================================

    events = []

    latest_state = "WAITING"


    # =================================
    # RESET ESP32
    # =================================

    try:


        print(

            "Sending RESET to ESP32..."

        )


        response = requests.get(

            RESET_URL,

            timeout=REQUEST_TIMEOUT

        )


        response.raise_for_status()


        print(

            "ESP32 RESET RESPONSE:",

            response.text

        )


        return jsonify({

            "success": True,

            "esp32_reset": True,

            "message":

                "Dashboard and ESP32 reset"

        })


    except requests.exceptions.RequestException as error:


        print()

        print(

            "ESP32 RESET ERROR:"

        )

        print(error)


        return jsonify({

            "success": True,

            "esp32_reset": False,

            "message":

                "Dashboard reset but ESP32 failed",

            "esp32_error":

                str(error)

        })


# =====================================
# START SERVER
# =====================================

if __name__ == "__main__":


    print()

    print(

        "================================"

    )

    print(

        "AI PROFANITY DETECTOR RECEIVER"

    )

    print(

        "================================"

    )

    print()

    print(

        "Dashboard:"

    )

    print(

        "http://127.0.0.1:5000"

    )

    print()

    print(

        "Waiting for ChatGPT messages..."

    )

    print()


    app.run(

        host="0.0.0.0",

        port=5000,

        debug=False

    )