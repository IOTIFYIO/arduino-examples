#include <Wire.h>

class TMP105 {
public:
  bool begin(int addrSel = 0) {
    switch (addrSel) {
    case 0:
      i2cAddr = 0x48;
      break;
    case 1:
      i2cAddr = 0x49;
      break;
    default:
      return false;
    }
    Wire.begin();
    return true;
  }

  bool readTemperature(float *temp) {
    return getTemp(REG_TEMP, temp);
  }

  bool setResolution(int bits) {
    byte cfg;

    if (!getCfg(&cfg)) {
      return false;
    }
    cfg &= ~(0x60);
    switch (bits) {
    case 9:
      break;
    case 10:
      cfg |= 0x20;
      break;
    case 11:
      cfg |= 0x40;
      break;
    case 12:
      cfg |= 0x60;
      break;
    default:
      return false;
    }
    return setCfg(cfg);
  }

  bool getCfg(byte *cfg) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(REG_CFG);
    if (Wire.endTransmission() != 0) {
      return false;
    }
    if (size != 1) {
      return false;
    }
    if (Wire.requestFrom(i2cAddr, 1) != 1) {
      return false;
    }
    *cfg = Wire.read();
    return true;
  }

  bool setCfg(byte cfg) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(REG_CFG);
    if (size == 1) {
      Wire.write(cfg);
    }
    if (Wire.endTransmission() != 0) {
      return false;
    }
    if (size != 1) {
      return false;
    }
    return true;
  }

  bool getTLow(float *temp) {
    return getTemp(REG_TLOW, temp);
  }

  bool setTLow(float temp) {
    return setTemp(REG_TLOW, temp);
  }

  bool getTHigh(float *temp) {
    return getTemp(REG_THIGH, temp);
  }

  bool setTHigh(float temp) {
    return setTemp(REG_THIGH, temp);
  }

private:
  bool getTemp(byte reg, float *temp) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(reg);
    if (Wire.endTransmission() != 0) {
      return false;
    }
    if (size != 1) {
      return false;
    }
    if (Wire.requestFrom(i2cAddr, 2) != 2) {
      return false;
    }
    signed char msb = Wire.read();
    byte lsb = Wire.read();
    *temp = msb + lsb / 256.0;
    return true;
  }

  bool setTemp(byte reg, float val) {
    byte msb, lsb;

    if (val >= 0) {
      msb = (byte)val;
      lsb = (byte)((val - msb) * 256);
    }
    else {
      msb = (byte)(256 + val);
      lsb = (byte)((256 + val - msb) * 256);
    }
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(reg);
    if (size == 1) {
      Wire.write(msb);
    }
    if (size == 1) {
      Wire.write(lsb);
    }
    if (Wire.endTransmission() != 0) {
      return false;
    }
    if (size != 1) {
      return false;
    }
    return true;
  }

  static const byte REG_TEMP = 0x00;
  static const byte REG_CFG = 0x01;
  static const byte REG_TLOW = 0x02;
  static const byte REG_THIGH = 0x03;
  int i2cAddr;
};

TMP105 tmp105;

void setup() {
  tmp105.begin();
  tmp105.setResolution(12);
  Serial.begin(115200);
}

void loop() {
  float temp;

  if (tmp105.readTemperature(&temp)) {
    Serial.println("Temperature: " + String(temp) + " °C");
  }
  else {
    Serial.println("Cannot read temperature!");
  }
  delay(1000);
}
