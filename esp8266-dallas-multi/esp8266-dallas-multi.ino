/*
 * 
 * Copyright 2019 Bernd Künnen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in the 
 * Software without restriction, including without limitation the rights to use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the 
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * --------
 * This piece of code was inspired by & thanks to thre authors of:
 *  - https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
 *  - https://lastminuteengineers.com/multiple-ds18b20-esp8266-nodemcu-tutorial/
 *  - https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/
 *
 *
 */
 
#include <Arduino.h>
#include <ESPBASE.h>

#include <OneWire.h>
#include <DallasTemperature.h>


//==== config section ===================

#define timer_millis 10000

//-- define how many dallas sensors should be supported
#define maxNumDevices   8

// GPIO where the DS18B20 is connected to
#define GPIO_oneWireBus 4



//==== global objects ========
ESPBASE Esp;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireInst(GPIO_oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature dallas_sensors(&oneWireInst);

//-- array of sensor addresses, plain and as hex string
struct SensorAddress {
  DeviceAddress devAddr;
  char      hexName[18];
} ;
SensorAddress myDeviceAddress[maxNumDevices];

//-- counter for devices
int numberOfDevices;
int numberOfDevicesFound;

//-- counter for cronned code
int timer = 0;

//-- global buffer for converting device addresses to hex string
char hx[18];

#include "tools.h"
#include "webhandlers.h"


//==== fire up your engines ... ====
void setup() {
  Serial.begin(115200);     // init serial
  Esp.initialize();         // init ESPbase
  dallas_sensors.begin();   // init dallas sensors

  find_sensor_addresses();  // build internal list of sensors

  //-- setup web server ressources
  server.on ( "/", webhdl_root );
  server.onNotFound(webhdl_http404);
  server.on ( "/sensor/addresses", webhdl_send_sensor_addresses );
  server.on ( "/sensor/all/json",  webhdl_send_sensor_all_json  );

  //-- individual sensor uris like /sensor/0x1234abcd/format
  for(int i=0; i<numberOfDevices; i++){
    server.on ( "/sensor/" + String(myDeviceAddress[i].hexName) + "/c" ,    webhdl_send_sensor_data );
    server.on ( "/sensor/" + String(myDeviceAddress[i].hexName) + "/f" ,    webhdl_send_sensor_data );
    server.on ( "/sensor/" + String(myDeviceAddress[i].hexName) + "/json" , webhdl_send_sensor_data );
  }

  //-- init timer for cronned function
  timer = millis();
}

//==== Run, Forrest, run! ====
void loop() {
  //-- ESPbase routines first
  ArduinoOTA.handle();        // OTA request handling
  server.handleClient();      // WebServer requests handling
  customWatchdog = millis();  // feed the DOG :)

  //-- Add normal Sketch code here ...

  if (millis() > timer + timer_millis) 
  {
    timer = millis();
    cronned_code();
  }
}


//==== got some code to execute every few seconds? ====
void cronned_code(){
  Serial.println("This would be the cronned Fnord.");
}
