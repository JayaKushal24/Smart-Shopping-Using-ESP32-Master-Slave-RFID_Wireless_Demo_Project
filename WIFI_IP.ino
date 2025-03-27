#include <WiFi.h>

const char *ssid = "YJK";
const char *password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println("Starting...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
