#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#pragma once

class SonicModule {
    int TRIG;
    int ECHO;
    bool is_trigger;
    bool is_echo;
    unsigned long pulse_timer;
    unsigned long pulse_start;
    float distance;

    public:
    SonicModule(int trigger_pin, int echo_pin);

    void update();

    float get_distance();
};

enum action_type {
  FORWARD,
  TURN_LEFT,
  BACKWARD,
  TURN_RIGHT,
  VOICE,
  PLAY_SONG,
  NONE
};

class BotAction {
  action_type type;
  int duration;
  bool active;
  public:
  static const uint16_t MAX_ACTION_DURATION = 5000;
  static const uint16_t MIN_ACTION_DURATION = 500;
  BotAction();

  void randomize();

  void set(action_type action, int dur);

  action_type get_type();

  int get_duration();

  bool is_active();
  void toggle_active();
};

class BotCar {
  /* MOTOR PINS*/
  uint8_t SPEED_RIGHT;
  uint8_t SPEED_LEFT;
  uint8_t RIGHT_ONE;
  uint8_t RIGHT_TWO;
  uint8_t LEFT_ONE;
  uint8_t LEFT_TWO;

  /* PWM VARIABLES */
  const uint8_t PWM_RESOLUTION = 8;
  const uint8_t PWM_CHANNEL = 0;
  const uint16_t PWM_FREQUENCY = 500;
  const uint8_t speed = 255;

  /* SENSING */
  Adafruit_MPU6050* imu;
  const static uint8_t ARRAY_SIZE = 24;  // how many samples to store
  const uint16_t INITIALIZE_COUNT = 1000;  // how many samples to take when finding offset
  const float ANG_VEL_THRESHOLD = 0.01;
  float prev_ang_vel = 0;
  float ang_vel = 0;
  float ang_pos = 0;
  float ang_offset = 0;
  float ang_velocities[ARRAY_SIZE] = {0};
  unsigned long new_timing;
  unsigned long prev_timing;
  void initialize_imu_readings();
  unsigned long update_angular_velocity();
  void update_angular_position(bool print);
  void print_angular_motion();

  /* DRIVING */
  void set_motorspeed(int motorspeed);
  void forward();
  void backward();
  void left();
  void right();
  void stop();

  /* TIMING VARIABLES */
  unsigned long action_timer;
  unsigned long timeout_timer;
  bool is_executing = false;
  
  public:
  BotCar(Adafruit_MPU6050* imu_point, int speed_right, int speed_left, int right_one, int right_two, int left_one, int left_two);

  void begin();
  
  void execute(BotAction* action);

  void break_execution(BotAction* action);

  void rotate(float angle);

  unsigned long millis_since_last_action();
};
