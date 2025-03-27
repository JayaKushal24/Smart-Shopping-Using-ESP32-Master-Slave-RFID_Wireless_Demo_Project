#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char *ssid = "moto";
const char *password = "123456789";

WebServer server(80);

#define SS_PIN 5
#define RST_PIN 2
#define BUZZER_PIN 4
#define PUSH_BUTTON 13
#define GREEN_LED 14
#define RED_LED 12

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

double total = 0;
int count_prod = 0;

struct Item {
  const char* name;
  double price;
  byte uid[4];
  int quantity;
  const char* color;
};

Item items[] = {
  {"Book1", 35.00, {0x92, 0xE5, 0xF6, 0x03}, 0, "#FFC0CB"},
  {"Book2", 24.00, {0xA9, 0xE0, 0xE0, 0x00}, 0, "#ADD8E6"}, 
  {"Book3", 10.00, {0x85, 0x37, 0xE1, 0x00}, 0, "#FFFF99"},
  {"Book4", 20.00, {0x9B, 0x33, 0xE1, 0x00}, 0, "#D3D3D3"}
};


void handleRoot() {
  String msg = "<html><head><meta http-equiv='refresh' content='4'/><meta name='viewport' content='width=device-width, initial-scale=1'><title>Smart Cart</title>";
  msg += "<style>body { font-family: Arial, sans-serif; background: #f8f9fa; color: #333; text-align: center; padding: 20px; }";
  msg += ".container { max-width: 800px; margin: auto; background: white; padding: 20px; box-shadow: 0px 4px 10px rgba(0,0,0,0.2); border-radius: 10px; }";
  msg += "h2 { color: #007BFF; } .table { width: 100%; margin-top: 20px; border-collapse: collapse; } .table th, .table td { padding: 10px; text-align: center; border-bottom: 1px solid #ddd; }";
  msg += "th { background: #007BFF; color: white; } .item-row:hover { background: #f1f1f1; }";
  msg += "</style></head><body><div class='container'><h2>Smart Shopping Cart</h2><h3>Items List</h3><table class='table'><tr><th>Item</th><th>Quantity</th><th>Price</th></tr>";
  
  for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
    if (items[i].quantity > 0) {
      msg += "<tr class='item-row' style='background:" + String(items[i].color) + "'><td>" + String(items[i].name) + "</td><td>" + String(items[i].quantity) + "</td><td>" + String(items[i].price) + " INR</td></tr>";
    }
  }
  
  msg += "</table><br><h3>Total Bill</h3><table class='table'><tr><th>Total Items</th><th>Total Price</th></tr>";
  msg += "<tr><td>" + String(count_prod) + "</td><td>" + String(total, 2) + " INR</td></tr></table></div></body></html>";
  server.send(200, "text/html", msg);
}

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
  
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Back!!");
  lcd.setCursor(0, 1);
  lcd.print("Please add Items");
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    bool buttonPressed = (digitalRead(PUSH_BUTTON) == LOW);
    for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
      if (compareUID(items[i].uid)) {
        if (buttonPressed) removeItem(i);
        else addItem(i);
        break;
      }
    }
    rfid.PICC_HaltA();
  }
}

bool compareUID(byte *cardUID) {
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != cardUID[i]) return false;
  }
  return true;
}

void addItem(int index) {
  items[index].quantity++;
  total += items[index].price;
  count_prod++;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(items[index].name);
  lcd.print(" ADDED!!");
  digitalWrite(GREEN_LED, HIGH);
  delay(500);
  digitalWrite(GREEN_LED, LOW);
  lcd.setCursor(0, 1);
  lcd.print("ITEM PRICE=");
  lcd.print(items[index].price);
  delay(1500);
  updateLCD();
}

void removeItem(int index) {
  if (items[index].quantity > 0) {
    items[index].quantity--;
    total -= items[index].price;
    count_prod--;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(items[index].name);
    lcd.print(" REMOVED");
    digitalWrite(RED_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("ITEM PRICE=");
    lcd.print(items[index].price);
    delay(1500);
    updateLCD();
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TOTAL:");
  lcd.setCursor(7, 0);
  lcd.print(total);
  lcd.setCursor(0, 1);
  lcd.print("Items Added:");
  lcd.setCursor(13, 1);
  lcd.print(count_prod);
}
