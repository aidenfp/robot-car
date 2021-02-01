#include "BluetoothSerial.h"
#include "BlindCat.h"
#include <DFRobotDFPlayerMini.h> //for mp3 player
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/* MOTOR PINS */
#define speedPinR 13    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  16    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  17    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 12    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  5    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  19   //Left Motor direction pin 1 to MODEL-X IN4

/* SENSOR PINS */
#define MICROPHONE_PIN 25
#define SONIC_TRIG 26
#define SONIC_ECHO 27

/* TFT COLORS */
#define BACKGROUND TFT_BLACK
#define BALL_COLOR TFT_WHITE

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

BluetoothSerial SerialBT;

HardwareSerial mpSerial(2);
DFRobotDFPlayerMini music_player;

SonicModule sonic_module(SONIC_TRIG, SONIC_ECHO);
float distance = 0.0;

Adafruit_MPU6050* imu = new Adafruit_MPU6050;

BotCar car(imu, speedPinR, speedPinL, RightMotorDirPin1, RightMotorDirPin2, LeftMotorDirPin1, LeftMotorDirPin2);
BotAction* current_action = new BotAction;

unsigned long last_sonic_update;
unsigned long last_rotation;
const uint16_t ACTION_TIMEOUT = 1500;

void setup() {
  // begin Serial and bluetooth
  Serial.begin(115200);
  SerialBT.begin(F("ESP32"));
  Serial.println(F("The device started, now you can pair it with bluetooth!"));

  // initialize mp3 module
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  mpSerial.begin(9600, SERIAL_8N1, 32, 33);  // speed, type, RX, TX
  delay(1000);
  if (!music_player.begin(mpSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(music_player.readType(), HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  music_player.setTimeOut(500);
  music_player.volume(30);  //Set volume value (0~30).
  music_player.EQ(DFPLAYER_EQ_NORMAL);
  music_player.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println(F("MP3 initialized"));

  // initialize the display
  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(BACKGROUND);

  // initialize microphone
  pinMode(MICROPHONE_PIN, INPUT);

  // initialize car
  car.begin();
  
  // set random seed and randomize current_action
  randomSeed(analogRead(14));
  current_action->randomize();

  // begin timers
  last_rotation = millis();
  last_sonic_update = millis();
}

void loop() {
  /*if (millis() - milli > 10) {
    milli = millis();
    Serial.print("microphone"); Serial.println(analogRead(A0));
  }*/
  
  sonic_module.update();
  if (millis() - last_sonic_update > 10) {
    distance = sonic_module.get_distance();
    last_sonic_update = millis();
  }
  
  car.execute(current_action);

  if (car.millis_since_last_action() > ACTION_TIMEOUT) {
    Serial.println("Generating new action");
    current_action->randomize();
  }
  
  if (distance < 15 && millis() - last_rotation > 100) {
    Serial.println("rotating");
    car.break_execution(current_action);
    car.rotate(3.14);
    current_action->set(FORWARD, 1000);
    last_rotation = millis();
  }
}
