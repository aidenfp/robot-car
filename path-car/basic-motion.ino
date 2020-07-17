#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  8   //Left Motor direction pin 1 to MODEL-X IN4 

/*MOTOR CONTROL*/

uint8_t forward(float distance){
  if(distance == 0) return false;
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_motorspeed(175, 175);
  a_updates[0] = millis();
  v_updates[0] = millis();
  while(fabs(distance-lin_motion[4]) > 0.1){
    update_lin_pos(true);
    if(distance - lin_motion[4] < 0) break;
  }
  car_stop();
  return true;
}

uint8_t rotate_to_ang(float ang){
  if(ang == 0) return false;
  set_motorspeed(150, 150);
  ang_updates[0] = millis();
  if(ang > 0){ //rotate counterclockwise
    digitalWrite(RightMotorDirPin1, HIGH);
    digitalWrite(RightMotorDirPin2, LOW);
    digitalWrite(LeftMotorDirPin1, LOW);
    digitalWrite(LeftMotorDirPin2, HIGH);
    while(fabs(ang-ang_motion[2]) > .05){
      update_ang_pos(true);
      if(ang - ang_motion[2] < 0) break;
    }
  }else{ //rotate clockwise
    digitalWrite(RightMotorDirPin1, LOW);
    digitalWrite(RightMotorDirPin2, HIGH);
    digitalWrite(LeftMotorDirPin1, HIGH);
    digitalWrite(LeftMotorDirPin2, LOW);
    while(fabs(ang-ang_motion[2]) > .05){
      update_ang_pos(true);
      if(ang - ang_motion[2] > 0) break;
    }
  }
  car_stop();
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
  for(int i=0;i<5;i++){
    lin_motion[i] = 0.0;
  }
  for(int i=0;i<3;i++){
    ang_motion[i] = 0.0;
  }
}

/*
int pid(float p, float i, float d){
  k += (float) p*0.5
  k += (float) 
  return (int) k * 255
}*/
