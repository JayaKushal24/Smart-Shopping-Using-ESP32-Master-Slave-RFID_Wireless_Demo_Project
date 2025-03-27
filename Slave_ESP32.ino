#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "moto";
const char* password = "123456789";
const char* serverAddress = "http://192.168.225.161/";  // Replace with Master's IP

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16x2 LCD

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Cart Ready");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverAddress);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      parseAndDisplay(payload);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(5000);  // Fetch data every 5 seconds
}

void parseAndDisplay(String data) {
  int totalIndex = data.indexOf("<tr><td>") + 8;
  int totalEndIndex = data.indexOf("</td>", totalIndex);
  int totalItems = data.substring(totalIndex, totalEndIndex).toInt();

  int priceIndex = data.indexOf("<td>", totalEndIndex + 5) + 4;
  int priceEndIndex = data.indexOf(" INR</td>", priceIndex);
  String totalPrice = data.substring(priceIndex, priceEndIndex);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Items: " + String(totalItems));

  lcd.setCursor(0, 1);
  lcd.print("Total: " + totalPrice + " INR");

  Serial.println("Updated LCD Display");
}
