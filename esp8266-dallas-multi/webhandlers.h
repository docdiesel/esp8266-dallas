

/*
 * send temperature data of all sensors as json
 */
void webhdl_send_sensor_all_json(){
  dallas_sensors.requestTemperatures(); 

  String json = "{\n  \"sensors\":\n  {\n";
  for(int i=0;i<numberOfDevices; i++){
    json += "    \""  +  String( myDeviceAddress[i].hexName ) + "\": ";  // add hex address as key
    json += all_sensor_data_to_json( myDeviceAddress[i].devAddr );           // add {c:... f:...} "data line" as value
    if (i < numberOfDevices-1) json += ",";
    json += "\n";
  }
  json += "  }\n}\n";
  
  server.send( 200, "text/plain", json);
  Serial.println("called webhdl_send_sensor_all_json()");
}

/*
 * show device addresses of all sensors found as json array
 */
void webhdl_send_sensor_addresses(){
  String response = "{\n  \"sensors\": [" ;
  for(int i=0;i<numberOfDevices; i++){
    response += "\"";
    response += myDeviceAddress[i].hexName;
    response += "\"";
    if (i < numberOfDevices-1) response += ",";
  }
  response += "]\n}";
  server.send( 200, "application/javascript", response);
  Serial.println("called webhdl_send_sensor_addresses()");
}


/*
 * handler for individual sensor data
 * - send temp plain as °C, °F or both as JSON, depending on URI called
 */
void webhdl_send_sensor_data(){

  // which uri has been called?
  String uri_called = server.uri();
  Serial.print("uri called is ");   Serial.println(uri_called);

  // read temperature data from sensors
  dallas_sensors.requestTemperatures(); 

  // default answer
  String response = "NULL";

  // check if temp is requested for any sensor in °C
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceAddress[i].hexName) + "/c" ){
      // match found
      response = String(dallas_sensors.getTempC(myDeviceAddress[i].devAddr));
    }
  }

  // check if temp is requested for any sensor in °F
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceAddress[i].hexName) + "/f" ){
      // match found
      response = String(dallas_sensors.getTempF(myDeviceAddress[i].devAddr));
    }
  }

  // check if temp is requested for any sensor as json
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceAddress[i].hexName) + "/json" ){
      // match found
      response = all_sensor_data_to_json(myDeviceAddress[i].devAddr);
    }
  }

  // send response
  server.send( 200, "text/plain", response);
  Serial.println("called webhdl_send_sensor_data()");
}

//-- web root
void webhdl_root() {
  server.send(200, "text/plain", "esp8266-dallas-multi v0.1\n");
}

//-- page not found
void webhdl_http404(){
  server.send(404, "text/plain", "404: Not found");
}

