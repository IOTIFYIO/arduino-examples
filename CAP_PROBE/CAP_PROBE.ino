#define probe 12

float resistance = 1000000;

float waterCapacitance = 0.000032;
float soilCapacitance = 0.0000016;

long startTime = 0;
long endTime = 0;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);

}

// the loop function runs over and over again forever
void loop() {

  Serial.println("Measuring Moisture Level...");
  pinMode(probe, OUTPUT);
  digitalWrite(probe, LOW);
  Serial.print("Probe Low ");
  startTime = millis();
  Serial.println(startTime);

  pinMode(probe, INPUT);
  while (!digitalRead(probe)) {
    Serial.print(".");
    continue;
  }
  Serial.println("");
  endTime = millis();
  Serial.println(endTime);

  long riseTime = endTime - startTime;
  Serial.print("Rise Time : ");
  Serial.println(riseTime);

  float capacitance = riseTime * 0.001 / resistance;
  Serial.print("Capacitance : ");
  Serial.println(capacitance);

  if (capacitance < soilCapacitance)
    capacitance = soilCapacitance;
  else if (capacitance > waterCapacitance)
    capacitance = waterCapacitance;

  float moisture = 100 * (capacitance - soilCapacitance) / (waterCapacitance - soilCapacitance);
  Serial.print("Moisture Level : ");
  Serial.println(moisture);


}
