#define SERIAL_BAUD 115200 //the rate for the ESP chip

const int tempPin = A0;
const int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(SERIAL_BAUD);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(tempPin);
  float voltage = (sensorVal / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100;
  
   //fake do stuff
  Serial.print("[");
  Serial.print(temperature);
  Serial.println("]");
  delay(58000);

  if (Serial.available()) {
    char c = Serial.read();

    if (c == 'Y') {
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(ledPin, LOW);
    }
  }
}
