#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  8   //Left Motor direction pin 1 to MODEL-X IN4 


/*motor control*/
uint8_t forward(float distance){
  if(distance == 0) return false;
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_motorspeed(200, 200);
  while(fabs(distance-lin_pos) > 0.5){
    update_lin_pos(true);
    if(distance - lin_pos < 0) break;
  }
  car_stop();
  return true;
}

uint8_t rotate_to_ang(float ang){
  if(ang == 0) return false;
  if(ang > 0){
    digitalWrite(RightMotorDirPin1, LOW);
    digitalWrite(RightMotorDirPin2, HIGH);
    digitalWrite(LeftMotorDirPin1, HIGH);
    digitalWrite(LeftMotorDirPin2, LOW);
    set_motorspeed(255, 255);
    while(fabs(ang-ang_pos) > .01){
      update_ang_pos(true);
    }
    car_stop();
  }else{
    digitalWrite(RightMotorDirPin1, HIGH);
    digitalWrite(RightMotorDirPin2, LOW);
    digitalWrite(LeftMotorDirPin1, LOW);
    digitalWrite(LeftMotorDirPin2, HIGH);
    set_motorspeed(255, 255);
    while(fabs(ang-ang_pos) > .1){
      update_ang_pos(true);
    }
    car_stop();
  }
  return true;
}

void car_stop(){
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,LOW);
  reset_location();
}

void set_motorspeed(int speed_L,int speed_R){
  analogWrite(speedPinL,speed_L);
  analogWrite(speedPinR,speed_R);
}

//Pins initialize
void car_initialize(){
  pinMode(RightMotorDirPin1, OUTPUT); 
  pinMode(RightMotorDirPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  car_stop();
}

void reset_location(){
  lin_pos = 0;
  lin_vel = 0;
  ang_pos = 0;
  ang_vel = 0;
}

/*
int pid(float p, float i, float d){
  k += (float) p*0.5
  k += (float) 
  return (int) k * 255
}*/
