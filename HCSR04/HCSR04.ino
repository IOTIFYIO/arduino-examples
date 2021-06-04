#define trigPin 5
#define echoPin 6

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("HC-SR04 Test");
}
void loop() {
  digitalWrite(trigPin, LOW);
  delay(2000);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 17150;

  Serial.print("Duration: ");
  Serial.println(duration);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
