#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define SERIAL_BAUD 115200 //the rate for the ESP chip
#define DHTPIN 9
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define WIREPIN 5
OneWire oneWire(WIREPIN);
// pass reference to onewire object instance
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(SERIAL_BAUD);
  dht.begin();
  sensors.begin();
}

void loop() {
  //index allows for multiple sensors on the same pin
  sensors.requestTemperatures();
  
  float outsideTemperature = sensors.getTempCByIndex(0); 
  float insideTemperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("<");
  Serial.print(insideTemperature);
  Serial.print(",");
  Serial.print(outsideTemperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.println(">");
  delay(60000);

}
