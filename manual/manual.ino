#include <SoftwareSerial.h>

#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  8   //Left Motor direction pin 1 to MODEL-X IN4 

enum direction {
  FORWARD,
  LEFT,
  BACKWARD,
  RIGHT,
  NONE
};

direction prevCommand = NONE;
direction command = NONE;


void setup() {
  Serial.begin(9600);
  Serial.println("initializing");
  car_initialize();
}

void loop() {
  char data = NULL;
  while(Serial.available()) {
    data = Serial.read();
    Serial.println(data);
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
    command = NONE;
  }

  if (command != prevCommand) {
    switch(command) {
      case FORWARD:
        Serial.println("FORWARD");
        set_motorspeed(255, 255);
        forward();
        break;
      case LEFT:
        Serial.println("LEFT");
        set_motorspeed(255, 255);
        left_turn();
        break;
      case BACKWARD:
        Serial.println("BACKWARD");
        set_motorspeed(255, 255);
        backward();
        break;
      case RIGHT:
        Serial.println("RIGHT");
        set_motorspeed(255, 255);
        right_turn();
        break;
      case NONE:
        Serial.println("NONE");
        set_motorspeed(0, 0);
        car_stop();
        break;
    }
  }
  
  prevCommand = command;
}

void set_motorspeed(int speed_L,int speed_R){
  analogWrite(speedPinL,speed_L);
  analogWrite(speedPinR,speed_R);
}

void car_initialize(){
  pinMode(RightMotorDirPin1, OUTPUT); 
  pinMode(RightMotorDirPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  car_stop();
}

void forward() {
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
}

void left_turn() {
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
}

void backward() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
}

void right_turn() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
}

void car_stop(){
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,LOW);
}
