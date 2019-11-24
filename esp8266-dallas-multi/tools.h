/*
 * get temp data for given sensor and build json string
 */
String all_sensor_data_to_json(DeviceAddress devAddr){
  String json = "";
  json += "{ \"c\": "  +  String(dallas_sensors.getTempC(devAddr)) ;
  json += ", \"f\": "  +  String(dallas_sensors.getTempF(devAddr)) + " }";
  return json;
}



/*
 * convert byte to hex chars
 */
String byte2hex(int convertible)
{
  char hexdigit[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  char result[3];
  result[0] = hexdigit[ convertible >> 4   ];
  result[1] = hexdigit[ convertible &  0xF ];
  result[2] = '\0';
  return result;
}


/*
 * build hex string "0x1234abcd" from device address
 */
void toHexString(DeviceAddress deviceAddress) {
  hx[0] = '0';
  hx[1] = 'x';
  for (uint8_t i = 0; i < 8; i++){
    hx[2+i*2] = byte2hex(deviceAddress[i])[0];
    hx[3+i*2] = byte2hex(deviceAddress[i])[1];
  }
}

/*
 * look for sensors and save their device addresses, plain and as hex string
 */
void find_sensor_addresses(){
  DeviceAddress tempDeviceAddress;
  char tempHexString[18];
  
  //-- check num of sensors; print error if there's too much
  numberOfDevicesFound = dallas_sensors.getDeviceCount();
  Serial.println("Found " + String(numberOfDevicesFound) + " sensors");
  if (numberOfDevicesFound>maxNumDevices){
    Serial.println("but can handle max. " + String(maxNumDevices) + " => omitting rest");
    numberOfDevices = maxNumDevices;
  }
  else  numberOfDevices = numberOfDevicesFound;

  //-- fill internal array with addresses in plain and as hex string
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(dallas_sensors.getAddress(tempDeviceAddress, i)){
      memcpy( myDeviceAddress[i].devAddr, tempDeviceAddress, sizeof(tempDeviceAddress) );
      toHexString(tempDeviceAddress);
      memcpy( myDeviceAddress[i].hexName, hx, sizeof(myDeviceAddress[i].hexName) );
      Serial.print("Found device "   + String( i ));
      Serial.print(" with address: " + String( myDeviceAddress[i].hexName ) );
      Serial.println();
    } else {
      Serial.print("Found ghost device at " + String(i) );
      Serial.print(" but could not detect address. Check power and cabling");
      Serial.println();
    }
  }
}
