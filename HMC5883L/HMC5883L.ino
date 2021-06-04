#include <Wire.h>

class HMC5883L {
public:

  bool begin() {
    Wire.begin();
    return true;
  }

  bool start() {
    uint8_t status;

    if (!readReg(sr, &status)) {
      return false;
    }
    if (status & SR_RDY) {
      uint8_t buf[dyrb - dxra + 1];

      /* Discard previously measured data. */
      if (!readRegs(dxra, buf, sizeof(buf))) {
        return false;
      }
    }
    if (!writeReg(cra, CRA_MA_1 | CRA_DO_1_5HZ | CRA_MS_NORMAL) ||
        !writeReg(crb, CRB_GN_7)) {
      return false;
    }
    return writeReg(mr, MR_MD_CONTINUOUS);
  }

  bool stop() {
    return writeReg(mr, MR_MD_IDLE);
  }

  bool getMag(float *x, float *y, float *z) {
    byte buf[dyrb - dxra + 1];
    uint32_t startTime = millis();

    while (true) {
      if (!readReg(sr, buf)) {
        return false;
      }
      if (buf[0] & SR_RDY) {
        break;
      }
      if (millis() - startTime > 1000) {
        return false;
      }
    }
    if (!readRegs(dxra, buf, sizeof(buf))) {
      return false;
    }
    *x = accelValue(buf[0], buf[1]);
    *y = accelValue(buf[4], buf[5]);
    *z = accelValue(buf[2], buf[3]);
    return true;
  }

  float accelValue(byte msb, byte lsb) {
    float val = (signed char)msb * 0x100 + lsb;

    if (val == -4096) {
      return NAN;
    }
    return (val / 230);
  }

  bool readReg(byte addr, byte *val) {
    return readRegs(addr, val, 1);
  }

  bool writeReg(byte addr, byte val) {
    return writeRegs(addr, &val, 1);
  }

  bool readRegs(byte addr, byte *buf, unsigned int len) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(addr);
    if ((Wire.endTransmission() != 0) || (size != 1)) {
      return false;
    }
    if (Wire.requestFrom(i2cAddr, len) != len) {
      return false;
    }
    for (int i = 0; i < len; i++) {
      buf[i] = Wire.read();
    }
    return true;
  }

  bool writeRegs(byte addr, byte *buf, unsigned int len) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(addr);
    if (size != 1) {
      return false;
    }
    size = Wire.write(buf, len);
    if ((Wire.endTransmission() != 0) || (size != len)) {
      return false;
    }
    return true;
  }

private:
  static const int i2cAddr = 0x1E;

  enum reg_addr {
    cra = 0x00,
    crb,
    mr,
    dxra,
    dxrb,
    dzra,
    dzrb,
    dyra,
    dyrb,
    sr,
    ira,
    irb,
    irc,
  };

  static const int CRA_MA_1 = 0x00;
  static const int CRA_MA_2 = 0x20;
  static const int CRA_MA_4 = 0x40;
  static const int CRA_MA_8 = 0x60;
  static const int CRA_DO_0_75HZ = 0x00;
  static const int CRA_DO_1_5HZ = 0x04;
  static const int CRA_DO_3HZ = 0x08;
  static const int CRA_DO_7_5HZ = 0x0C;
  static const int CRA_DO_15HZ = 0x10;
  static const int CRA_DO_30HZ = 0x14;
  static const int CRA_DO_75HZ = 0x18;
  static const int CRA_MS_NORMAL = 0x00;
  static const int CRA_MS_PBIAS = 0x01;
  static const int CRA_MS_NBIAS = 0x02;

  static const int CRB_GN_0 = 0x00;
  static const int CRB_GN_1 = 0x20;
  static const int CRB_GN_2 = 0x40;
  static const int CRB_GN_3 = 0x60;
  static const int CRB_GN_4 = 0x80;
  static const int CRB_GN_5 = 0xA0;
  static const int CRB_GN_6 = 0xC0;
  static const int CRB_GN_7 = 0xE0;

  static const int MR_MD_CONTINUOUS = 0x00;
  static const int MR_MD_SINGLE = 0x01;
  static const int MR_MD_IDLE = 0x02;

  static const int SR_LOCK = 1 << 1;
  static const int SR_RDY = 1 << 0;
};

HMC5883L hmc5883l;

void setup() {
  Serial.begin(115200);
  hmc5883l.begin();
  hmc5883l.start();
}

void loop() {
  float magX, magY, magZ;

  if (hmc5883l.getMag(&magX, &magY, &magZ)) {
    Serial.print("Magnetic field (Gauss units): x ");
    Serial.print(magX);
    Serial.print(", y ");
    Serial.print(magY);
    Serial.print(", z ");
    Serial.println(magZ);
  }
  else {
    Serial.println("Cannot measure magnetic field");
  }
  delay(1000);
}
