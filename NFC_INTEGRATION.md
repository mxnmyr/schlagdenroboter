# 📡 NFC-Reader Integration Guide

Vollständige Anleitung zur Integration des Arduino NFC-Readers mit dem Game Station Server.

## 📋 Inhaltsverzeichnis

1. [Hardware-Setup](#hardware-setup)
2. [Arduino-Code Upload](#arduino-code-upload)
3. [Software-Installation](#software-installation)
4. [Verwendung](#verwendung)
5. [Troubleshooting](#troubleshooting)

---

## 🔧 Hardware-Setup

### Benötigte Komponenten

- **Arduino Uno/Nano/Mega** (oder ESP8266/ESP32 für WiFi)
- **MFRC522 RFID/NFC Reader Modul**
- **NFC-Tags** (ISO14443A kompatibel)
- **USB-Kabel** (Arduino → Computer)
- **Breadboard + Kabel** (optional)

### Verkabelung MFRC522 → Arduino

| MFRC522 Pin | Arduino Uno/Nano | Arduino Mega | ESP32 |
|-------------|------------------|--------------|-------|
| SDA (SS)    | Pin 10          | Pin 10       | Pin 21 |
| SCK         | Pin 13          | Pin 52       | Pin 18 |
| MOSI        | Pin 11          | Pin 51       | Pin 23 |
| MISO        | Pin 12          | Pin 50       | Pin 19 |
| IRQ         | -               | -            | -      |
| GND         | GND             | GND          | GND    |
| RST         | Pin 9           | Pin 9        | Pin 22 |
| 3.3V        | 3.3V ⚠️         | 3.3V ⚠️      | 3.3V   |

**⚠️ WICHTIG:** MFRC522 benötigt 3.3V, NICHT 5V!

### Schaltplan

```
Arduino          MFRC522
  3.3V ────────── VCC
  GND  ────────── GND
  D9   ────────── RST
  D10  ────────── SDA
  D11  ────────── MOSI
  D12  ────────── MISO
  D13  ────────── SCK
```

---

## 💾 Arduino-Code Upload

### Schritt 1: Bibliothek installieren

1. Arduino IDE öffnen
2. `Sketch` → `Bibliothek einbinden` → `Bibliotheken verwalten`
3. Suche nach **"MFRC522"** von GithubCommunity
4. Klicke **"Installieren"**

### Schritt 2: Code hochladen

**Option A: Nur Serieller Output (Einfach)**

```cpp
// Verwende den originalen Code (bereits vorhanden)
// Datei: arduino_nfc_reader.ino
// Zeilen 1-60 (ohne WiFi)
```

1. Öffne `arduino_nfc_reader.ino`
2. Falls Arduino Uno/Nano: Code funktioniert direkt
3. Upload: `Sketch` → `Hochladen` (oder Ctrl+U)
4. Öffne Seriellen Monitor: `Tools` → `Serieller Monitor` (Ctrl+Shift+M)
5. Baudrate auf **9600** stellen

**Option B: WiFi-Modus (ESP8266/ESP32)**

1. Öffne `arduino_nfc_reader.ino`
2. Ändere WiFi-Einstellungen (Zeile 24-26):
   ```cpp
   const char* ssid = "DEIN_WIFI_NAME";
   const char* password = "DEIN_WIFI_PASSWORT";
   const char* serverUrl = "http://192.168.1.100:5000/api/nfc_scan";
   ```
3. Finde Server-IP-Adresse:
   ```powershell
   ipconfig
   # Suche nach "IPv4-Adresse" im WiFi-Adapter
   ```
4. Upload auf ESP8266/ESP32
5. NFC-Tags werden automatisch an Server gesendet!

---

## 🐍 Software-Installation

### Schritt 1: Python-Pakete installieren

```powershell
# Navigiere zum Projektordner
cd "C:\...\Heißer Draht Server"

# Installiere pyserial (für Arduino-Kommunikation)
pip install pyserial

# Installiere requests (für Server-Kommunikation)
pip install requests
```

### Schritt 2: Server starten

```powershell
# Terminal 1: Server starten
python server.py
```

Server läuft auf: `http://127.0.0.1:5000`

---

## 🎯 Verwendung

### Methode 1: Arduino Bridge (Empfohlen für Arduino Uno/Nano)

**Schritt 1:** Arduino mit NFC-Reader an Computer anschließen

**Schritt 2:** Arduino Bridge starten

```powershell
# Terminal 2: Arduino Bridge starten
python arduino_bridge.py
```

**Output:**
```
============================================================
🎮 Arduino NFC-Reader Bridge
============================================================
Verfügbare serielle Ports:
  1. COM3 - Arduino Uno (Arduino Uno)
  2. COM5 - USB Serial Port

✓ Arduino gefunden auf: COM3

✓ Verbunden mit COM3 @ 9600 baud

🔍 Warte auf NFC-Tags...
   (Drücke Ctrl+C zum Beenden)
```

**Schritt 3:** NFC-Tag an Reader halten

```
------------------------------------------------------------
📡 NFC-Tag erkannt: 1A2B3C4D
  ✓ Server-Antwort: success
  ⚠️  Chip noch nicht benannt
------------------------------------------------------------
```

**Schritt 4:** Admin-Panel öffnen → Namen zuweisen

```
http://127.0.0.1:5000/admin
```

### Methode 2: ESP8266/ESP32 mit WiFi (Automatisch)

**Vorteile:**
- ✅ Kein Computer nötig
- ✅ Direkte Verbindung zum Server
- ✅ Standalone-Betrieb möglich

**Setup:**
1. WiFi-Credentials in Arduino-Code eintragen
2. Server-IP eintragen
3. ESP hochladen
4. ESP mit Strom versorgen
5. **Fertig!** NFC-Tags werden automatisch gesendet

### Methode 3: Manuell (ohne Arduino)

Für Tests oder wenn kein Arduino verfügbar:

```powershell
# Admin-Panel öffnen
Start-Process "http://127.0.0.1:5000/admin"

# NFC-ID manuell eingeben im Formular
# "NFC-Chip manuell hinzufügen"
```

---

## 🎮 Workflow: Spieler registrieren

### Variante A: Mit Arduino Bridge

```
1. Server läuft (python server.py)
   ↓
2. Arduino Bridge läuft (python arduino_bridge.py)
   ↓
3. NFC-Tag an Reader halten
   ↓
4. Bridge sendet NFC-ID an Server
   ↓
5. Admin-Panel öffnen (http://127.0.0.1:5000/admin)
   ↓
6. Namen in Tabelle eingeben + "Zuweisen" klicken
   ↓
7. ✅ Spieler bereit für Spiele!
```

### Variante B: Mit ESP8266/ESP32 WiFi

```
1. Server läuft (python server.py)
   ↓
2. ESP8266/ESP32 mit NFC-Reader eingeschaltet
   ↓
3. NFC-Tag an Reader halten
   ↓
4. ESP sendet direkt an Server (per WiFi)
   ↓
5. Admin-Panel zeigt neuen Chip sofort an
   ↓
6. Namen zuweisen
   ↓
7. ✅ Spieler bereit!
```

---

## 🔍 Troubleshooting

### Problem: Arduino nicht gefunden

**Symptom:**
```
❌ Keine seriellen Ports gefunden!
```

**Lösungen:**
1. ✅ Arduino per USB anschließen
2. ✅ Arduino-Treiber installieren (CH340/FTDI)
3. ✅ Anderen USB-Port versuchen
4. ✅ Arduino IDE Serial Monitor schließen (blockiert Port!)
5. ✅ Windows: Geräte-Manager → COM-Ports prüfen

### Problem: Port-Zugriff verweigert

**Symptom:**
```
❌ Fehler beim Öffnen von COM3: PermissionError
```

**Lösungen:**
1. ✅ **Arduino IDE Serial Monitor schließen!** (häufigste Ursache)
2. ✅ Andere Programme schließen die auf Serial zugreifen
3. ✅ Arduino neu anschließen
4. ✅ Computer neu starten (als letztes Mittel)

### Problem: Keine Daten vom Arduino

**Symptom:**
```
🔍 Warte auf NFC-Tags...
(nichts passiert beim Scannen)
```

**Lösungen:**
1. ✅ Prüfe Arduino Serial Monitor direkt:
   ```
   Arduino IDE → Tools → Serieller Monitor
   Baudrate: 9600
   ```
2. ✅ Solltest du sehen: "Scan PICC to see UID..."
3. ✅ NFC-Tag scannen → UID sollte erscheinen
4. ✅ Falls nichts: Verkabelung prüfen (siehe Hardware-Setup)
5. ✅ Falls "Protocol error": MFRC522 defekt oder falsch verkabelt

### Problem: Server nicht erreichbar

**Symptom:**
```
❌ Verbindungsfehler: Connection refused
```

**Lösungen:**
1. ✅ Prüfe ob Server läuft: `python server.py`
2. ✅ Browser-Test: http://127.0.0.1:5000
3. ✅ Firewall-Einstellungen prüfen
4. ✅ In `arduino_bridge.py` URL prüfen (Zeile 12)

### Problem: ESP8266/ESP32 verbindet nicht mit WiFi

**Symptom:**
```
✗ WiFi-Verbindung fehlgeschlagen!
```

**Lösungen:**
1. ✅ SSID und Passwort korrekt? (Groß-/Kleinschreibung!)
2. ✅ 2.4 GHz WiFi verwenden (nicht 5 GHz)
3. ✅ Router-Reichweite prüfen
4. ✅ Serial Monitor öffnen → Fehlermeldung lesen
5. ✅ WiFi-Zugangsdaten in `arduino_nfc_reader.ino` Zeile 24-26

### Problem: NFC-Tag wird nicht erkannt

**Symptom:**
- Arduino läuft, aber keine Reaktion beim Scannen

**Lösungen:**
1. ✅ **Abstand:** Tag direkt an Reader halten (< 3cm)
2. ✅ **NFC-Typ:** Nur ISO14443A Tags funktionieren (z.B. NTAG213, MIFARE Classic)
3. ✅ **Spannung:** MFRC522 bekommt 3.3V (NICHT 5V!)
4. ✅ **Verkabelung:** Alle 7 Pins korrekt verbunden?
5. ✅ **LED:** Blinkt LED auf MFRC522 beim Scannen?
6. ✅ **Test:** Arduino-Beispiel testen:
   ```
   Arduino IDE → File → Examples → MFRC522 → DumpInfo
   ```

### Problem: Gleicher Tag wird mehrfach gescannt

**Symptom:**
```
📡 NFC-Tag erkannt: 1A2B3C4D
📡 NFC-Tag erkannt: 1A2B3C4D
📡 NFC-Tag erkannt: 1A2B3C4D
```

**Lösung:**
- ✅ Bereits implementiert! Cooldown-Mechanismus (3 Sekunden)
- ✅ Tag vom Reader entfernen nach Scan
- ✅ Falls dennoch: Cooldown erhöhen in `arduino_bridge.py` Zeile 94

---

## 📊 Datenfluss-Diagramm

```
┌─────────────┐
│  NFC-Tag    │
└──────┬──────┘
       │ (RFID 13.56 MHz)
       ▼
┌─────────────┐
│  MFRC522    │ ← Hardware: Liest UID
└──────┬──────┘
       │ (SPI)
       ▼
┌─────────────┐
│  Arduino    │ ← Firmware: Verarbeitet UID
└──────┬──────┘
       │
       ├─ Option A (Seriell) ─────────┐
       │                              ▼
       │                    ┌──────────────────┐
       │                    │ arduino_bridge.py│
       │                    │  (Python Script) │
       │                    └────────┬─────────┘
       │                             │ (HTTP POST)
       │                             ▼
       └─ Option B (WiFi) ──────────────────┐
                                             ▼
                                   ┌──────────────┐
                                   │  server.py   │
                                   │   (Flask)    │
                                   └──────┬───────┘
                                          │
                                ┌─────────┼─────────┐
                                ▼         ▼         ▼
                          ┌──────┐  ┌──────┐  ┌──────┐
                          │ nfc_ │  │game_ │  │admin │
                          │mapping│  │data  │  │.html │
                          └──────┘  └──────┘  └──────┘
```

---

## 🧪 Test-Befehle

### Test 1: Server erreichbar?

```powershell
Invoke-RestMethod -Uri "http://127.0.0.1:5000/" -Method GET
```

### Test 2: NFC-Scan API testen

```powershell
$body = @{"nfc_id" = "TEST123"} | ConvertTo-Json
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body -ContentType "application/json"
```

**Erwartete Ausgabe:**
```json
{
  "status": "success",
  "nfc_id": "TEST123",
  "exists": false,
  "has_name": false,
  "player_name": "Unbenannt"
}
```

### Test 3: Arduino Serial Monitor

```
1. Arduino IDE öffnen
2. Tools → Serieller Monitor
3. Baudrate: 9600
4. NFC-Tag scannen

Erwartete Ausgabe:
========================================
✓ NFC-Tag erkannt: 1A2B3C4D
========================================
PICC type: MIFARE 1KB
Card UID: 1A 2B 3C 4D
...
```

---

## 📚 Zusätzliche Ressourcen

### NFC-Tag Typen (Kompatibel)

✅ **NTAG213/215/216** - Modern, empfohlen
✅ **MIFARE Classic 1K/4K** - Klassisch
✅ **MIFARE Ultralight** - Günstig
❌ **NFC-Forum Type 3/4** - Nicht kompatibel
❌ **HF Tags (nicht 13.56 MHz)** - Nicht kompatibel

### Bibliotheken

- **MFRC522:** https://github.com/miguelbalboa/rfid
- **PySerial:** https://pyserial.readthedocs.io/
- **Flask:** https://flask.palletsprojects.com/

### Arduino Pin-Referenzen

- **Uno/Nano:** https://www.arduino.cc/en/Reference/Board
- **Mega:** https://www.arduino.cc/en/Hacking/PinMapping2560
- **ESP32:** https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

---

## ✅ Quick Start Checkliste

### Hardware
- [ ] MFRC522 an Arduino angeschlossen (7 Pins)
- [ ] Arduino mit 3.3V versorgt (nicht 5V!)
- [ ] USB-Kabel verbunden

### Software
- [ ] MFRC522-Bibliothek in Arduino IDE installiert
- [ ] `arduino_nfc_reader.ino` hochgeladen
- [ ] Serial Monitor getestet (Baudrate 9600)
- [ ] `pyserial` installiert (`pip install pyserial`)
- [ ] `requests` installiert (`pip install requests`)

### Betrieb
- [ ] Server läuft (`python server.py`)
- [ ] Arduino Bridge läuft (`python arduino_bridge.py`)
- [ ] Admin-Panel geöffnet (`http://127.0.0.1:5000/admin`)
- [ ] Test-NFC-Tag gescannt
- [ ] Namen zugewiesen
- [ ] ✅ System bereit!

---

## 🎉 Fertig!

Dein NFC-Reader ist jetzt voll integriert!

**Nächste Schritte:**
1. Registriere alle Spieler-NFC-Tags
2. Starte die Spiele
3. Leaderboards füllen sich automatisch

**Bei Problemen:** Siehe [Troubleshooting](#troubleshooting)

---

**💡 Tipp:** Für Produktion mit ESP8266/ESP32 arbeiten - dann läuft alles standalone ohne Computer!
