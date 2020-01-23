#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char host[] = "http://shaun-temp-mon.azurewebsites.net/";
char functionKey[] = "5DluXPfgBx6S2O1/zNgV/VMXu1BAgk2asWMqXYb4Sp/628XOQyBJoA==";

const char* ssid = "Area 51 Surveillance Van";
const char* password = "monkeyslut";

#define SERIAL_BAUD 115200 //ESP rate

String strArr[3];

String insideTemperature = "";
String outsideTemperature = "";
String humidity = "";

char c;
char start = '<';
char End = '>';
char delimiter = ',';
String data;

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
  while (Serial.available() > 0) {
    c = Serial.read();
    if (c == delimiter or c == End) {
      Parse(data);
      data = "";
    }
    else if (c != start) {
      data += c;
    }

    if (c == End) {
      insideTemperature = strArr[0];
      outsideTemperature = strArr[1];
      humidity = strArr[2];
    
      int response = httpCall(insideTemperature, outsideTemperature, humidity);
    }
  }
}

void Parse(String charString) {
  if (strArr[0].length() == 0) {
    strArr[0] = charString;
  }
  else if (strArr[1].length() == 0) {
    strArr[1] = charString;
  }

  else if (strArr[2].length() == 0) {
    strArr[2] = charString;
  }
}

int httpCall(String insideTemperature, String outsideTemperature, String humidity) {
  if(WiFi.status() == WL_CONNECTED) {   
    int returnCode = 0;
  
    float pressure = 1000.0;
    String json = "{\"tempInside\":" + insideTemperature + ",\"tempOutside\":" + outsideTemperature + ",\"humidity\":" + humidity + "}";
    Serial.println(json);
    String response = httpPost("api/temperature", json);

    if (response == "") {
      returnCode = 200;
      strArr[0] = "";
      strArr[1] = "";
      strArr[2] = "";
    }
    else {
      returnCode = 500;
    }
    
    return returnCode;
  } 
}

String httpPost(String url, String data) {
  WiFiClient client;
  HTTPClient http;
  int count = 0;
  int maxRetryCount = 20;

  Serial.println("Connecting...");
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
    http.end();
    return "fail";  
  }
}
