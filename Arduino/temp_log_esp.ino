#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char host[] = "http://shaun-temp-mon.azurewebsites.net/";
char functionKey[] = "5DluXPfgBx6S2O1/zNgV/VMXu1BAgk2asWMqXYb4Sp/628XOQyBJoA==";
//char fingerprint[] = "39 8E 01 A5 0C 66 8A 74 F0 10 4A 83 60 15 A2 6E 21 55 4C CE";

const char* ssid = "Area 51 Surveillance Van";
const char* password = "monkeyslut";

#define SERIAL_BAUD 115200 //ESP rate

String temperature = "";
bool dataIn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);

  Serial.println(); 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    char c = Serial.read();

    //the start of the temperature reading
    if (c == '[') { 
      temperature = "";
      dataIn = true;
    }
    //if not at the end of the line, append to temperature string
    else if (dataIn && c != ']') {
      temperature += c;
    }

    else if (dataIn && c == ']') {
      //we have reached the end of the stream, post the data to my azure site

      int response = httpCall(temperature);
      if (response == 200) {
        Serial.write("Y");
      }
      
      delay(1000);
    }
    delay(10);
  }

}

int httpCall(String temperature) {
  if(WiFi.status() == WL_CONNECTED) {   
    int returnCode = 0;
    Serial.println("Doing HTTP Call....");
  
    float pressure = 1000.0;
    String json = "{\"tempInside\":" + temperature + ",\"tempOutside\":" + temperature + ",\"pressure\":" + pressure + "}";
    Serial.println(json);
    String response = httpsPost("api/temperature", json);
    
    if (response == "") {
      returnCode = 200;
    }
    else {
      returnCode = 500;
    }
    
    return returnCode;
  }
  else {
    Serial.println("Please reset me!");
  }
 
}

String httpsPost(String url, String data) {
  WiFiClient client;
  HTTPClient http;
  int count = 0;
  int maxRetryCount = 20;
  
  Serial.print("Connecting");
  http.begin(client, host + url); //HTTP
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-functions-key", functionKey);
  int httpCode = http.POST(data);
  Serial.println(httpCode);
  if (httpCode > 0) {
    http.end();
    return "";
  }
  else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return "fail";  
  }
}
