#include <Wire.h>

class HIH6000 {
public:
  bool begin(int addr = 0x27) {
    i2cAddr = addr;
    Wire.begin();
    return true;
  }

  bool measure() {
    Wire.beginTransmission(i2cAddr);
    int size = Wire.write((uint8_t *)NULL, 0);
    if (Wire.endTransmission() != 0) {
      return false;
    }
    if (size != 0) {
      return false;
    }
    delay(37);  /* 36.65 nominal measurement cycle duration */
    for (int i = 0; i < 10; i++) {
      if (Wire.requestFrom(i2cAddr, 4) != 4) {
        return false;
      }
      int hum = Wire.read();
      if ((hum & STATUS_BITMASK) == STATUS_NORMAL) {
        hum = (hum << 8) | Wire.read();
        this->hum = hum * 100.0 / ((1 << 14) - 2);
        int temp = Wire.read();
        temp = (temp << 6) | (Wire.read() >> 2);
        this->temp = temp * 165.0 / ((1 << 14) - 2) - 40;
        return true;
      }
    }
    return false;
  }

  float readHumidity() {
    return hum;
  }

  float readTemperature() {
    return temp;
  }

  bool measureHumidity(float *hum) {
    if (!measure()) {
      return false;
    }
    *hum = this->hum;
    return true;
  }

  bool measureTemperature(float *temp) {
    if (!measure()) {
      return false;
    }
    *temp = this->temp;
    return true;
  }

private:
  static const byte STATUS_NORMAL = 0x00;
  static const byte STATUS_STALE = 0x40;
  static const byte STATUS_CMD = 0x80;
  static const byte STATUS_BITMASK = 0xC0;
  int i2cAddr;
  float hum, temp;
};

HIH6000 hih6000;

void setup() {
  hih6000.begin();
  Serial.begin(115200);
}

void loop() {
  float hum, temp;

  if (hih6000.measure()) {
    Serial.print("Humidity: " + String(hih6000.readHumidity()) + "%");
    Serial.println(", temperature: " + String(hih6000.readTemperature()) + " °C");
  }
  else {
    Serial.println("Cannot measure temperature/humidity!");
  }
  delay(1000);
}
