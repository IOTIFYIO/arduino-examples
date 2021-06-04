#include <time.h>
#include <Wire.h>

static const char *daysOfWeek[] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

static unsigned int from_bcd8(uint8_t bcd)
{
  return (bcd & 0x0F) + 10 * (bcd >> 4);
}

static uint8_t to_bcd8(unsigned int val)
{
  uint8_t bcd = val % 10;

  val -= bcd;
  bcd |= (val / 10) << 4;
  return bcd;
}

class PCF8523 {
public:

  bool begin() {
    Wire.begin();
    return true;
  }

  bool getDatetime(struct tm *datetime) {
    byte buf[reg_years - reg_control_1 + 1];

    if (!readRegs(reg_control_1, buf, sizeof(buf))) {
      return false;
    }
    datetime->tm_sec = from_bcd8(buf[reg_seconds] & 0x7F);
    datetime->tm_min = from_bcd8(buf[reg_minutes]);
    if (buf[reg_control_1] & HOUR12) {
      datetime->tm_hour = from_bcd8(buf[reg_hours] & HOUR12_BITMASK);
      if (buf[reg_hours] & PM) {
        if (datetime->tm_hour < 12) {
          datetime->tm_hour += 12;
        }
      }
      else {
        if (datetime->tm_hour == 12) {
          datetime->tm_hour = 0;
        }
      }
    }
    else {
      datetime->tm_hour = from_bcd8(buf[reg_hours] & HOUR24_BITMASK);
    }
    datetime->tm_mday = from_bcd8(buf[reg_days]);
    datetime->tm_wday = from_bcd8(buf[reg_weekdays]);
    datetime->tm_mon = from_bcd8(buf[reg_months]) - 1;
    datetime->tm_year = from_bcd8(buf[reg_years]) + 100;
    return true;
  }

  bool setDatetime(struct tm *datetime) {
    byte ctrl1;
    byte buf[reg_years - reg_seconds + 1];

    if (!readReg(reg_control_1, &ctrl1)) {
      return false;
    }
    if (ctrl1 & HOUR12) {
      ctrl1 &= ~HOUR12;
      if (!writeReg(reg_control_1, &ctrl1)) {
        return false;
      }
    }
    buf[0] = to_bcd8(datetime->tm_sec);
    buf[1] = to_bcd8(datetime->tm_min);
    buf[2] = to_bcd8(datetime->tm_hour);
    buf[3] = to_bcd8(datetime->tm_mday);
    buf[4] = to_bcd8(datetime->tm_wday);
    buf[5] = to_bcd8(datetime->tm_mon) + 1;
    buf[6] = to_bcd8(datetime->tm_year % 100);
    return writeRegs(reg_seconds, buf, sizeof(buf));
  }

  bool setDatetimeAMPM(struct tm *datetime) {
    byte ctrl1;
    byte buf[reg_years - reg_seconds + 1];

    if (!readReg(reg_control_1, &ctrl1)) {
      return false;
    }
    if (!(ctrl1 & HOUR12)) {
      ctrl1 |= HOUR12;
      if (!writeReg(reg_control_1, &ctrl1)) {
        return false;
      }
    }
    buf[0] = to_bcd8(datetime->tm_sec);
    buf[1] = to_bcd8(datetime->tm_min);
    if (datetime->tm_hour == 0) {
      buf[2] = to_bcd8(12);
    }
    else if (datetime->tm_hour < 12) {
      buf[2] = to_bcd8(datetime->tm_hour);
    }
    else if (datetime->tm_hour == 12) {
      buf[2] = PM | to_bcd8(12);
    }
    else {
      buf[2] = PM | to_bcd8(datetime->tm_hour - 12);
    }
    buf[3] = to_bcd8(datetime->tm_mday);
    buf[4] = to_bcd8(datetime->tm_wday);
    buf[5] = to_bcd8(datetime->tm_mon) + 1;
    buf[6] = to_bcd8(datetime->tm_year % 100);
    return writeRegs(reg_seconds, buf, sizeof(buf));
  }

  bool readReg(byte addr, byte *buf) {
    return readRegs(addr, buf, 1);
  }

  bool writeReg(byte addr, byte *buf) {
    return writeRegs(addr, buf, 1);
  }

  bool readRegs(byte addr, byte *buf, unsigned int len) {
    Wire.beginTransmission(I2C_ADDR);
    int size = Wire.write(addr);
    if ((Wire.endTransmission() != 0) || (size != 1)) {
      return false;
    }
    if (Wire.requestFrom(I2C_ADDR, len) != len) {
      return false;
    }
    for (int i = 0; i < len; i++) {
      buf[i] = Wire.read();
    }
    return true;
  }

  bool writeRegs(byte addr, byte *buf, unsigned int len) {
    Wire.beginTransmission(I2C_ADDR);
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
  static const int I2C_ADDR = 0x68;

  enum reg_addr {
    reg_control_1 = 0,
    reg_control_2,
    reg_control_3,
    reg_seconds,
    reg_minutes,
    reg_hours,
    reg_days,
    reg_weekdays,
    reg_months,
    reg_years,
    reg_minute_alarm,
    reg_hour_alarm,
    reg_day_alarm,
    reg_weekday_alarm,
    reg_offset,
    reg_tmr_clkout_ctrl,
    reg_tmr_a_freq_ctrl,
    reg_tmr_a_reg,
    reg_tmr_b_freq_ctrl,
    reg_tmr_b_reg,
    reg_count
  };

  static const int HOUR12 = 1 << 3; /* reg_control_1 */
  static const int PM = 1 << 5;           /* reg_hours */
  static const int HOUR12_BITMASK = 0x1F; /* reg_hours */
  static const int HOUR24_BITMASK = 0x3F; /* reg_hours */
};

PCF8523 pcf8523;

void setup() {
  Serial.begin(115200);
  pcf8523.begin();
}

void loop() {
  struct tm datetime;
  char buf[32];

  if (pcf8523.getDatetime(&datetime)) {
    strftime(buf, sizeof(buf), "%H:%M:%S - %d-%m-%Y ", &datetime);
    Serial.print(buf);
    Serial.println(daysOfWeek[datetime.tm_wday]);
  }
  else {
    Serial.println("Cannot get date/time");
  }
  delay(1000);
}
