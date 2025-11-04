#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h"
#include "Wire.h"

// Smart Servo Variables
#define rxPin 8
#define txPin 9
SoftwareSerial mySerial(rxPin, txPin);
#define LSS_ID 254                          // ID 254 to broadcast to every motor on bus
#define LEFT_MOTOR   0
#define RIGHT_MOTOR  1

int currentSpeed = 1;

SCMD md1;
SCMD md2;
SCMD md3;

// Button
#define PIN1 7
#define PIN2 4
#define PIN3 2


#define SCMD_ADDR_1 0x5D
#define SCMD_ADDR_2 0x61
#define SCMD_ADDR_3 0x60

#include "SerialChatGPT.h"

// Variables
bool ledState = false;
float motorPosition = 0.0;
int motorSpeed = 0;
int imuValue = 5;
unsigned long previousMillisShake = 0; // This is used to keep track of notify frequencies
String storedString = "a robot may not injure a human being or, through inaction, allow one to come to harm";


void initSCMD(SCMD &drv, uint8_t address) {
  drv.settings.commInterface = I2C_MODE;
  drv.settings.I2CAddress = address;

  while (drv.begin() != 0xA9) {
    Serial.print("ID mismatch at 0x");
    Serial.println(address, HEX);
    delay(500);
  }
  while (!drv.ready());
  while (drv.busy());
  drv.enable();
}

void setup()
{
  Serial.begin(115200); // don't change the baud rate!
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT); // used to indicate TTS state

  pinMode(PIN1, INPUT_PULLUP); 
  pinMode(PIN2, INPUT_PULLUP);  
  
  Wire.begin();

  mySerial.print("#0D1500\r");

  initSCMD(md1, SCMD_ADDR_1);
  initSCMD(md2, SCMD_ADDR_2);
  initSCMD(md3, SCMD_ADDR_3);

  mySerial.begin(115200);
  mySerial.print("#0D1500\r");    
  
  mySerial.print(String("#") + LSS_ID + String("WR") + currentSpeed  + "\r");

}

void loop() {
  unsigned long currentMillis = millis(); // we will use this to keep track of notify frequency
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }

  if (analogRead(A0) >= 1000 && currentMillis - previousMillisShake >= 2000)
  {
    notify("press", true);
    previousMillisShake = currentMillis;
  }
}

// -- Rotation Speed --
// Accepts number between 5 and 20
// Define Speed
void change_rotation_speed(int targetSpeed) {
  int stepDelay = 10;
  int stepSize = 1;

  if (targetSpeed > currentSpeed) {
    for (int s = currentSpeed; s <= targetSpeed; s += stepSize) {
      currentSpeed = s;
      mySerial.print(String("#") + LSS_ID + String("WR") + currentSpeed  + "\r");
      delay(stepDelay);
    }
  } else if (targetSpeed < currentSpeed) {
    // decrease speed gradually
    for (int s = currentSpeed; s >= targetSpeed; s -= stepSize) {
      currentSpeed = s;
      mySerial.print(String("#") + LSS_ID + String("WR") + currentSpeed  + "\r");
      delay(stepDelay);
    }
  }
}

// -- Spin Motor 1 --
// Accepts number between 30 and 200
// Defined Speed
void start_spin_1(int speed) {
  if (speed >= 30 && speed <= 200) {
    md1.setDrive(LEFT_MOTOR, 0, speed);
    delay(1000);
    md1.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
    md1.setDrive(LEFT_MOTOR, 1, speed);
    delay(1000);
    md1.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
  }
}

// -- Spin Motor 2 --
// Accepts number between 30 and 200
// Defined Speed
void start_spin_2(int speed) {
  if (speed >= 30 && speed <= 200) {
    md1.setDrive(RIGHT_MOTOR, 0, speed);
    delay(1000);
    md1.setDrive(RIGHT_MOTOR, 0, 0);
    delay(200);
    md1.setDrive(RIGHT_MOTOR, 1, speed);
    delay(1000);
    md1.setDrive(RIGHT_MOTOR, 0, 0);
    delay(200);
  }
}

