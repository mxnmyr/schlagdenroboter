/*
 * NFC Reader für Game Station Server
 * 
 * Liest NFC-Tags aus und sendet die UID automatisch an den Server
 * 
 * Hardware:
 * - Arduino Uno/Nano/Mega
 * - MFRC522 RFID Reader
 * - ESP8266 WiFi-Modul ODER ESP32 (für WiFi-Verbindung)
 * 
 * Verbindungen MFRC522:
 * RST  -> Pin 9
 * SS   -> Pin 10
 * MOSI -> Pin 11
 * MISO -> Pin 12
 * SCK  -> Pin 13
 */

#include <SPI.h>
#include <MFRC522.h>

// Falls ESP8266 oder ESP32 verwendet wird:
#if defined(ESP8266) || defined(ESP32)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #define WIFI_ENABLED true
#else
  #define WIFI_ENABLED false
#endif

// RFID Pin-Konfiguration
#define RST_PIN         9
#define SS_PIN          10

// WiFi-Konfiguration (nur für ESP8266/ESP32)
const char* ssid = "DEIN_WIFI_NAME";           // WiFi-Name hier eintragen
const char* password = "DEIN_WIFI_PASSWORT";   // WiFi-Passwort hier eintragen
const char* serverUrl = "http://192.168.1.100:5000/api/nfc_scan";  // Server-IP anpassen

MFRC522 mfrc522(SS_PIN, RST_PIN);
String lastUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_COOLDOWN = 2000; // 2 Sekunden zwischen Scans

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("=== NFC Reader für Game Station ==="));
  
  // RFID initialisieren
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  
  #if WIFI_ENABLED
    // WiFi verbinden
    Serial.print(F("Verbinde mit WiFi: "));
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F("\n✓ WiFi verbunden!"));
      Serial.print(F("IP-Adresse: "));
      Serial.println(WiFi.localIP());
    } else {
      Serial.println(F("\n✗ WiFi-Verbindung fehlgeschlagen!"));
      Serial.println(F("Arbeite nur im Seriellen Modus..."));
    }
  #else
    Serial.println(F("Modus: Nur Serieller Output"));
    Serial.println(F("Für WiFi-Funktion ESP8266/ESP32 verwenden!"));
  #endif
  
  Serial.println(F("\nBereit zum Scannen..."));
  Serial.println(F("Halte NFC-Tag an den Leser.\n"));
}

void loop() {
  // Suche nach neuen Karten
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // UID auslesen
  String uid = getUID();
  
  // Cooldown prüfen (verhindert mehrfaches Scannen derselben Karte)
  unsigned long currentTime = millis();
  if (uid == lastUID && (currentTime - lastScanTime) < SCAN_COOLDOWN) {
    mfrc522.PICC_HaltA();
    return;
  }
  
  lastUID = uid;
  lastScanTime = currentTime;
  
  // Ausgabe im Seriellen Monitor
  Serial.println(F("========================================"));
  Serial.print(F("✓ NFC-Tag erkannt: "));
  Serial.println(uid);
  Serial.println(F("========================================"));
  
  // Details ausgeben
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  #if WIFI_ENABLED
    // An Server senden (nur wenn WiFi verbunden)
    if (WiFi.status() == WL_CONNECTED) {
      sendToServer(uid);
    } else {
      Serial.println(F("✗ Nicht mit WiFi verbunden - keine Übertragung"));
    }
  #else
    // Serieller Modus: Spezielle Ausgabe für Python-Script
    Serial.print(F("NFC_ID:"));
    Serial.println(uid);
  #endif
  
  // Piep-Ton (optional - LED an Pin 13 blinken)
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Karte deaktivieren
  mfrc522.PICC_HaltA();
  
  Serial.println(F("\nBereit für nächsten Scan...\n"));
}

// Funktion: UID als String auslesen
String getUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uid += "0";
    }
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

#if WIFI_ENABLED
// Funktion: UID an Server senden
void sendToServer(String uid) {
  HTTPClient http;
  WiFiClient client;
  
  Serial.print(F("Sende an Server: "));
  Serial.println(serverUrl);
  
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  // JSON erstellen
  String jsonPayload = "{\"nfc_id\":\"" + uid + "\"}";
  
  // POST-Request
  int httpResponseCode = http.POST(jsonPayload);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print(F("✓ Server-Antwort ("));
    Serial.print(httpResponseCode);
    Serial.print(F("): "));
    Serial.println(response);
  } else {
    Serial.print(F("✗ Fehler beim Senden: "));
    Serial.println(httpResponseCode);
  }
  
  http.end();
}
#endif
