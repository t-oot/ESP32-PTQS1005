# ESP32-PTQS1005

PTQS1005 Sensor Data Acquisition Library for ESP32 and Arduino

## Features

* Read PM1.0, PM2.5, PM10, TVOC, HCHO, CO2, Temperature, Humidity
  * Supports Standard Particles and Atmospheric Environment readings
* Read particles count in >=0.3um, >=0.5um, >=1.0um, >=2.5um, >=5.0um, >=10um / 0.1L
* Error checking
* Supports sleep/wake up control

## Usage

```cpp
#include <ESP32-PTQS1005.h>
HardwareSerial mySerial(0); // Depends on your board.
PTQS1005 ptqs(mySerial);

void setup() {
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // Depends on your board.
  Serial.begin(115200);
}

void loop(){
    ptqs.requestReadFull();
    PTQS1005::DATA data;
    int c = 0;
    while (!(ptqs.read(data))) {}
    Serial.printf("PM1.0: %d ug/m3\n", data.PM_AE_UG_1_0);
    Serial.printf("PM2.5: %d ug/m3\n", data.PM_AE_UG_2_5);
    Serial.printf("PM10: %d ug/m3\n", data.PM_AE_UG_10_0);
    ... // See example for more
}
```

* Read mode
    * `requestReadFull()` - Request to read all data
    * `requestReadFull_TSI()` - Request to read all data (TSI mode)
    * `requestReadGeneral()` - Request to read only PM2.5(SP), TVOC, HCHO, CO2, Temperature, Humidity


## Remarks

Tested with PTQS1005 and Seeed Studio XIAO ESP32C3.

## Contributes

Issues/PR due to bugs, typos and improvements are welcome.

## Changelog

* 1.0.0
    * Initial release