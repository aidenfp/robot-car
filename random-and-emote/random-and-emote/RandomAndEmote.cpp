#include "RandomAndEmote.h"

/* SONIC MODULE */
SonicModule::SonicModule(int trigger_pin, int echo_pin) {
    TRIG = trigger_pin;
    ECHO = echo_pin;
    pulse_timer = micros();
    pulse_start = micros();
    is_trigger = false;
    is_echo = false;
    distance = 0.0;
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

void SonicModule::update() {
    if (!is_trigger) {
        pulse_timer = micros();
        is_trigger = true;
        digitalWrite(TRIG, HIGH);
    }
    if (micros() - pulse_timer > 10 && is_trigger) {
        digitalWrite(TRIG, LOW);
        is_trigger = false;
        if(!is_echo && digitalRead(ECHO) == HIGH) {
            pulse_start = micros();
            is_echo = true;
        }
        if (is_echo && digitalRead(ECHO) == LOW) {
            double duration = micros() - pulse_start;
            distance = duration * 0.017;
            is_trigger = false;
            is_echo = false;
        }
    }
}

float SonicModule::get_distance() {
    return distance;
}


/* BOT ACTIONS */
BotAction::BotAction() {
  active = false;
  type = NONE;
  duration = 0;
}

void BotAction::randomize() {
  duration = random(MIN_ACTION_DURATION, MAX_ACTION_DURATION);  //duration in millis 
  int rand_num = random(100);
  if (rand_num >= 0 && rand_num < 15) {
    type = NONE;
  } else if (rand_num >= 15 && rand_num < 37) {
    type = FORWARD;
  } else if (rand_num >= 37 && rand_num < 49) {
    type = BACKWARD;
  } else if (rand_num >= 49 && rand_num < 61) {
    type = TURN_RIGHT;
  } else if (rand_num >= 61 && rand_num < 73) {
    type = TURN_LEFT;
  } else if (rand_num >= 73 && rand_num < 85) {
    type = NONE;//PLAY_SONG;
  } else {
    type = NONE;//VOICE;
  }
  active = true;
}

void BotAction::set(action_type action, int dur) {
  type = action;
  duration = dur;
  active = true;
}

action_type BotAction::get_type() {
  return type;
}

int BotAction::get_duration() {
  return duration;
}

bool BotAction::is_active() {
  return active;
}

void BotAction::toggle_active() {
  active = !active;
}


/* BOT CAR */
/* PRIVATE */
void BotCar::initialize_imu_readings() {
  int i = 0;
  for(int i = 0;i<INITIALIZE_COUNT;i++){
    sensors_event_t a, g, temp;
    imu->getEvent(&a, &g, &temp);
    ang_offset += g.gyro.z;
  }
  ang_offset *= -1.0/INITIALIZE_COUNT;
}

unsigned long BotCar::update_angular_velocity() {
  sensors_event_t a, g, temp;
  imu->getEvent(&a, &g, &temp);
  
  for(int i=ARRAY_SIZE-1;i>1;i--){
    ang_velocities[i] = ang_velocities[i-1];
  }
  ang_velocities[0] = g.gyro.z - ang_offset;

  prev_ang_vel = ang_vel;
  for(int i=0;i<ARRAY_SIZE;i++){
    ang_vel += (float) ang_velocities[i];
  }
  ang_vel *= (float) 1/ARRAY_SIZE;
  prev_timing = new_timing;
  new_timing = millis();
  
  if(fabs(ang_vel)<ANG_VEL_THRESHOLD) ang_vel = 0;
  
  return new_timing - prev_timing; // measurement time
}

//grab time delta and use midpoint approximation
void BotCar::update_angular_position(bool print) {
  int DELTA = update_angular_velocity();
  ang_pos += (float) (ang_vel+prev_ang_vel)/2*DELTA/1000*3.14/.14;

  if(print) print_angular_motion();
}

void BotCar::print_angular_motion() {
  Serial.print("A.Velocity: "); Serial.print(ang_vel); Serial.print("  ");
  Serial.print("A.Position: "); Serial.print(ang_pos); Serial.print("  ");
  Serial.println("");
}

void BotCar::set_motorspeed(int motorspeed) {
  ledcWrite(PWM_CHANNEL, motorspeed);
}

void BotCar::forward() {
  set_motorspeed(speed);
  digitalWrite(RIGHT_ONE, HIGH);
  digitalWrite(RIGHT_TWO, LOW);
  digitalWrite(LEFT_ONE, HIGH);
  digitalWrite(LEFT_TWO, LOW);
  is_executing = true;
}

void BotCar::backward() {
  set_motorspeed(speed);
  digitalWrite(RIGHT_ONE, LOW);
  digitalWrite(RIGHT_TWO, HIGH);
  digitalWrite(LEFT_ONE, LOW);
  digitalWrite(LEFT_TWO, HIGH);
  is_executing = true;
}

void BotCar::left() {
  set_motorspeed(speed);
  digitalWrite(RIGHT_ONE, HIGH);
  digitalWrite(RIGHT_TWO, LOW);
  digitalWrite(LEFT_ONE, LOW);
  digitalWrite(LEFT_TWO, HIGH);
  is_executing = true;
}

void BotCar::right() {
  set_motorspeed(speed);
  digitalWrite(RIGHT_ONE, LOW);
  digitalWrite(RIGHT_TWO, HIGH);
  digitalWrite(LEFT_ONE, HIGH);
  digitalWrite(LEFT_TWO, LOW);
  is_executing = true;
}

void BotCar::stop() {
  set_motorspeed(0);
  digitalWrite(RIGHT_ONE, LOW);
  digitalWrite(RIGHT_TWO, LOW);
  digitalWrite(LEFT_ONE, LOW);
  digitalWrite(LEFT_TWO, LOW);
  is_executing = false;
}

/* PUBLIC */
BotCar::BotCar(Adafruit_MPU6050* imu_pointer, int speed_right, int speed_left, int right_one, int right_two, int left_one, int left_two) {
  imu = imu_pointer;
  SPEED_RIGHT = speed_right;
  SPEED_LEFT = speed_left;
  RIGHT_ONE = right_one;
  RIGHT_TWO = right_two;
  LEFT_ONE = left_one;
  LEFT_TWO = left_two;
}

void BotCar::begin() {
  if (imu->begin()) {
    Serial.println("IMU initialized");
    imu->setAccelerometerRange(MPU6050_RANGE_4_G);
    imu->setGyroRange(MPU6050_RANGE_250_DEG);
    imu->setFilterBandwidth(MPU6050_BAND_184_HZ);
    initialize_imu_readings();
  } else {
    Serial.println("IMU not connected");
    Serial.println("Restarting");
    ESP.restart();
  }
  
  pinMode(SPEED_RIGHT, OUTPUT);
  pinMode(SPEED_LEFT, OUTPUT);
  pinMode(RIGHT_ONE, OUTPUT);
  pinMode(RIGHT_TWO, OUTPUT);
  pinMode(LEFT_ONE, OUTPUT);
  pinMode(LEFT_TWO, OUTPUT);
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(SPEED_RIGHT, PWM_CHANNEL);
  ledcAttachPin(SPEED_LEFT, PWM_CHANNEL);
  stop();
}

void BotCar::execute(BotAction* action) {
  if (action->is_active()) {
    timeout_timer = millis();
    int duration = action->get_duration();
    switch(action->get_type()) {
      case NONE:
      if (!is_executing) {
        Serial.print("My action is "); Serial.print("NONE"); Serial.print(" for "); Serial.println(duration);
        is_executing = true;
        action_timer = millis();
      } 
      if (millis() - action_timer > duration) {
        Serial.println("Action done");
        action->toggle_active();
        is_executing = false;
      }
      break;
      case FORWARD:
      if (!is_executing) {
        Serial.print("My action is "); Serial.print("FORWARD"); Serial.print(" for "); Serial.println(duration);
        forward();
        action_timer = millis();
      } else if (millis() - action_timer > duration) {
        Serial.println("Action done");
        action->toggle_active();
        stop();
      }
      break;
      case BACKWARD:
      if (!is_executing) {
        Serial.print("My action is "); Serial.print("BACKWARD"); Serial.print(" for "); Serial.println(duration);
        backward();
        action_timer = millis();
      } else if (millis() - action_timer > duration) {
        Serial.println("Action done");
        action->toggle_active();
        stop();
      }
      break;
      case TURN_RIGHT:
      if (!is_executing) {
        Serial.print("My action is "); Serial.print("TURN_RIGHT"); Serial.print(" for "); Serial.println(duration);
        right();
        action_timer = millis();
      } else if (millis() - action_timer > duration) {
        Serial.println("Action done");
        action->toggle_active();
        stop();
      }
      break;
      case TURN_LEFT:
      if (!is_executing) {
        Serial.print("My action is "); Serial.print("TURN_LEFT"); Serial.print(" for "); Serial.println(duration);
        left();
        action_timer = millis();
      } else if (millis() - action_timer > duration) {
        Serial.println("Action done");
        action->toggle_active();
        stop();
      }
      break;
      case PLAY_SONG: //TODO
      Serial.print("My action is "); Serial.print("PLAY_SONG");
      action->toggle_active();
      break;
      case VOICE: //TODO
      Serial.print("My action is "); Serial.print("VOICE");
      action->toggle_active();
      break;
    }
  }
}

void BotCar::break_execution(BotAction* action) {
  stop();
  if (action->is_active()) {
    action->toggle_active();
    timeout_timer = millis();
  }
}

void BotCar::rotate(float angle) {
  if(angle != 0) {
    new_timing = millis();
    if(angle > 0){ //rotate counterclockwise
      left();
      while(fabs(angle-ang_pos) > .01){
        update_angular_position(true);
        if(angle - ang_pos < 0) break;
      }
    } else { //rotate clockwise
      right();
      while(fabs(angle-ang_pos) > .01){
        update_angular_position(true);
        if(angle - ang_pos > 0) break;
      }
    }
  }
  ang_pos = 0;
  stop();
}

unsigned long BotCar::millis_since_last_action() {
  return millis() - timeout_timer;
}
