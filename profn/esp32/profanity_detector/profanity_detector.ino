#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>


// =====================================
// WIFI SETTINGS
// =====================================

const char* ssid = "😎";
const char* password = "12345678";


// =====================================
// PINS
// =====================================

// RGB LED
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

// Servo
#define SERVO_PIN 18

// Buzzer
#define BUZZER_PIN 19


// =====================================
// LCD
// =====================================

LiquidCrystal_I2C lcd(0x27, 16, 2);


// =====================================
// WEB SERVER
// =====================================

WebServer server(80);


// =====================================
// SERVO
// =====================================

Servo myServo;


// =====================================
// PROFANITY COUNT
// =====================================

int profanityCount = 0;


// =====================================
// RGB FUNCTIONS
// COMMON CATHODE RGB LED
// =====================================

void setRed() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

}


void setGreen() {

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);

}


void setViolet() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);

}


// =====================================
// BUZZER FUNCTIONS
// ACTIVE LOW
// LOW = ON
// HIGH = OFF
// =====================================

void buzzerOn() {

  digitalWrite(BUZZER_PIN, LOW);

}


void buzzerOff() {

  digitalWrite(BUZZER_PIN, HIGH);

}


// =====================================
// UPDATE LCD WITH IDLE SCREEN
// =====================================

void showIdleLCD() {

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("PROFANITY COUNT");

  lcd.setCursor(0, 1);

  lcd.print("COUNT: ");

  lcd.print(profanityCount);

}


// =====================================
// ALERT PATTERN - SHORT "NOTIFICATION" CHIME (~0.9s)
// =====================================

void playAlert() {

  // Two quick light taps
  buzzerOn();  delay(70);  buzzerOff(); delay(60);
  buzzerOn();  delay(70);  buzzerOff(); delay(100);

  // One soft closing beep
  buzzerOn();  delay(250); buzzerOff();

}


// =====================================
// FOUND
// =====================================

void handleFound() {

  Serial.println("FOUND RECEIVED");


  // Increase counter

  profanityCount++;


  // Red alert LED

  setRed();


  // LCD

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("FOUND!");


  lcd.setCursor(0, 1);

  lcd.print("COUNT: ");

  lcd.print(profanityCount);


  // Send response immediately

  server.send(

    200,

    "text/plain",

    "FOUND"

  );


  // Move servo to alert position

  myServo.write(90);


  // Play buzzer pattern

  playAlert();


  // Ensure buzzer OFF

  buzzerOff();


  // Return servo to normal position

  myServo.write(0);


  // Return LED to idle

  setViolet();


  Serial.println("FOUND ALERT COMPLETE");

}


// =====================================
// NOTFOUND
// =====================================

void handleNotFound() {

  Serial.println("NOTFOUND RECEIVED");


  // Ensure buzzer is OFF

  buzzerOff();


  // Ensure normal servo position

  myServo.write(0);


  // Green LED

  setGreen();


  // LCD

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("NOT FOUND");


  lcd.setCursor(0, 1);

  lcd.print("COUNT: ");

  lcd.print(profanityCount);


  // Send response

  server.send(

    200,

    "text/plain",

    "NOTFOUND"

  );


  // Show result briefly

  delay(1000);


  // Return to idle

  buzzerOff();

  myServo.write(0);

  setViolet();


  Serial.println("NOTFOUND COMPLETE");

}


// =====================================
// RESET
// =====================================

void handleReset() {

  Serial.println("RESET RECEIVED");


  // Reset profanity counter

  profanityCount = 0;


  // Turn buzzer OFF

  buzzerOff();


  // Return servo to normal position

  myServo.write(0);


  // Return RGB LED to idle colour

  setViolet();


  // Update LCD

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("SESSION RESET");


  lcd.setCursor(0, 1);

  lcd.print("COUNT: 0");


  Serial.println("COUNTER RESET TO ZERO");


  // Send response

  server.send(

    200,

    "text/plain",

    "COUNTER RESET TO 0"

  );


  // Briefly show reset message

  delay(1000);


  // Return to normal idle display

  showIdleLCD();

}


// =====================================
// ROOT
// =====================================

void handleRoot() {

  String message = "ESP32 PROFANITY DETECTOR\n";

  message += "Count: ";

  message += String(profanityCount);


  server.send(

    200,

    "text/plain",

    message

  );

}


// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);


  // RGB pins

  pinMode(RED_PIN, OUTPUT);

  pinMode(GREEN_PIN, OUTPUT);

  pinMode(BLUE_PIN, OUTPUT);


  // Buzzer

  pinMode(BUZZER_PIN, OUTPUT);

  buzzerOff();


  // Start violet

  setViolet();


  // Servo

  myServo.attach(SERVO_PIN);

  myServo.write(0);


  // LCD

  Wire.begin(21, 22);

  lcd.init();

  lcd.backlight();


  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Profanity");


  lcd.setCursor(0, 1);

  lcd.print("Detector");


  delay(1500);


  // Show initial counter

  showIdleLCD();


  // =====================================
  // WIFI
  // =====================================

  WiFi.mode(WIFI_STA);

  WiFi.begin(

    ssid,

    password

  );


  Serial.print("Connecting to WiFi");


  int attempts = 0;


  while (

    WiFi.status() != WL_CONNECTED

    &&

    attempts < 30

  ) {

    delay(500);

    Serial.print(".");

    attempts++;

  }


  Serial.println();


  if (

    WiFi.status() == WL_CONNECTED

  ) {

    Serial.println("WIFI CONNECTED");


    Serial.print("ESP32 IP: ");

    Serial.println(

      WiFi.localIP()

    );


  }

  else {

    Serial.println("WIFI FAILED");


    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("WiFi Failed");

  }


  // =====================================
  // WEB ROUTES
  // =====================================

  server.on(

    "/",

    handleRoot

  );


  server.on(

    "/found",

    handleFound

  );


  server.on(

    "/notfound",

    handleNotFound

  );


  // NEW RESET ROUTE

  server.on(

    "/reset",

    handleReset

  );


  // Start server

  server.begin();


  Serial.println("SERVER STARTED");

}


// =====================================
// LOOP
// =====================================

void loop() {

  server.handleClient();

}