
bool i0 = false;
bool i1 = false;
bool i2 = false;

int current = 0;
int previous = 0;

void setup() {

  for (int i = 0; i < 8; ++i) {
    pinMode(i, OUTPUT);
  }

  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  Serial.begin(9600);
  Serial.println("Initialized Serial Communication");

  Serial.println("Convert Binary to Decimal");
  
  Serial.println("Use the switches to input a 3-bit binary value ");

  Serial.println("NOTE: The pins are mapped as i2, i1, i0 => 11, 12, 13");
  
  Serial.println("      ie. Pin 11 is the MSB");
}

void clearDIO() {
  for (int i = 0; i < 8; ++i) {
    digitalWrite(i, LOW);
  }
}

int readBinaryToDecimal() {

  int sum = 0;

  if (digitalRead(13) == HIGH)
    sum += 1;
  if (digitalRead(12) == HIGH)
    sum += 2;
  if (digitalRead(11) == HIGH)
    sum += 4;

  return sum;


}

void loop() {

  current = readBinaryToDecimal();

  if (current != previous) {

    Serial.print("Decimal Equivalent : ");
    Serial.println(current);

    clearDIO();

    digitalWrite(current, HIGH);

    previous = current;

  }



}
