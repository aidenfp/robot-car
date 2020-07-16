void parse_point(char* point){
  target_r = atof(&strtok(point, ",")[1]);
  target_theta = atof(strtok(NULL, "))"));
  Serial.print("r: "); Serial.println(target_r);
  Serial.print("theta: "); Serial.println(target_theta);
}


uint8_t append(char* buff, char c, uint16_t buff_size){
  int len = strlen(buff);
  if (len>buff_size) return false;
  buff[len] = c;
  buff[len+1] = '\0';
  return true;
}

 
void http_request(char* request, char* buff, uint16_t buff_size, uint16_t response_timeout, uint8_t serial){
  WiFiEspClient client;
  if (serial) Serial.println(request);
  if(client.connect(host, 80)){
    client.println(request);
    memset(buff, 0, buff_size); //Null out (0 is the value of the null terminator '\0') entire buff
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n', buff, buff_size);
      if (serial) Serial.println(buff);
      if (strcmp(buff,"\r")==0) { //found a blank line!
        break;
      }
      memset(buff, 0, buff_size);
      if (millis()-count>response_timeout) break;
    }
    memset(buff, 0, buff_size);  
    count = millis();
    while (client.available()) { //read out remaining text (body of response)
      append(buff,client.read(),buff_size);
    }
    if (serial) Serial.println(buff);
    client.stop();
    if (serial) Serial.println("-----------");  
  }else{
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
    
  }
}
