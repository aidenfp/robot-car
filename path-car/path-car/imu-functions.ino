void initialize_mpu_readings(bool serial){
  int i = 0;
  for(int i = 0;i<INITIALIZE_COUNT;i++){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    lin_motion[5] += a.acceleration.y;
    ang_motion[3] += g.gyro.x;
  }
  ang_motion[3] *= (float) 1/INITIALIZE_COUNT;
  lin_motion[5] *= (float) 1/INITIALIZE_COUNT;
  if(serial){
    Serial.print("Linear offset: ");
    Serial.println(lin_motion[5]);
    Serial.print("Angular offset: ");
    Serial.println(ang_motion[3]);
  }
}


int update_lin_accel(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // downshift all values
  for(int i=ARRAY_SIZE-1;i>1;i--){
    lin_accelerations[i] = lin_accelerations[i-1];
  }
  lin_accelerations[0] = 9.81*(a.acceleration.y - lin_motion[5]);

  // get new lin_accel value then update timing
  lin_motion[0] = lin_motion[1];
  for(int i=0;i<ARRAY_SIZE;i++){
    lin_motion[1] += (float) lin_accelerations[i];
  }
  lin_motion[1] *= (float) 1/ARRAY_SIZE;
  a_updates[1] = a_updates[0];
  a_updates[0] = millis();
  
  if(fabs(lin_motion[1])<LIN_ACCEL_THRESHOLD) lin_motion[1] = 0;

  return a_updates[0] - a_updates[1];
}


int update_lin_vel(int DELTA){
  // downshift lin_vel values then use midpoint approximation
  lin_motion[2] = lin_motion[3]; 
  lin_motion[3] += (float) (lin_motion[1]+lin_motion[0])/2*DELTA/1000;
  v_updates[1] = v_updates[0];
  v_updates[0] = millis();
  
  if(fabs(lin_motion[3])<LIN_VEL_THRESHOLD) lin_motion[3] = 0;

  return v_updates[0] - v_updates[1];
}


void update_lin_pos(bool serial){
  int DELTA = update_lin_vel(update_lin_accel());
  
  // use midpoint approximation and convert to cm for scaling
  lin_motion[4] += (float) (lin_motion[3]+lin_motion[2])/2*DELTA/10;
  if(serial) print_lin_motion();
}


void print_lin_motion(){
  Serial.print("L.Acceleration: "); Serial.print(lin_motion[1]); Serial.print("  ");
  Serial.print("L.Velocity: "); Serial.print(lin_motion[3]); Serial.print("  ");
  Serial.print("L.Position: "); Serial.print(lin_motion[4]/100); Serial.print("  ");
  Serial.println("");
}


int update_ang_vel(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  for(int i=ARRAY_SIZE-1;i>1;i--){
    ang_velocities[i] = ang_velocities[i-1];
  }
  ang_velocities[0] = -(g.gyro.z - ang_motion[3]);

  ang_motion[0] = ang_motion[1];
  for(int i=0;i<ARRAY_SIZE;i++){
    ang_motion[1] += (float) ang_velocities[i];
  }
  ang_motion[1] *= (float) 1/ARRAY_SIZE;
  ang_updates[1] = ang_updates[0];
  ang_updates[0] = millis();
  
  if(fabs(ang_motion[1])<ANG_VEL_THRESHOLD) ang_motion[1] = 0;
  
  return ang_updates[0] - ang_updates[1];
}


void update_ang_pos(bool serial){
  //grab time delta and use midpoint approximation
  int DELTA = update_ang_vel();
  ang_motion[2] += (float) (ang_motion[1]+ang_motion[0])/2*DELTA/1000*3.14/.14;
  
  if(serial) print_ang_motion();
}


void print_ang_motion(){
  Serial.print("A.Velocity: "); Serial.print(ang_motion[1]); Serial.print("  ");
  Serial.print("A.Position: "); Serial.print(ang_motion[2]); Serial.print("  ");
  Serial.println("");
}
