#include <Stepper.h>

#if defined(ARDUINO_NUCLEO_F411RE)

#define L298_PIN_ENA  41 // PB_1
#define L298_PIN_ENB  40 // PB_2
#define L298_PIN_IN1  7 // PA_8
#define L298_PIN_IN2  8 // PA_9
#define L298_PIN_IN3  2 // PA_10
#define L298_PIN_IN4  37 // PA_11

#else // Arduino Uno

#define L298_PIN_ENA  1 // PD1
#define L298_PIN_ENB  2 // PD2
#define L298_PIN_IN1  14 // PC0
#define L298_PIN_IN2  15 // PC1
#define L298_PIN_IN3  16 // PC2
#define L298_PIN_IN4  17 // PC3

#endif

const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution,
  L298_PIN_IN1, L298_PIN_IN2, L298_PIN_IN3, L298_PIN_IN4);

int stepCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(L298_PIN_ENA, OUTPUT);
  digitalWrite(L298_PIN_ENA, HIGH);
  pinMode(L298_PIN_ENB, OUTPUT);
  digitalWrite(L298_PIN_ENB, HIGH);
}

void loop() {
  myStepper.step(1);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(500);
}
