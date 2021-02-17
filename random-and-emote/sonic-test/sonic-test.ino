#include "BluetoothSerial.h"
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
DFRobotDFPlayerMini musicPlayer;
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

boolean is_playing = false;

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
  if (!musicPlayer.begin(mpSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(musicPlayer.readType(), HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  musicPlayer.setTimeOut(500);
  musicPlayer.volume(30);  //Set volume value (0~30).
  musicPlayer.EQ(DFPLAYER_EQ_NORMAL);
  musicPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println(F("DFPlayer Mini online."));

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(BACKGROUND);

  if (imu.setupIMU(1)) {
    Serial.println("IMU Connected!");
  } else {
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }

  pinMode(SONIC_TRIG, OUTPUT);
  pinMode(SONIC_ECHO, INPUT);

  pinMode(MICROPHONE_PIN, INPUT);

  Serial.println("Initializing motors");
  car_initialize();
}

boolean written = false;
boolean started = false;
unsigned long micro = micros();
unsigned long milli = millis();
unsigned long start = micros();

void loop() {
  /*if (millis() - milli > 10) {
    milli = millis();
    Serial.print("microphone"); Serial.println(analogRead(A0));
  }*/

  if (!written) {
    micro = micros();
    written = true;
    digitalWrite(SONIC_TRIG, HIGH);
  }

  if (micros() - micro > 10 && written) {
    digitalWrite(SONIC_TRIG, LOW);
    written = false;
    if(!started && digitalRead(SONIC_ECHO) == HIGH) { 
      start = micros(); 
      started = true;
    }
    if (started && digitalRead(SONIC_ECHO) == LOW) {
      int duration = micros() - start;
      Serial.print("sonic"); Serial.println((double) duration * 0.017);
      written = false;
      started = false;
    }
  }
  /*char data = NULL;
  while(SerialBT.available()) {
    data = SerialBT.read();
    //Serial.println(data);
  }

  if (!written) {
    tft.fillCircle(15, 15, 3, BALL_COLOR);
    written = true;
  }

  int track = random(11, 18);
  
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

  if (command != prev_command || command == TOGGLE_MUSIC) {
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
          musicPlayer.play(track);
        } else {
          is_playing = false;
          musicPlayer.pause();
        }
      case NONE:
        break;
    }
  }
  
  prev_command = command;*/
}
