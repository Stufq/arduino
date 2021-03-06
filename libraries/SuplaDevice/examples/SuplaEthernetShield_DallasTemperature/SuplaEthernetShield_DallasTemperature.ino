#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SuplaDevice.h>

/*
 * SUPLA DEVICE - ARDUINO - ETHERNET SHIELD & DS18B20 SENSOR
 * Author: Przemyslaw Zygmunt <przemek@supla.org>
 *
 * This example requires Dallas Temperature Control library installed. 
 * https://github.com/milesburton/Arduino-Temperature-Control-Library
 */

 
// Setup a oneWire instance
OneWire oneWire(24); // 24 - Pin number

// Pass oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// DS18B20 Sensor read implementation
double get_temperature(int channelNumber, double last_val) {

    double t = -275;
    
    if ( sensors.getDeviceCount() > 0 )
      {
         sensors.requestTemperatures();
         t = sensors.getTempCByIndex(0);
      };

    return t;  
}

void setup() {

  Serial.begin(9600);

  // Init DS18B20 library 
  sensors.begin();
  
  // Set temperature callback
  SuplaDevice.setTemperatureCallback(&get_temperature);
 
  // ﻿Replace the falowing GUID
  char GUID[SUPLA_GUID_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  // ﻿with GUID that you can retrieve from https://www.supla.org/arduino/get-guid


  // Ethernet MAC address
  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

  /*
   * Having your device already registered at cloud.supla.org,
   * you want to change CHANNEL sequence or remove any of them,
   * then you must also remove the device itself from cloud.supla.org.
   * Otherwise you will get "Channel conflict!" error.
   */
    
  // CHANNEL0 - RELAY
  SuplaDevice.addRelay(44, true);           // ﻿44 - ﻿Pin number where the relay is connected      
                                      // Call SuplaDevice.addRelay(44, true) with an extra "true" parameter 
                                      // to enable "port value inversion"
                                      // where HIGH == LOW, and LOW == HIGH   

  // CHANNEL1 - RELAY
  SuplaDevice.addRelay(45, true);           // 45 - ﻿﻿Pin number where the relay is connected   

  // CHANNEL3 - TWO RELAYS (Roller shutter operation)
  SuplaDevice.addRollerShutterRelays(46,     // 46 - ﻿﻿Pin number where the 1st relay is connected   
                                     47, true);    // 47 - ﻿Pin number where the 2nd relay is connected  

  // CHANNEL4 - Opening sensor (Normal Open)
  SuplaDevice.addSensorNO(A0); // A0 - ﻿Pin number where the sensor is connected
                               // Call SuplaDevice.addSensorNO(A0, true) with an extra "true" parameter
                               // to enable the internal pull-up resistor


  // CHANNEL5 - Opening sensor (Normal Open)
  SuplaDevice.addSensorNO(A1); // A1 - ﻿Pin number where the sensor is connected


  // CHANNEL6 - Thermometer DS18B20
  SuplaDevice.addDS18B20Thermometer();


  /*
   * SuplaDevice Initialization.
   * Server address, LocationID and LocationPassword are available at https://cloud.supla.org 
   * If you do not have an account, you can create it at https://cloud.supla.org/account/create
   * SUPLA and SUPLA CLOUD are free of charge
   * 
   */

  SuplaDevice.begin(GUID,              // Global Unique Identifier 
                    mac,               // Ethernet MAC address
                    "svr1.supla.org",  // SUPLA server address
                    0,                 // Location ID 
                    "");               // Location Password
    
}

void loop() {
  SuplaDevice.iterate();
}
