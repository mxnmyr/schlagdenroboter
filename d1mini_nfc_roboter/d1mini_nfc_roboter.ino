/*
 * NFC Reader für Wemos D1 Mini V4.0 (ESP8266) + RFID-RC522
 * Ausgabe über USB-Serial für Roboter-Integration
 * 
 * Hardware:
 * - Wemos D1 Mini V4.0 (ESP8266)
 * - MFRC522 RFID Reader Modul
 * - NFC-Tags (ISO14443A, z.B. MIFARE, NTAG)
 * 
 * Verbindungen MFRC522 -> Wemos D1 Mini V4.0:
 * --------------------------------------------
 * SDA/SS   -> D8  (GPIO15)
 * SCK      -> D5  (GPIO14)
 * MOSI     -> D7  (GPIO13)
 * MISO     -> D6  (GPIO12)
 * IRQ      -> nicht verbunden
 * GND      -> GND
 * RST      -> D3  (GPIO0)
 * 3.3V     -> 3.3V (WICHTIG: NICHT 5V!)
 * 
 * ⚠️ WICHTIG: D1 Mini Pin-Nummern ≠ GPIO-Nummern!
 * 
 * Serial Output Format:
 * NFC_ID:A1B2C3D4E5F6
 * 
 * Autor: Game Station Server
 * Version: 2.0 (D1 Mini Edition)
 * Datum: 21.10.2025
 */

#include <SPI.h>
#include <MFRC522.h>

// Pin-Konfiguration für Wemos D1 Mini V4.0
// WICHTIG: Verwende GPIO-Nummern, NICHT die D-Nummern!
#define RST_PIN         D3      // GPIO0  (D3 auf dem Board)
#define SS_PIN          D8      // GPIO15 (D8 auf dem Board)

// Alternative Pin-Konfiguration (falls Probleme mit D3/D8):
// #define RST_PIN      D4      // GPIO2
// #define SS_PIN       D2      // GPIO4

// LED-Pin (integrierte blaue LED)
#define LED_PIN         LED_BUILTIN  // GPIO2 (D4)

// RFID-Reader Objekt
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Variablen für Cooldown-Mechanismus
String lastUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_COOLDOWN = 2000;  // 2 Sekunden zwischen Scans

void setup() {
  // Serielle Kommunikation initialisieren (USB)
  Serial.begin(9600);
  delay(100);  // Kurze Pause für ESP8266
  
  // LED-Pin initialisieren
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED aus (bei ESP8266 ist HIGH = aus)
  
  // SPI-Bus initialisieren
  SPI.begin();
  
  // MFRC522 initialisieren
  mfrc522.PCD_Init();
  delay(100);
  
  // LED blinken als Ready-Signal
  blinkLED(3, 200);
}

void loop() {
  // Nach neuen Tags suchen
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  // Kein Tag vorhanden
  }
  
  // Tag-UID auslesen
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  // Lesefehler
  }
  
  // UID in String konvertieren
  String uid = getUID();
  
  // Cooldown-Prüfung (verhindert Mehrfach-Scans)
  unsigned long currentTime = millis();
  if (uid == lastUID && (currentTime - lastScanTime) < SCAN_COOLDOWN) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;  // Zu früh für erneuten Scan
  }
  
  // UID an Roboter senden
  sendToRobot(uid);
  
  // LED-Feedback
  blinkLED(1, 100);
  
  // Letzten Scan speichern
  lastUID = uid;
  lastScanTime = currentTime;
  
  // Tag deaktivieren
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

/**
 * Liest die UID des Tags aus und konvertiert zu Hex-String
 */
String getUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uid += "0";  // Führende Null für einstellige Hex-Werte
    }
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

/**
 * Sendet NFC-ID über Serial an den Roboter
 * Format: Nur die UID (z.B. A1B2C3D4E5F6)
 */
void sendToRobot(String uid) {
  // Nur die Seriennummer ausgeben
  Serial.println(uid);
}

/**
 * Blinkt die LED
 * @param times Anzahl der Blinkvorgänge
 * @param delayMs Verzögerung zwischen Ein/Aus in Millisekunden
 * 
 * HINWEIS: Bei ESP8266 ist die LED invertiert!
 * LOW = LED an, HIGH = LED aus
 */
void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, LOW);   // LED an (invertiert!)
    delay(delayMs);
    digitalWrite(LED_PIN, HIGH);  // LED aus (invertiert!)
    delay(delayMs);
  }
}

/**
 * Gibt erweiterte Tag-Informationen aus (optional)
 * Kann für Debugging aktiviert werden
 */
void printTagDetails() {
  Serial.println(F("--- Tag Details ---"));
  
  // Tag-Typ
  Serial.print(F("PICC Type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
  // UID
  Serial.print(F("UID: "));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // SAK
  Serial.print(F("SAK: 0x"));
  Serial.println(mfrc522.uid.sak, HEX);
  
  Serial.println(F("-------------------"));
  Serial.println();
}
