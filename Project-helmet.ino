#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>

const char* ssid = "******";  // Replace with your Wi-Fi network name
const char* password = "******";  // Replace with your Wi-Fi password

const int gasSensorPin = A0;  // Pin connected to the gas sensor analog output
const int irPin = D1;  // Pin connected to the IR sensor output

const String thingspeakURL = "http://api.thingspeak.com/update";
const String apiKey = "24JSU0QGTUS2Y5WK";  // Replace with your ThingSpeak API key
const String channelID = "2215783";  // Replace with your ThingSpeak channel ID

WiFiClient client;  // Create a WiFiClient object

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int gasSensorValue = analogRead(gasSensorPin);
  int irSensorValue = digitalRead(irPin);
  if (irSensorValue == 1) {
    Serial.println("Helmet not detected");
  } else {
    Serial.println("Helmet detected");
    Serial.print("IR Sensor Value: ");
    Serial.println(irSensorValue);
    Serial.print("Gas Sensor Value: ");
    Serial.println(gasSensorValue);
    Serial.print("Ready to go!!");
  }
  if (sendDataToThingSpeak(irSensorValue, gasSensorValue)) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }
  delay(1000);  // Adjust delay as per your requirement
}
bool sendDataToThingSpeak(int irValue, int gasValue) {
  String data = "api_key=" + apiKey + "&field1=" + String(irValue) + "&field2=" + String(gasValue);

  HTTPClient http;
  http.begin(client, thingspeakURL);  // Specify the URL
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(data);  // Send the POST request
  http.end();  // Close the connection
  return (httpResponseCode == HTTP_CODE_OK);
}
