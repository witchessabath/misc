#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <WiFiNINA.h>
#include <Arduino_JSON.h>
#include <ArduinoHttpClient.h>

hd44780_I2Cexp lcd(0x27, 20, 4);

const char* ssid = "xx";
const char* password = "xx";

String openWeatherMapApiKey = "xx";
String city = "xx";
String countryCode = "xx";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.begin(20, 4);
  lcd.backlight();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println("HTTP Response: " + jsonBuffer);

      int jsonStartIndex = jsonBuffer.indexOf("{");
      if (jsonStartIndex == -1) {
        Serial.println("No JSON data found!");
        return;
      }

      String jsonResponse = jsonBuffer.substring(jsonStartIndex);
      Serial.println("JSON Body: " + jsonResponse);
      
      JSONVar myObject = JSON.parse(jsonResponse);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      double temperatureK = myObject["main"]["temp"];
      double temperatureC = (temperatureK - 273.15);
      int humidity = myObject["main"]["humidity"];

      Serial.print("Temperature: ");
      Serial.println(temperatureC);
      Serial.print("Humidity: ");
      Serial.println(humidity);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperatureC);
      lcd.print(" ");
      lcd.print((char)223);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.print(" %");
    } else {
      Serial.println("WiFi Disconnected");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WiFi Disconnected");
    }
    lastTime = millis();
    delay(2000);  // Wait for 2 seconds before the next update
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  String serverPath = String("GET ") + serverName + " HTTP/1.1\r\nHost: api.openweathermap.org\r\nConnection: close\r\n\r\n";

  if (client.connect("api.openweathermap.org", 80)) {
    client.print(serverPath);
  } else {
    Serial.println("Connection failed");
    return "{}";
  }

  String payload = "";
  bool httpBody = false;

  while (client.connected() || client.available()) {
    if (client.available()) {
      char c = client.read();
      if (httpBody) {
        payload += c;
      } else if (c == '\n' && payload.endsWith("\r\n")) {
        httpBody = true;
        payload = "";  // Clear the payload to start fresh with the JSON body
      } else {
        payload += c;
      }
    }
  }

  client.stop();
  return payload;
}
