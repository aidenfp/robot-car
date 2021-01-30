#define speedPinR 13    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  16    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  17    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 12    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  5    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  19   //Left Motor direction pin 1 to MODEL-X IN4

const int freq = 500; //pwm frequency
const int motor_channel = 0; //pwm channel
const int resolution = 8; //8-bit pwm resolution

void set_motorspeed(int motor_speed){
  ledcWrite(motor_channel, motor_speed);
}

void car_initialize(){
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  ledcSetup(motor_channel, freq, resolution);
  ledcSetup(motor_channel, freq, resolution);
  ledcAttachPin(speedPinL, motor_channel);
  ledcAttachPin(speedPinR, motor_channel);
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
