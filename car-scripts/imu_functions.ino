void initialize_mpu_readings(bool serial){
  int i = 0;
  for(int i = 0;i<INITIALIZE_COUNT;i++){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    lin_offset += a.acceleration.y;
    ang_offset += g.gyro.z;
  }
  ang_offset *= (float) 1/INITIALIZE_COUNT;
  lin_offset *= (float) 1/INITIALIZE_COUNT;
  if(serial){
    Serial.print("Linear offset: ");
    Serial.println(lin_offset);
    Serial.print("Angular offset: ");
    Serial.println(ang_offset);
  }
}


int update_lin_accel(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // downshift all values
  for(int i=ARRAY_SIZE-1;i>1;i--){
    lin_accelerations[i] = lin_accelerations[i-1];
  }
  lin_accelerations[0] = 9.81*(a.acceleration.y - lin_offset);

  // get new lin_accel value
  prev_lin_accel = lin_accel;
  for(int i=0;i<ARRAY_SIZE;i++){
    lin_accel += (float) lin_accelerations[i];
  }
  lin_accel *= (float) 1/ARRAY_SIZE;
  a_updates[1] = a_updates[0];
  a_updates[0] = millis();
  
  if(fabs(lin_accel)<LIN_ACCEL_THRESHOLD) lin_accel = 0;

  return a_updates[0] - a_updates[1];
}


int update_lin_vel(int DELTA){
  prev_lin_vel = lin_vel;
  lin_vel += (float) (lin_accel+prev_lin_accel)/2*DELTA/1000;
  v_updates[1] = v_updates[0];
  v_updates[0] = millis();
  if(fabs(lin_vel)<LIN_VEL_THRESHOLD) lin_vel = 0;
  return v_updates[0] - v_updates[1];
}


void update_lin_pos(bool serial){
  update_lin_accel();
  update_lin_vel();
  lin_pos += (float) lin_vel*DELTA/1000;
  if(serial) print_lin_motion();
}


void print_lin_motion(){
  Serial.print("L.Acceleration: "); Serial.print(lin_accel); Serial.print("  ");
  Serial.print("L.Velocity: "); Serial.print(lin_vel); Serial.print("  ");
  Serial.print("L.Position: "); Serial.print(lin_pos); Serial.print("  ");
  Serial.println("");
}

int update_ang_vel(){
  int start_timer = millis();
  ang_vel = 0;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  for(int i=0;i<ARRAY_SIZE;i++){
    ang_offset += (float) (ang_velocities[i]);
  }
  ang_offset *= (float) 1/ARRAY_SIZE;
  
  for(int i=ARRAY_SIZE-1;i>1;i--){
    ang_velocities[i] = ang_velocities[i-1];
  }
  ang_velocities[0] = g.gyro.z;
  
  for(int i=0;i<ARRAY_SIZE;i++){
    ang_vel += (float) ang_velocities[i];
  }
  ang_vel -= ang_offset;
  ang_vel *= (float) 1/ARRAY_SIZE;
  if(fabs(ang_vel)<ANG_VEL_THRESHOLD) ang_vel = 0;
  
  return millis()-start_timer;
}


void update_ang_pos(bool serial){
  int DELTA = update_ang_vel();
  ang_pos += (float) ang_vel*DELTA/1000/**26.18*/;
  if(serial) print_ang_motion();
}


void print_ang_motion(){
  Serial.print("A.Velocity: "); Serial.print(ang_vel); Serial.print("  ");
  Serial.print("A.Position: "); Serial.print(ang_pos); Serial.print("  ");
  Serial.println("");
}
