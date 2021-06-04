#include <Wire.h>

class LIS3DH {
public:

  bool begin(int sa0 = 1) {
    if (!sa0) {
      i2cAddr = 0x18;
    }
    else {
      i2cAddr = 0x19;
    }
    Wire.begin();
    return true;
  }

  bool start() {
    uint8_t status;

    if (!readReg(status_reg, &status)) {
      return false;
    }
    if (status & STATUS_ZYXDA) {
      uint8_t buf[out_z_h - out_x_l + 1];

      /* Discard previously measured data. */
      if (!readRegs(out_x_l, buf, sizeof(buf))) {
        return false;
      }
    }
    if (!writeReg(ctrl_reg4, CTRL_BDU | FS_16G | CTRL_HR)) {
      return false;
    }
    return writeReg(ctrl_reg1, ODR_10HZ | CTRL_XEN | CTRL_YEN | CTRL_ZEN);
  }

  bool stop() {
    return writeReg(ctrl_reg1, ODR_POWERDOWN);
  }

  bool getAccel(float *x, float *y, float *z) {
    byte buf[out_z_h - out_x_l + 1];
    uint32_t startTime = millis();

    while (true) {
      if (!readReg(status_reg, buf)) {
        return false;
      }
      if (buf[0] & STATUS_ZYXDA) {
        break;
      }
      if (millis() - startTime > 1000) {
        return false;
      }
    }
    if (!readRegs(out_x_l, buf, sizeof(buf))) {
      return false;
    }
    *x = accelValue(buf[0], buf[1]);
    *y = accelValue(buf[2], buf[3]);
    *z = accelValue(buf[4], buf[5]);
    return true;
  }

  float accelValue(byte lsb, byte msb) {
    float val = (signed char)msb * 0x10 + (lsb >> 4);

    return (val * 0.012F);
  }

  bool readReg(byte addr, byte *val) {
    return readRegs(addr, val, 1);
  }

  bool writeReg(byte addr, byte val) {
    return writeRegs(addr, &val, 1);
  }

  bool readRegs(byte addr, byte *buf, unsigned int len) {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write(addr | REG_ADDR_INCR);
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
    int size = Wire.write(addr | REG_ADDR_INCR);
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
  int i2cAddr;

  enum reg_addr {
    status_reg_aux = 0x07,
    out_adc1_l,
    out_adc1_h,
    out_adc2_l,
    out_adc2_h,
    out_adc3_l,
    out_adc3_h,
    who_am_i = 0x0F,
    ctrl_reg0 = 0x1E,
    temp_cfg_reg,
    ctrl_reg1,
    ctrl_reg2,
    ctrl_reg3,
    ctrl_reg4,
    ctrl_reg5,
    ctrl_reg6,
    reference,
    status_reg,
    out_x_l,
    out_x_h,
    out_y_l,
    out_y_h,
    out_z_l,
    out_z_h,
    fifo_ctrl_reg,
    fifo_src_reg,
    int1_cfg,
    int1_src,
    int1_ths,
    int1_duration,
    int2_cfg,
    int2_src,
    int2_ths,
    int2_duration,
    click_cfg,
    click_src,
    click_ths,
    time_limit,
    time_latency,
    time_window,
    act_ths,
    act_dur,
  };

  static const int REG_ADDR_INCR = 1 << 7;

  static const int ODR_POWERDOWN = 0x00;
  static const int ODR_1HZ = 0x10;
  static const int ODR_10HZ = 0x20;
  static const int ODR_25HZ = 0x30;
  static const int ODR_50HZ = 0x40;
  static const int ODR_100HZ = 0x50;
  static const int ODR_200HZ = 0x60;
  static const int ODR_400HZ = 0x70;
  static const int ODR_1600HZ = 0x80;
  static const int ODR_5376HZ = 0x90;
  static const int CTRL_LPEN = 1 << 3;
  static const int CTRL_ZEN = 1 << 2;
  static const int CTRL_YEN = 1 << 1;
  static const int CTRL_XEN = 1 << 0;

  static const int CTRL_BDU = 1 << 7;
  static const int FS_2G = 0x00;
  static const int FS_4G = 0x10;
  static const int FS_8G = 0x20;
  static const int FS_16G = 0x30;
  static const int CTRL_BLE = 1 << 6;
  static const int CTRL_HR = 1 << 3;

  static const int STATUS_ZYXOR = 1 << 7;
  static const int STATUS_ZOR = 1 << 6;
  static const int STATUS_YOR = 1 << 5;
  static const int STATUS_XOR = 1 << 4;
  static const int STATUS_ZYXDA = 1 << 3;
  static const int STATUS_ZDA = 1 << 2;
  static const int STATUS_YDA = 1 << 1;
  static const int STATUS_XDA = 1 << 0;

  static const int FULL_RES = 1 << 3; /* reg_data_format */
  static const int RANGE_2G = 0;      /* reg_data_format */
  static const int RANGE_4G = 1;      /* reg_data_format */
  static const int RANGE_8G = 2;      /* reg_data_format */
  static const int RANGE_16G = 3;     /* reg_data_format */

  static const int POWER_CTL_MEASURE = 1 << 3;
};

LIS3DH lis3dh;

void setup() {
  Serial.begin(115200);
  lis3dh.begin();
  lis3dh.start();
}

void loop() {
  float accX, accY, accZ;

  if (lis3dh.getAccel(&accX, &accY, &accZ)) {
    Serial.print("Acceleration: x ");
    Serial.print(accX);
    Serial.print("g, y ");
    Serial.print(accY);
    Serial.print("g, z ");
    Serial.print(accZ);
    Serial.println("g");
  }
  else {
    Serial.println("Cannot measure acceleration");
  }
  delay(1000);
}
