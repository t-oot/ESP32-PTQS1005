#ifndef PTQS1005_H
#define PTQS1005_H
#include "Stream.h"
#include "Arduino.h"

#ifdef ESP32
#define LOG(...) esp_log_write(ESP_LOG_ERROR, "PTQS1005", __VA_ARGS__)
#elif defined(Arduino_h)
#define LOG(...) Serial.printf(__VA_ARGS__)
#else
#define LOG(...) \
  {              \
  }
#endif

class PTQS1005
{
public:
  struct DATA
  {
    // SP = Standard Particles, CF=1 ug/m3
    // AE = Atmospheric Environment ug/m3
    uint16_t PM_SP_UG_2_5;
    float TVOC_PPM;
    float HCHO_MGM3;
    uint16_t CO2_PPM;
    float TEMP;
    float HUMIDITY;
    // Below are for full mode only
    uint16_t PM_SP_UG_1_0;
    uint16_t PM_SP_UG_10_0;
    uint16_t PM_AE_UG_1_0;
    uint16_t PM_AE_UG_2_5;
    uint16_t PM_AE_UG_10_0;
    uint16_t num_particles_0_3;  // particles with diameter > 0.3 um in 100 cm3 of air
    uint16_t num_particles_0_5;  // particles with diameter > 0.5 um in 100 cm3 of air
    uint16_t num_particles_1_0;  // particles with diameter > 1.0 um in 100 cm3 of air
    uint16_t num_particles_2_5;  // particles with diameter > 2.5 um in 100 cm3 of air
    uint16_t num_particles_5_0;  // particles with diameter > 5.0 um in 100 cm3 of air
    uint16_t num_particles_10_0; // particles with diameter > 10. um in 100 cm3 of air
  };

  PTQS1005(Stream &);
  void sleep();
  void wakeUp();
  bool hasError();
  void reset();
  bool requestReadGeneral();
  bool requestReadFull();
  bool requestReadFull_TSI();
  bool read(DATA &data);

private:
  enum STATUS
  {
    STATUS_WAITING,
    STATUS_OK,
    STATUS_ERROR,
  };

  enum READ_MODE
  {
    MODE_GENERAL,
    MODE_FULL,
    MODE_FULL_TSI
  };

  uint8_t _payload[36];
  Stream *_stream;
  DATA *_data;
  STATUS _status;
  READ_MODE _read_mode;

  uint8_t _index = 0;
  uint16_t _frameLen;
  uint16_t _checksum;
  uint16_t _calculatedChecksum;

  void loop();
};

#endif
