# Smart Shopping Cart Using ESP32 & RFID  

## Overview  
This project implements a **smart shopping cart system** using **ESP32, RFID, and an I2C LCD display**. The system consists of:  
- A **master ESP32** that scans RFID tags and hosts a web server displaying cart details.  
- A **slave ESP32** that connects to the master ESP32's web server and displays the total bill and item count on an **I2C LCD screen**.  

## Features  
✅ **RFID-Based Item Detection** – Adds/removes items by scanning RFID tags.  
✅ **Web Server for Cart Overview** – Displays added items, quantity, and total cost.  
✅ **I2C LCD Integration** – Displays real-time total items and price on a second ESP32.  
✅ **WiFi Connectivity** – Syncs data between master and slave ESP32 devices.  
✅ **Buzzer & LED Indicators** – Provides feedback for item addition/removal.  

## Components Required  
| Component | Quantity |
|-----------|---------|
| ESP32 Dev Kit | 2 |
| RFID Module (MFRC522) | 1 |
| RFID Tags | Multiple |
| I2C LCD Display (16x2) | 1 |
| Buzzer | 1 |
| LEDs (Red & Green) | 1 each |
| Push Button | 1 |
| Resistors & Wires | As needed |

## Circuit Connections  

### Master ESP32 (RFID Scanner & Web Server)  
| Component | ESP32 Pin |
|-----------|----------|
| RFID SS (SDA) | GPIO 5 |
| RFID RST | GPIO 2 |
| Buzzer | GPIO 4 |
| Push Button | GPIO 13 |
| Green LED | GPIO 14 |
| Red LED | GPIO 12 |

### Slave ESP32 (LCD Display)  
| Component | ESP32 Pin |
|-----------|----------|
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |

## How It Works  
1. **Master ESP32** reads RFID tags and updates the shopping cart.  
2. The web server on the **master ESP32** displays the cart’s total items and price.  
3. **Slave ESP32** fetches this data and updates the **I2C LCD display** every 5 seconds.  

## Installation & Setup  

### Master ESP32  
1. Connect the **RFID module, buzzer, LEDs, and push button** to the master ESP32.  
2. Upload the `master_esp32.ino` sketch to your ESP32.  
3. Check the **Serial Monitor** for the assigned IP address.  

### Slave ESP32  
1. Connect the **I2C LCD** to the slave ESP32.  
2. Update the `serverAddress` variable in `slave_esp32.ino` with the master ESP32's IP.  
3. Upload the `slave_esp32.ino` sketch to the slave ESP32.  

## Dependencies  
- **ESP32 WiFi Library** (`WiFi.h`)  
- **Web Server Library** (`WebServer.h`)  
- **MFRC522 RFID Library** (`MFRC522.h`)  
- **I2C LCD Library** (`LiquidCrystal_I2C.h`)  
- **SPI Communication Library** (`SPI.h`)  

## Usage  
1. Power on both **master and slave ESP32**.  
2. Scan an RFID tag to add an item.  
3. Press the push button while scanning to **remove an item**.  
4. View the cart details on the **master’s web interface**.  
5. The **slave’s LCD display** updates automatically.  

## Demo  
![Smart Cart Web Interface](https://your-image-url.com)  
![ESP32 LCD Display](https://your-image-url.com)  

## License  
This project is open-source under the **MIT License**.  

---
