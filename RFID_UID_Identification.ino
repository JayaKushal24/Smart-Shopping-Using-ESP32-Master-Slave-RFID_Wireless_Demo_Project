#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5        // SDA connected to GPIO5
#define RST_PIN 22      // RST connected to GPIO22
#define MISO_PIN 19     // MISO connected to GPIO19
#define MOSI_PIN 23     // MOSI connected to GPIO23
#define SCK_PIN 18      // SCK connected to GPIO18

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);  // Initialize SPI bus with custom pins
  rfid.PCD_Init();

  Serial.println("Scan an RFID Tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID Tag: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA();
}
