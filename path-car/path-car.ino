#include <string.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/*
//state variables
#define IDLE 0 
#define DRIVE 1
#define ARRIVE 2
uint8_t car_state = IDLE;
*/

// http request buffer and parameters
const uint16_t response_timeout = 5000;
const uint8_t BUFFER_SIZE = 40; //size of HTTP buffer
char http_response[BUFFER_SIZE]; //char array buffer to hold HTTP response
char* current_point, next_point;

// global position variables and imu parameters
const uint8_t ARRAY_SIZE = 24;
const uint16_t INITIALIZE_COUNT = 1000;

const float LIN_ACCEL_THRESHOLD = 0.1;
const float LIN_VEL_THRESHOLD = 0.025;
float lin_motion[6] = {0}; //prev_lin_accel, lin_accel, prev_lin_vel, lin_vel, lin_pos, lin_offset
float lin_accelerations[ARRAY_SIZE] = {0};

const float ANG_VEL_THRESHOLD = 0.01;
float ang_motion[4] = {0}; //prev_ang_vel, ang_vel, ang_pos, ang_offset
float ang_velocities[ARRAY_SIZE] = {0};

float target_r, target_theta;

// timing variables
int a_updates[2] = {0};
int v_updates[2] = {0};
int ang_updates[2] = {0};

// Emulate Serial1 on pins 4/5
#ifndef HAVE_HWSERIAL1
SoftwareSerial Serial1(4, 5); // RX, TX
#endif

const char ssid[] = "JRJ8M";            // your network SSID (name)
const char pass[] = "abc1768def";        // your network password
const char request_string[] = "GET /sandbox/sc/aidenfp/car/car_requests.py HTTP/1.1\r\nHost: 608dev-2.net\r\n";
const char host[] = "608dev-2.net";

int status = WL_IDLE_STATUS;     // the Wifi radio's status

Adafruit_MPU6050 mpu;

void setup(){
  // initialize serial and software serial for esp32
  Serial.begin(9600);
  Serial1.begin(9600);
  // initialize MPU
  if(mpu.begin()) Serial.println("MPU Found"); else Serial.println("MPU not found");
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
  initialize_mpu_readings(true); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
}


void loop(){
  http_request(request_string, http_response, BUFFER_SIZE, response_timeout, true);
  parse_point(http_response);
  rotate_to_ang(target_theta);
  forward(target_r);
}

/*
void car_fsm(){
  update_lin_pos(false);
  update_ang_pos(false);
  switch(car_state){
    case IDLE:
    http_request(request_string, http_response, BUFFER_SIZE, response_timeout, true);
    if(strlen(http_response) != 0) strcat(current_point, http_response); else Serial.println("Unable to retrieve point");
    http_request(request_string, http_response, BUFFER_SIZE, response_timeout, true);
    if(strlen(http_response) != 0) strcat(next_point, http_response); else Serial.println("Unable to retrieve point");
    parse_point(current_point);
    if(fabs(target_r) > 0) car_state = DRIVE;
    break;
    case DRIVE:
    break;
    case ARRIVE:
    break;
  }
}*/
