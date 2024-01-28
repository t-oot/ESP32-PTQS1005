#include "PTQS1005.h"

// Based on the library for plantower sensors found here: https://github.com/fu-hsi/pms

PTQS1005::PTQS1005(Stream &stream)
{
  this->_stream = &stream;
  this->_status = STATUS_OK;
}

// Standby
void PTQS1005::sleep()
{
  uint8_t command[] = {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73};
  _stream->write(command, sizeof(command));
}

// Wake up
void PTQS1005::wakeUp()
{
  uint8_t command[] = {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74};
  _stream->write(command, sizeof(command));
}

// Request read
bool PTQS1005::requestReadGeneral()
{
  if (_status == STATUS_WAITING)
  {
    return false;
  }
  uint8_t command[] = {0x42, 0x4D, 0xAB, 0x00, 0x00, 0x01, 0x3A};
  _read_mode = MODE_GENERAL;
  _stream->write(command, sizeof(command));
  return true;
}

// Request read
bool PTQS1005::requestReadFull()
{
  if (_status == STATUS_WAITING)
  {
    // return false;
  }
  uint8_t command[] = {0x42, 0x4D, 0xAC, 0x00, 0x00, 0x01, 0x3B};
  _read_mode = MODE_FULL;
  _stream->write(command, sizeof(command));
  return true;
}

// Request read
bool PTQS1005::requestReadFull_TSI()
{
  if (_status == STATUS_WAITING)
  {
    return false;
  }
  uint8_t command[] = {0x42, 0x4D, 0xAD, 0x00, 0x00, 0x01, 0x3C};
  _read_mode = MODE_FULL_TSI;
  _stream->write(command, sizeof(command));
  return true;
}

// Non-blocking function for parse response.
bool PTQS1005::read(DATA &data)
{
  _data = &data;
  loop();

  return _status == STATUS_OK;
}

bool PTQS1005::hasError(){
return _status == STATUS_ERROR;
}
void PTQS1005::reset()
{
  _index = 0;
  _status = STATUS_OK;
}

void PTQS1005::loop()
{
  if (_status == STATUS_ERROR)
  {
    return;
  }
  _status = STATUS_WAITING;
  if (_stream->available())
  {
    uint8_t ch = _stream->read();
    switch (_index)
    {
    case 0:
      if (ch != 0x42)
      {
        LOG("Byte 0 is not 0x42\n");
        while (_stream->available())
        {
          ch = _stream->read();
        }
        _status = STATUS_ERROR;
        return;
      }
      _calculatedChecksum = ch;
      break;

    case 1:
      if (ch != 0x4D)
      {
        LOG("Byte 1 is not 0x4D\n");
        _status = STATUS_ERROR;
        _index = 0;
        return;
      }
      _calculatedChecksum += ch;
      break;

    case 2:
      _calculatedChecksum += ch;
      _frameLen = ch << 8;
      break;

    case 3:
      _frameLen |= ch;

      // Unsupported sensor, different frame length, transmission error e.t.c.
      if (_frameLen != 2 * 9 + 2 && _frameLen != 2 * 13 + 2 && _frameLen != 2 * 18 + 2)
      {
        LOG("Wrong frame length\n");
        _status = STATUS_ERROR;
        _index = 0;
        return;
      }
      _calculatedChecksum += ch;
      break;

    default:
      if (_index == _frameLen + 2)
      {
        _checksum = ch << 8;
      }
      else if (_index == _frameLen + 2 + 1)
      {
        _checksum |= ch;

        if (_calculatedChecksum == _checksum)
        {
          _status = STATUS_OK;

          _data->PM_AE_UG_2_5 = makeWord(_payload[0], _payload[1]);
          _data->TVOC_PPM = (makeWord(_payload[2], _payload[3])) / 100.0;
          _data->HCHO_MGM3 = (makeWord(_payload[5], _payload[6])) / 100.0;
          _data->CO2_PPM = makeWord(_payload[8], _payload[9]);
          _data->TEMP = (makeWord(_payload[10], _payload[11])) / 10.0;
          _data->HUMIDITY = (makeWord(_payload[12], _payload[13])) / 10.0;
          if (_read_mode == MODE_FULL || _read_mode == MODE_FULL_TSI)
          {
            _data->PM_SP_UG_1_0 = makeWord(_payload[0], _payload[1]);
            _data->PM_SP_UG_2_5 = makeWord(_payload[2], _payload[3]);
            _data->PM_SP_UG_10_0 = makeWord(_payload[4], _payload[5]);
            _data->PM_AE_UG_1_0 = makeWord(_payload[6], _payload[7]);
            _data->PM_AE_UG_2_5 = makeWord(_payload[8], _payload[9]);
            _data->PM_AE_UG_10_0 = makeWord(_payload[10], _payload[11]);
            _data->num_particles_0_3 = makeWord(_payload[12], _payload[13]);
            _data->num_particles_0_5 = makeWord(_payload[14], _payload[15]);
            _data->num_particles_1_0 = makeWord(_payload[16], _payload[17]);
            _data->num_particles_2_5 = makeWord(_payload[18], _payload[19]);
            _data->num_particles_5_0 = makeWord(_payload[20], _payload[21]);
            _data->num_particles_10_0 = makeWord(_payload[22], _payload[23]);
            _data->TVOC_PPM = (makeWord(_payload[24], _payload[25])) / 100.0;
            _data->HCHO_MGM3 = (makeWord(_payload[27], _payload[28])) / 100.0;
            _data->CO2_PPM = makeWord(_payload[30], _payload[31]);
            _data->TEMP = (makeWord(_payload[32], _payload[33])) / 10.0;
            _data->HUMIDITY = (makeWord(_payload[34], _payload[35])) / 10.0;
          }
          else
          {
            _data->PM_SP_UG_2_5 = makeWord(_payload[0], _payload[1]);
            _data->TVOC_PPM = (makeWord(_payload[2], _payload[3])) / 100.0;
            _data->HCHO_MGM3 = (makeWord(_payload[5], _payload[6])) / 100.0;
            _data->CO2_PPM = makeWord(_payload[8], _payload[9]);
            _data->TEMP = (makeWord(_payload[10], _payload[11])) / 10.0;
            _data->HUMIDITY = (makeWord(_payload[12], _payload[13])) / 10.0;
            _data->PM_SP_UG_1_0 = 0;
            _data->PM_SP_UG_10_0 = 0;
            _data->PM_AE_UG_1_0 = 0;
            _data->PM_AE_UG_2_5 = 0;
            _data->PM_AE_UG_10_0 = 0;
            _data->num_particles_0_3 = 0;
            _data->num_particles_0_5 = 0;
            _data->num_particles_1_0 = 0;
            _data->num_particles_2_5 = 0;
            _data->num_particles_5_0 = 0;
            _data->num_particles_10_0 = 0;
          }
        }
        else
        {
          LOG("Wrong checksum\n");
          _status = STATUS_ERROR;
        }

        _index = 0;
        return;
      }
      else
      {
        _calculatedChecksum += ch;
        uint8_t payloadIndex = _index - 4;

        if (payloadIndex < sizeof(_payload))
        {
          _payload[payloadIndex] = ch;
        }
      }

      break;
    }

    _index++;
  }
}