// -- Spin Motor 3 --
// Accepts number between 30 and 200
// Defined Speed
void start_spin_3(int speed) {
  if (speed >= 30 && speed <= 200) {
    md2.setDrive(LEFT_MOTOR, 0, speed);
    delay(1000);
    md2.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
    md2.setDrive(LEFT_MOTOR, 1, speed);
    delay(1000);
    md2.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
  }
}

// -- Spin Motor 4 --
// Accepts number between 30 and 200
// Defined Speed
void start_spin_4(int speed) {
  if (speed >= 30 && speed <= 200) {
    md2.setDrive(RIGHT_MOTOR, 0, speed);
    delay(1000);
    md2.setDrive(RIGHT_MOTOR, 0, 0);
    delay(200);
    md2.setDrive(RIGHT_MOTOR, 1, speed);
    delay(1000);
    md2.setDrive(RIGHT_MOTOR, 0, 0);
    delay(200);
  }
}

// -- Spin Motor 5 --
// Accepts number between 30 and 200
// Defined Speed
void start_spin_5(int speed) {
  if (speed >= 30 && speed <= 200) {
    md3.setDrive(LEFT_MOTOR, 0, speed);
    delay(1000);
    md3.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
    md3.setDrive(LEFT_MOTOR, 1, speed);
    delay(1000);
    md3.setDrive(LEFT_MOTOR, 0, 0);
    delay(200);
  }
}


// Stops All the Spin Motors at once
void stop_all_spins() {
  md1.setDrive(LEFT_MOTOR,  0, 0);
  md1.setDrive(RIGHT_MOTOR, 0, 0);
  md2.setDrive(LEFT_MOTOR,  0, 0);
  md2.setDrive(RIGHT_MOTOR, 0, 0);
  md3.setDrive(LEFT_MOTOR,  0, 0);
}

void set_LED(bool state)
{
  ledState = state;
  digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
}

void get_LED()
{
  notify("get_LED", ledState);
}

void set_motor_position(float position)
{
  motorPosition = position;
  mySerial.print(String("#") + LSS_ID + String("D") + int(motorPosition * 10) + "\r"); // move 100 degrees
  // Add code to set motor position
}

void get_motor_position()
{
  notify("get_motor_position", motorPosition);
  // Add code to set motor position
}

void set_motor_speed(int speed)
{
  motorSpeed = speed;
  mySerial.print(String("#") + LSS_ID + String("WR") + motorSpeed + "\r"); // RPM move
  // Add code to set motor speed
}

void get_IMU()
{
  notify("get_IMU", imuValue);
}

void set_String(String str)
{
  storedString = str;
}

void get_String()
{
  notify("get_String", storedString);
}

void TTS(bool state)
{
  // optional  function to indicating model is talking (true) or not (false)
  digitalWrite(10, state ? HIGH : LOW);
}

// {"function_name", "writeDataType", function}
Command commandFunctions[] = {
    {"change_rotation_speed", "int", change_rotation_speed},
    {"start_spin_1", "int", start_spin_1},
    {"start_spin_2", "int", start_spin_2},
    {"start_spin_3", "int", start_spin_3},
    {"start_spin_4", "int", start_spin_4},
    {"start_spin_5", "int", start_spin_5},
    {"stop_all_spins", "none", stop_all_spins},
    {"set_LED", "bool", set_LED},
    {"get_LED", "none", get_LED},
    {"set_motor_position", "float", set_motor_position},
    {"set_motor_speed", "int", set_motor_speed},
    {"get_motor_position", "none", get_motor_position},
    {"get_IMU", "none", get_IMU},
    {"set_String", "string", set_String},
    {"get_String", "none", get_String},
    {"TTS", "bool", TTS}
};

// Define the number of commands
const int numCommands = sizeof(commandFunctions) / sizeof(commandFunctions[0]);