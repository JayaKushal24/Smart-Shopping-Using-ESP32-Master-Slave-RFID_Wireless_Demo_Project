#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5   // SDA (SS) connected to GPIO 5
#define RST_PIN 22 // RST connected to GPIO 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200);  // Use higher baud rate for ESP32
  SPI.begin(18, 19, 23); // SCK = 18, MISO = 19, MOSI = 23
  mfrc522.PCD_Init();    // Initialize MFRC522

  Serial.println("Approximate your card to the reader...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();

  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "92 E5 F6 03") { // Change this to match your card's UID
    Serial.println("Authorized access");
  } else {
    Serial.println("Access denied");
  }
  delay(3000);
}
