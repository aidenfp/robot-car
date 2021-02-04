#include "BluetoothSerial.h"
#include "SonicModule.h"
#include <DFRobotDFPlayerMini.h> //for mp3 player
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.
#include <mpu6050_esp32.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define MICROPHONE_PIN 25
#define SONIC_TRIG 26
#define SONIC_ECHO 27

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

BluetoothSerial SerialBT;

HardwareSerial mpSerial(2);
DFRobotDFPlayerMini music_player;
SonicModule sonic_module(SONIC_TRIG, SONIC_ECHO);
MPU6050 imu; 

enum command_type {
  FORWARD,
  LEFT,
  BACKWARD,
  RIGHT,
  STOP,
  TOGGLE_MUSIC,
  NONE
};

command_type prev_command = NONE;
command_type command = NONE;

bool is_playing = false;

#define BACKGROUND TFT_BLACK
#define BALL_COLOR TFT_WHITE

void setup() {
  Serial.begin(115200);
  SerialBT.begin(F("ESP32"));
  Serial.println(F("The device started, now you can pair it with bluetooth!"));
  
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
  Serial.println(F("DFPlayer Mini online."));

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(BACKGROUND);
  tft.setCursor(20, 30, 1);
  tft.setTextSize(10);
  tft.setTextColor(TFT_ORANGE);
  tft.println("^_^");

  if (imu.setupIMU(1)) {
    Serial.println("IMU Connected!");
  } else {
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }

  pinMode(MICROPHONE_PIN, INPUT);

  Serial.println("Initializing motors");
  car_initialize();
}

bool written = false;
unsigned long check_timer = millis();
double dist = 0.0;

void loop() {
  char data = NULL;
  while(SerialBT.available()) {
    data = SerialBT.read();
    //Serial.println(data);
  }
  
  if (data == 'w') {
    command = FORWARD;
  } else if (data == 'a') {
    command = LEFT;
  } else if (data == 's') {
    command = BACKWARD;
  } else if (data == 'd') {
    command = RIGHT;
  } else if (data == 'e') {
    command = STOP;
  } else if (data == 'q') {
    command = TOGGLE_MUSIC;
  }

  if (command != prev_command || (command == TOGGLE_MUSIC && data != NULL)) {
    switch(command) {
      case FORWARD:
        //Serial.println("FORWARD");
        set_motorspeed(255);
        forward();
        break;
      case LEFT:
        //Serial.println("LEFT");
        set_motorspeed(255);
        left_turn();
        break;
      case BACKWARD:
        //Serial.println("BACKWARD");
        set_motorspeed(255);
        backward();
        break;
      case RIGHT:
        //Serial.println("RIGHT");
        set_motorspeed(255);
        right_turn();
        break;
      case STOP:
        //Serial.println("STOP");
        set_motorspeed(0);
        car_stop();
        break;
      case TOGGLE_MUSIC:
        if (!is_playing) {
          is_playing = true;
          music_player.playFolder(10, random(1, 8));
        } else {
          is_playing = false;
          music_player.pause();
        }
      case NONE:
        break;
    }
  }  
  prev_command = command;
}
