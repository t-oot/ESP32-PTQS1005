#include <Arduino.h>
#include <PTQS1005.h>

HardwareSerial mySerial(0); // Depends on your board.
PTQS1005 ptqs(mySerial);

int error_count = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("PTQS1005 Reader");
    mySerial.begin(9600, SERIAL_8N1, D7, D6); // Example for ESP32C3. Change to your pins.
}

void loop()
{

    Serial.println("Requesting data...");
    ptqs.requestReadFull();
    PTQS1005::DATA data;
    int c = 0;
    while (!(ptqs.read(data)))
    {
        delay(1);
        c++;
        if (c > 1000)
        {
            error_count++;
            if (ptqs.hasError())
            {
                Serial.println("Error.");
                ptqs.reset();
            }
            else if (error_count > 10)
            {
                Serial.println("Timtout.");
                ptqs.reset();
            }
            return;
        }
    }
    error_count = 0;
    Serial.println("---------------");
    Serial.print("PM2.5(SP PPM): ");
    Serial.println(data.PM_SP_UG_2_5);
    Serial.print("TVOC(PPM): ");
    Serial.println(data.TVOC_PPM);
    Serial.print("HCHO(Mg/m3): ");
    Serial.println(data.HCHO_MGM3);
    Serial.print("CO2(ppm): ");
    Serial.println(data.CO2_PPM);
    Serial.print("Temp: ");
    Serial.println(data.TEMP);
    Serial.print("Hum: ");
    Serial.println(data.HUMIDITY);
    Serial.println("------");
    Serial.println("PM1.0(SP PPM): ");
    Serial.println(data.PM_SP_UG_1_0);
    Serial.println("PM10.0(SP PPM): ");
    Serial.println(data.PM_SP_UG_10_0);
    Serial.println("PM1.0(AE PPM): ");
    Serial.println(data.PM_AE_UG_1_0);
    Serial.println("PM2.5(AE PPM): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.println("PM10.0(AE PPM): ");
    Serial.println(data.PM_AE_UG_10_0);
    Serial.println("num_particles_0_3: ");
    Serial.println(data.num_particles_0_3);
    Serial.println("num_particles_0_5: ");
    Serial.println(data.num_particles_0_5);
    Serial.println("num_particles_1_0: ");
    Serial.println(data.num_particles_1_0);
    Serial.println("num_particles_2_5: ");
    Serial.println(data.num_particles_2_5);
    Serial.println("num_particles_5_0: ");
    Serial.println(data.num_particles_5_0);
    Serial.println("num_particles_10_0: ");
    Serial.println(data.num_particles_10_0);
    Serial.println("---------------");
    delay(1000);
}
