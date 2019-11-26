
# esp8266-dallas-multi

### Using an ESP8266 for reading values from multiple Dallas DS18B20 sensors

I needed to read temperatures in three different places but at the same
time, and the data should be machine readable easily. So I decided to take
an ESP8266 and wire a couple of Dallas sensors to it.

### Web Interface
I wanted to focus on measuring the temperatures so I decided to let some
framework do the network and wifi stuff. In other projects I already used
ESPbase for this and so I included it here, too. When started the first time
or if the configured network isn't available, it starts a wifi access point
for easy configuration. It's still available at root level after
configuration which means a) it's reconfigured easily but b) broken easily,
too, if somebody in your network likes to mess around with open devices. So
be aware of this if you use this sketch.

### Read data
* /sensor/addresses - Once setup and the sensors are wired, this page gives you a json array of
the sensors it found. The addresses will be given in hexadecimal notation
like "0x1234abcd". You need this later when requesting temperature from
single sensors.
* /sensor/all/json - shows the temperatures of all sensors as json,
containing the values both in °C and °F.
* /sensor/0x1234abcd/c - shows the temperature of the specified sensor in °C, sending the data as plain text
* /sensor/0x1234abcd/f - shows the temperature of the specified sensor in °F, sending the data as plain text
* /sensor/0x1234abcd/json - shows the temperature of just the specified sensor both in °C and °F, sending the data as json object

### Configuring the ESP8266
In the code there's a config section where you may adapt two settings for
your needs:
* maxNumDevices - defines the maximum number of sensors the ESP will be able
to handle. This parameter is used to allocate memory for sensor addresses
etc. at runtime.
* GPIO_oneWireBus - defines on which GPIO the data bus of the Dallas sensors
is wired to.
* timer_millis - time in milliseconds at which some cronned code is to be
executed. This is just some option. In my case it's unused.

