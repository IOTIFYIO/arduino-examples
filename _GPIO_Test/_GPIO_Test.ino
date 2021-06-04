
void setup() {

  Serial.begin(9600);
  Serial.println("Initializing Digital Pins 0-13 as Output");

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}

void loop() {
  Serial.println("Setting D0 to High");
  digitalWrite(0, HIGH);
  delay(500);
  Serial.println("Setting D0 to Low");
  digitalWrite(0, LOW);
  delay(500);

  Serial.println("Setting D1 to High");
  digitalWrite(1, HIGH);
  delay(500);
  Serial.println("Setting D1 to Low");
  digitalWrite(1, LOW);
  delay(500);

  Serial.println("Setting D2 to High");
  digitalWrite(2, HIGH);
  delay(500);
  Serial.println("Setting D2 to Low");
  digitalWrite(2, LOW);
  delay(500);

  Serial.println("Setting D3 to High");
  digitalWrite(3, HIGH);
  delay(500);
  Serial.println("Setting D3 to Low");
  digitalWrite(3, LOW);
  delay(500);

  Serial.println("Setting D4 to High");
  digitalWrite(4, HIGH);
  delay(500);
  Serial.println("Setting D4 to Low");
  digitalWrite(4, LOW);
  delay(500);

  Serial.println("Setting D5 to High");
  digitalWrite(5, HIGH);
  delay(500);
  Serial.println("Setting D5 to Low");
  digitalWrite(5, LOW);
  delay(500);

  Serial.println("Setting D6 to High");
  digitalWrite(6, HIGH);
  delay(500);
  Serial.println("Setting D6 to Low");
  digitalWrite(6, LOW);
  delay(500);


  Serial.println("Setting D7 to High");
  digitalWrite(7, HIGH);
  delay(500);
  Serial.println("Setting D7 to Low");
  digitalWrite(7, LOW);
  delay(500);

  Serial.println("Setting D8 to High");
  digitalWrite(8, HIGH);
  delay(500);
  Serial.println("Setting D8 to Low");
  digitalWrite(8, LOW);
  delay(500);

  Serial.println("Setting D9 to High");
  digitalWrite(9, HIGH);
  delay(500);
  Serial.println("Setting D9 to Low");
  digitalWrite(9, LOW);
  delay(500);

  Serial.println("Setting D10 to High");
  digitalWrite(10, HIGH);
  delay(500);
  Serial.println("Setting D10 to Low");
  digitalWrite(10, LOW);
  delay(500);

  Serial.println("Setting D11 to High");
  digitalWrite(11, HIGH);
  delay(500);
  Serial.println("Setting D11 to Low");
  digitalWrite(11, LOW);
  delay(500);

  Serial.println("Setting D12 to High");
  digitalWrite(12, HIGH);
  delay(500);
  Serial.println("Setting D12 to Low");
  digitalWrite(12, LOW);
  delay(500);

  Serial.println("Setting D13 to High");
  digitalWrite(13, HIGH);
  delay(500);
  Serial.println("Setting D13 to Low");
  digitalWrite(13, LOW);
  delay(500);

}
