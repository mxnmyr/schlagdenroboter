# 🎮 Game Station Server - Leaderboard System

Ein Flask-basiertes Leaderboard-System für drei verschiedene Spiele mit NFC-Chip-basierter Spielerverwaltung.

## 📋 Inhaltsverzeichnis

- [Übersicht](#übersicht)
- [Systemarchitektur](#systemarchitektur)
- [Installation](#installation)
- [NFC-Reader Integration](#nfc-reader-integration)
- [Server starten](#server-starten)
- [Spiele & Endpunkte](#spiele--endpunkte)
- [Verwaltungssystem](#verwaltungssystem)
- [Datenspeicherung](#datenspeicherung)
- [Testing](#testing)
- [Troubleshooting](#troubleshooting)

---

## 🎯 Übersicht

Das System verwaltet Leaderboards für drei Spiele:
- 🔥 **Heißer Draht** - Geschicklichkeitsspiel mit Zeit, Fehlern und Schwierigkeitsgrad
- 🎲 **Vier Gewinnt** - Strategiespiel mit Siegquote und Schwierigkeitsgrad
- 🧩 **Puzzle** - Logikspiel mit Zeit und Schwierigkeitsgrad

### Key Features

✅ **NFC-Chip Verwaltung** - Spieler werden über NFC-Chips identifiziert  
✅ **Chip-Wiederverwendung** - Chips können nach Abschluss neu zugewiesen werden  
✅ **Daten-Archivierung** - Alte Spielerdaten bleiben in Leaderboards erhalten  
✅ **Echtzeit-Leaderboards** - Top 5 und Letzte 5 für jedes Spiel  
✅ **Urkunden-Generator** - Thermaldrucker-optimierte Zertifikate (4"x6")  
✅ **Responsive Design** - Optimiert für Raspberry Pi 3 B+

---

## 🏗️ Systemarchitektur

### Projektstruktur

```
Heißer Draht Server/
├── server.py                          # Haupt-Flask-Server
├── game_data.json                     # Aktive Spielerdaten
├── nfc_mapping.json                   # NFC-ID → Spielername Zuordnung
├── game_archive.json                  # Archivierte Spieler-Sessions
├── API_DOKUMENTATION.md               # Detaillierte API-Docs
├── README.md                          # Diese Datei
│
├── static/                            # Statische Dateien
│   ├── logo.png                       # Firmenlogo
│   └── luh_logo.png                   # Universitätslogo
│
└── templates/                         # HTML-Templates
    ├── home.html                      # Hauptseite (Top 5 alle Spiele)
    ├── leaderboard_heisser_draht.html # Heißer Draht Details
    ├── leaderboard_vier_gewinnt.html  # Vier Gewinnt Details
    ├── leaderboard_puzzle.html        # Puzzle Details
    ├── admin.html                     # Verwaltungsseite
    └── certificate_multi.html         # Multi-Game Urkunde
```

### Datenfluss

```
┌─────────────┐
│ Arduino/ESP │ ──POST──> ┌──────────────┐
│  NFC Reader │           │ Flask Server │
└─────────────┘           │  (server.py) │
                          └───────┬──────┘
                                  │
                    ┌─────────────┼─────────────┐
                    ▼             ▼             ▼
            ┌──────────┐  ┌──────────┐  ┌──────────┐
            │game_data │  │nfc_map   │  │archive   │
            │  .json   │  │ping.json │  │  .json   │
            └──────────┘  └──────────┘  └──────────┘
                    │             │             │
                    └─────────────┼─────────────┘
                                  ▼
                          ┌───────────────┐
                          │  Leaderboards │
                          │  (HTML/CSS)   │
                          └───────────────┘
```

### Datenbank-Schema

#### nfc_mapping.json
```json
{
  "12345ABC": "Max Mustermann",
  "67890DEF": "Anna Schmidt"
}
```

#### game_data.json
```json
{
  "12345ABC": {
    "heisser_draht": [
      {
        "name": "Max Mustermann",
        "time": 12.5,
        "errors": 2,
        "difficulty": "Mittel",
        "timestamp": "2025-10-10T14:30:00.000000"
      }
    ],
    "vier_gewinnt": [
      {
        "name": "Max Mustermann",
        "result": "won",
        "difficulty": "Schwer",
        "timestamp": "2025-10-10T14:35:00.000000"
      }
    ],
    "puzzle": [
      {
        "name": "Max Mustermann",
        "time": 38.75,
        "difficulty": "Schwer",
        "timestamp": "2025-10-10T14:40:00.000000"
      }
    ]
  }
}
```

#### game_archive.json
```json
[
  {
    "name": "Max Mustermann",
    "heisser_draht": [...],
    "vier_gewinnt": [...],
    "puzzle": [...],
    "archived_date": "2025-10-10T15:00:00.000000",
    "original_nfc_id": "12345ABC"
  }
]
```

---

## 🚀 Installation

### Voraussetzungen

- Python 3.8+
- Flask
- Webbrowser (für Leaderboard-Anzeige)

### Setup

```powershell
# 1. Repository klonen oder Ordner öffnen
cd "C:\...\Heißer Draht Server"

# 2. Abhängigkeiten installieren
pip install flask
pip install pyserial requests  # Für Arduino NFC-Reader

# 3. Server starten
python server.py
```

### Erste Schritte

1. **Server starten** → Server läuft auf `http://localhost:5000`
2. **Admin-Panel öffnen** → `http://localhost:5000/admin`
3. **NFC-Reader einrichten** → Siehe [NFC-Reader Integration](#nfc-reader-integration)
4. **NFC-Chips registrieren** → Mit Arduino scannen oder manuell eingeben
5. **Spiele spielen** → Daten über API senden
6. **Leaderboards anzeigen** → Automatische Updates

---

## 📡 NFC-Reader Integration

Das System unterstützt **automatisches Einlesen von NFC-Tags** über Arduino!

### Hardware-Setup

**Benötigt:**
- Arduino Uno/Nano/Mega (oder ESP8266/ESP32 mit WiFi)
- MFRC522 RFID/NFC Reader Modul
- NFC-Tags (ISO14443A)

**Verkabelung:**
```
Arduino Uno    →    MFRC522
3.3V           →    VCC (⚠️ NICHT 5V!)
GND            →    GND
Pin 9          →    RST
Pin 10         →    SDA
Pin 11         →    MOSI
Pin 12         →    MISO
Pin 13         →    SCK
```

### Software-Setup

**1. Arduino-Bibliothek installieren:**
```
Arduino IDE → Bibliotheken verwalten → "MFRC522" installieren
```

**2. Arduino-Code hochladen:**
```
Datei öffnen: arduino_nfc_reader.ino
Upload auf Arduino
```

**3. Python-Bridge starten:**
```powershell
# Terminal 1: Server
python server.py

# Terminal 2: Arduino-Bridge
python arduino_bridge.py
```

**4. NFC-Tags scannen:**
- NFC-Tag an Reader halten
- Bridge sendet ID automatisch an Server
- Admin-Panel zeigt neuen Chip an
- Namen zuweisen → Fertig!

### Optionen

**Option A: Serieller Modus (Arduino Uno/Nano)**
- Arduino per USB an Computer
- `arduino_bridge.py` läuft auf Computer
- Sendet Daten per HTTP an Server

**Option B: WiFi-Modus (ESP8266/ESP32)**
- ESP verbindet sich direkt mit WiFi
- Sendet Daten direkt an Server
- Kein Computer nötig!
- WiFi-Credentials in `arduino_nfc_reader.ino` eintragen

**Option C: Manuell (ohne Hardware)**
- Admin-Panel → "NFC-Chip manuell hinzufügen"
- NFC-ID eintippen

### 📖 Detaillierte Anleitung

**→ Siehe `NFC_INTEGRATION.md` für:**
- Schritt-für-Schritt Hardware-Setup
- Verkabelungs-Diagramme
- Troubleshooting
- ESP8266/ESP32 WiFi-Konfiguration
- Arduino-Code Erklärungen

---

## 🎮 Spiele & Endpunkte

### 1. 🔥 Heißer Draht

**Beschreibung:** Geschicklichkeitsspiel - Draht berühren ohne Kontakt

**API-Endpunkt:** `POST /api/heisser_draht`

**Datenformat:**
```json
{
  "nfc_id": "12345ABC",
  "time": 12.50,
  "errors": 2,
  "difficulty": "Mittel"
}
```

**PowerShell Befehl:**
```powershell
$body = @{
    "nfc_id" = "12345ABC"
    "time" = 12.50
    "errors" = 2
    "difficulty" = "Mittel"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/heisser_draht" `
    -Method POST -Body $body -ContentType "application/json"
```

**Bewertung:**
- **Top 5:** Schnellste Zeit (weniger ist besser)
- **Bottom 5:** Langsamste Zeit (mehr ist schlechter)
- Angezeigt: Zeit, Fehler, Schwierigkeitsgrad, Zeitstempel

---

### 2. 🎲 Vier Gewinnt

**Beschreibung:** Strategiespiel - Vier in einer Reihe

**API-Endpunkt:** `POST /api/vier_gewinnt`

**Datenformat:**
```json
{
  "nfc_id": "12345ABC",
  "result": "won",
  "difficulty": "Schwer"
}
```

**Hinweis:** `result` muss `"won"` oder `"lost"` sein

**PowerShell Befehl:**
```powershell
$body = @{
    "nfc_id" = "12345ABC"
    "result" = "won"
    "difficulty" = "Schwer"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/vier_gewinnt" `
    -Method POST -Body $body -ContentType "application/json"
```

**Bewertung:**
- **Top 5:** Höchste Siegquote (Siege/Gesamt × 100%)
- **Bottom 5:** Niedrigste Siegquote
- Angezeigt: Siege, Gesamt-Spiele, Quote, Zeitstempel (letztes Spiel)

---

### 3. 🧩 Puzzle

**Beschreibung:** Logikspiel - Puzzle in kürzester Zeit lösen

**API-Endpunkt:** `POST /api/puzzle`

**Datenformat:**
```json
{
  "nfc_id": "12345ABC",
  "time": 38.75,
  "difficulty": "Schwer"
}
```

**PowerShell Befehl:**
```powershell
$body = @{
    "nfc_id" = "12345ABC"
    "time" = 38.75
    "difficulty" = "Schwer"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/puzzle" `
    -Method POST -Body $body -ContentType "application/json"
```

**Bewertung:**
- **Top 5:** Schnellste Zeit (weniger ist besser)
- **Bottom 5:** Langsamste Zeit
- Angezeigt: Zeit, Schwierigkeitsgrad, Zeitstempel

---

## ⚙️ Verwaltungssystem

### Admin-Panel

**URL:** `http://localhost:5000/admin`

#### Funktionen:

1. **➕ Neuen NFC-Chip hinzufügen**
   - NFC-ID eingeben (Pflichtfeld)
   - Spielername eingeben (optional)
   - Chip wird sofort zur Tabelle hinzugefügt

2. **Namen zuweisen/ändern**
   - **"➕ Zuweisen"** - Bei neuen, unbenannten Chips
   - **"✏️ Ändern"** - Bei benannten Chips ohne Spiele
   - **"🔄 Neu zuweisen (Chip zurücksetzen)"** - Bei Chips mit Spielen

3. **Chip-Neuzuweisung (Wichtig!)**
   - Wenn ein Chip neu zugewiesen wird:
     - ✅ Alte Spieldaten werden ins **Archiv** verschoben
     - ✅ Alter Name bleibt in **Leaderboards** sichtbar
     - ✅ Chip beginnt mit **leeren Spieldaten**
     - ✅ Neuer Spieler kann von vorne beginnen

4. **🏅 Urkunde generieren**
   - Verfügbar wenn Spieler **alle 3 Spiele** abgeschlossen hat
   - Optimiert für **MUNBYN Thermaldrucker** (4"x6")
   - Zeigt Ergebnisse aller 3 Spiele

#### Chip-Status Badges:

| Badge | Bedeutung |
|-------|-----------|
| ✓ Alle Spiele | Spieler hat alle 3 Spiele abgeschlossen |
| ⏳ In Bearbeitung | Spieler hat Namen, aber noch nicht alle Spiele |
| ⚠ Unbenannt | Chip existiert, aber hat keinen Namen |

---

## 💾 Datenspeicherung

### Persistente JSON-Dateien

| Datei | Beschreibung | Auto-Erstellt |
|-------|-------------|---------------|
| `nfc_mapping.json` | NFC-ID → Spielername | ✅ |
| `game_data.json` | Aktive Spieler-Daten | ✅ |
| `game_archive.json` | Archivierte Sessions | ✅ |

### Daten-Lebenszyklus

```
1. Neuer Spieler
   └─> nfc_mapping.json: NFC-ID → Name
   └─> game_data.json: Leere Spieldaten

2. Spiele spielen
   └─> game_data.json: Spieldaten werden hinzugefügt
   └─> Name wird in jedem Eintrag gespeichert

3. Chip neu zuweisen
   └─> game_archive.json: Alte Daten archivieren
   └─> nfc_mapping.json: Neuer Name zuweisen
   └─> game_data.json: Spieldaten zurücksetzen

4. Leaderboards
   └─> Zeigen: game_data + game_archive
   └─> Namen bleiben erhalten!
```

### Backup-Strategie

**Empfohlen:** Regelmäßige Backups der JSON-Dateien

```powershell
# Backup erstellen
$date = Get-Date -Format "yyyy-MM-dd_HHmmss"
Copy-Item "*.json" -Destination "backup_$date/"
```

---

## 🧪 Testing

### Kompletter Test-Workflow

```powershell
# === TEST 1: Spieler erstellen ===
Write-Host "`n=== TEST 1: Spieler hinzufügen ==="
$form = @{nfc_id='TEST001'; name='Max Testmann'}
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/add_nfc" `
    -Method POST -Body $form -UseBasicParsing | Out-Null
Write-Host "✓ Max Testmann hinzugefügt"

# === TEST 2: Heißer Draht ===
Write-Host "`n=== TEST 2: Heißer Draht ==="
$body1 = @{
    "nfc_id" = "TEST001"
    "time" = 12.50
    "errors" = 2
    "difficulty" = "Mittel"
} | ConvertTo-Json

$result1 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/heisser_draht" `
    -Method POST -Body $body1 -ContentType "application/json"
Write-Host "✓ Heißer Draht: $($result1.status) - $($result1.player_name)"

# === TEST 3: Vier Gewinnt ===
Write-Host "`n=== TEST 3: Vier Gewinnt ==="
$body2 = @{
    "nfc_id" = "TEST001"
    "result" = "won"
    "difficulty" = "Schwer"
} | ConvertTo-Json

$result2 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/vier_gewinnt" `
    -Method POST -Body $body2 -ContentType "application/json"
Write-Host "✓ Vier Gewinnt: $($result2.status) - $($result2.player_name)"

# === TEST 4: Puzzle ===
Write-Host "`n=== TEST 4: Puzzle ==="
$body3 = @{
    "nfc_id" = "TEST001"
    "time" = 38.75
    "difficulty" = "Schwer"
} | ConvertTo-Json

$result3 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/puzzle" `
    -Method POST -Body $body3 -ContentType "application/json"
Write-Host "✓ Puzzle: $($result3.status) - $($result3.player_name)"

Write-Host "`n=== ALLE TESTS ERFOLGREICH ==="
Write-Host "Max Testmann hat alle 3 Spiele abgeschlossen!"
Write-Host "Urkunde verfügbar unter: http://127.0.0.1:5000/admin"
```

### Test: Chip-Neuzuweisung

```powershell
# === TEST 5: Chip neu zuweisen ===
Write-Host "`n=== TEST 5: Chip-Neuzuweisung ==="

# Alten Spielstand prüfen
Write-Host "Vor Neuzuweisung:"
Get-Content "game_data.json" -Raw | ConvertFrom-Json | ConvertTo-Json -Depth 3

# Chip neu zuweisen
$form = @{nfc_id='TEST001'; name='Anna Neustadt'}
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/assign_name" `
    -Method POST -Body $form -UseBasicParsing | Out-Null
Write-Host "✓ Chip TEST001 wurde Anna Neustadt zugewiesen"

# Archiv prüfen
Write-Host "`nArchiv:"
Get-Content "game_archive.json" -Raw | ConvertFrom-Json | ConvertTo-Json -Depth 3

# Neuen Spielstand prüfen
Write-Host "`nNach Neuzuweisung:"
Get-Content "game_data.json" -Raw | ConvertFrom-Json | ConvertTo-Json -Depth 3

Write-Host "`n✓ Max Testmanns Daten im Archiv"
Write-Host "✓ Chip bereit für Anna Neustadt"
```

### Schnelltests

```powershell
# Test 1: Server läuft?
Invoke-RestMethod -Uri "http://127.0.0.1:5000/" -Method GET -TimeoutSec 5

# Test 2: Admin-Panel erreichbar?
Invoke-RestMethod -Uri "http://127.0.0.1:5000/admin" -Method GET

# Test 3: API antwortet?
$testBody = @{"nfc_id"="QUICK_TEST"; "time"=10.5; "errors"=0; "difficulty"="Leicht"} | ConvertTo-Json
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/heisser_draht" -Method POST -Body $testBody -ContentType "application/json"
```

---

## 🌐 Web-Interface

### Seiten-Übersicht

| URL | Beschreibung |
|-----|-------------|
| `/` | **Hauptseite** - Top 5 aller 3 Spiele nebeneinander |
| `/leaderboard/heisser_draht` | **Heißer Draht Details** - Top 5 & Letzte 5 nebeneinander |
| `/leaderboard/vier_gewinnt` | **Vier Gewinnt Details** - Top 5 & Letzte 5 nebeneinander |
| `/leaderboard/puzzle` | **Puzzle Details** - Top 5 & Letzte 5 nebeneinander |
| `/admin` | **Verwaltung** - NFC-Chips verwalten, Urkunden drucken |
| `/admin/certificate/<nfc_id>` | **Urkunde** - Multi-Game Zertifikat für Thermaldrucker |

### Design-Features

- 🎨 **Farbe:** #b1cb21 (Firmenfarbe Gelb-Grün)
- 🏆 **Top 3:** Gold, Silber, Bronze Farbgebung
- 📱 **Responsive:** Optimiert für Desktop & Tablet
- 🖥️ **Performance:** Optimiert für Raspberry Pi 3 B+
- 🖨️ **Drucker:** 4"x6" Thermaldrucker-Layout

### Leaderboard-Anzeige

**Top 5:**
- Nummerierte Liste (1, 2, 3, 4, 5)
- Gold/Silber/Bronze für Top 3
- Alle Statistiken sichtbar

**Letzte 5:**
- 📅 Kalender-Icon statt Nummern
- 🕒 Zeitstempel (Datum + Uhrzeit)
- Alle Statistiken + Timestamp

---

## 🔧 Troubleshooting

### Problem: Server startet nicht

```powershell
# Lösung 1: Prüfe Python-Installation
python --version

# Lösung 2: Flask neu installieren
pip install --upgrade flask

# Lösung 3: Port bereits belegt?
# Ändere Port in server.py Zeile 474:
# app.run(host='0.0.0.0', port=5001, debug=True)
```

### Problem: Daten werden nicht gespeichert

```powershell
# Lösung: Prüfe Schreibrechte
Test-Path -Path "." -PathType Container
Get-Acl "." | Format-List

# Falls nötig: Als Administrator ausführen
```

### Problem: NFC-ID wird nicht erkannt

```powershell
# Prüfe JSON-Format
$testBody = @{"nfc_id"="TEST"; "time"=10.5; "errors"=0; "difficulty"="Test"} | ConvertTo-Json
Write-Host $testBody

# Prüfe Server-Log
# Konsole zeigt: "Heißer Draht Daten empfangen: {...}"
```

### Problem: Leaderboard zeigt alte Namen

**Ursache:** Daten wurden VOR der Archiv-Implementierung gespeichert

**Lösung:**
```powershell
# Option 1: Daten neu senden (empfohlen)
# Spiele erneut durchführen

# Option 2: JSON manuell bearbeiten
# Füge "name" Feld zu jedem Eintrag hinzu

# Option 3: Neustart mit leeren Daten
Remove-Item "*.json"
# Server neu starten
```

### Problem: Urkunde wird nicht angezeigt

**Checkliste:**
- [ ] Spieler hat **alle 3 Spiele** abgeschlossen?
- [ ] NFC-ID korrekt in Admin-Panel?
- [ ] Browser-Cache geleert? (Strg+F5)

```powershell
# Prüfe Spieler-Status
$data = Get-Content "game_data.json" -Raw | ConvertFrom-Json
$nfcId = "TEST001"
Write-Host "Heißer Draht: $($data.$nfcId.heisser_draht.Count)"
Write-Host "Vier Gewinnt: $($data.$nfcId.vier_gewinnt.Count)"
Write-Host "Puzzle: $($data.$nfcId.puzzle.Count)"
# Alle > 0 → Urkunde verfügbar
```

---

## 📊 Server-Konfiguration

### Flask Debug-Modus

**Standard:** Debug-Modus ist aktiviert (`debug=True`)

**Vorteile:**
- ✅ Auto-Reload bei Code-Änderungen
- ✅ Detaillierte Fehler-Seiten
- ✅ Entwickler-Konsole

**Für Produktion:**
```python
# In server.py Zeile 474 ändern:
app.run(host='0.0.0.0', port=5000, debug=False)
```

### Netzwerk-Zugriff

**Lokal:** `http://127.0.0.1:5000`  
**Netzwerk:** `http://<RASPBERRY_PI_IP>:5000`

```powershell
# IP-Adresse finden
ipconfig
# Suche nach "IPv4-Adresse"
```

### Performance-Tipps

1. **Raspberry Pi Optimierung:**
   - Swap-Speicher erhöhen
   - Unnötige Services deaktivieren
   - Lite-Version von Raspberry Pi OS verwenden

2. **Browser-Optimierung:**
   - Cache aktivieren
   - Hardware-Beschleunigung aktivieren
   - Chromium im Kiosk-Modus

3. **Datenbank-Optimierung:**
   - Alte Archiv-Einträge periodisch löschen
   - JSON-Dateien komprimieren (optional)

---

## 📝 Arduino/ESP32 Integration

### Beispiel-Code (ESP32)

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "http://192.168.1.100:5000/api/heisser_draht";

void sendGameData(String nfcId, float time, int errors, String difficulty) {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  StaticJsonDocument<200> doc;
  doc["nfc_id"] = nfcId;
  doc["time"] = time;
  doc["errors"] = errors;
  doc["difficulty"] = difficulty;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    Serial.print("✓ Daten gesendet: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("❌ Fehler: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✓ WiFi verbunden");
}

void loop() {
  // NFC-Chip lesen
  String nfcId = readNFCChip();
  
  // Spiel spielen
  float gameTime = playGame();
  int gameErrors = countErrors();
  
  // Daten senden
  sendGameData(nfcId, gameTime, gameErrors, "Mittel");
  
  delay(5000);
}
```

---

## 🎨 Design-Anpassungen

### Farben ändern

**Datei:** Alle `.html` Templates im `<style>` Bereich

```css
/* Hauptfarbe ändern */
background: linear-gradient(135deg, #b1cb21 0%, #7a9615 100%);
/* Zu: */
background: linear-gradient(135deg, #FF0000 0%, #AA0000 100%);

/* Button-Farbe ändern */
background: linear-gradient(135deg, #b1cb21 0%, #8fa619 100%);
```

### Logos ändern

**Dateien:**
- `static/logo.png` - Firmenlogo (rechts oben)
- `static/luh_logo.png` - Partner-Logo (links oben)

**Empfohlene Größe:** 150x150px, PNG mit Transparenz

### Texte anpassen

**Titel ändern:** In `templates/home.html`:
```html
<h1>🎮 Schlag den Roboter</h1>
<!-- Zu: -->
<h1>🎮 Ihr Titel hier</h1>
```

---

## 📚 Weiterführende Links

- **API-Dokumentation:** `API_DOKUMENTATION.md`
- **Flask Dokumentation:** https://flask.palletsprojects.com/
- **Jinja2 Template Engine:** https://jinja.palletsprojects.com/
- **Raspberry Pi Setup:** https://www.raspberrypi.org/documentation/

---

## 👥 Support & Kontakt

**Bei Fragen oder Problemen:**

1. ✅ Prüfe diese README
2. ✅ Prüfe `API_DOKUMENTATION.md`
3. ✅ Prüfe Server-Logs in der Konsole
4. ✅ Teste mit Beispiel-Befehlen aus dieser README

---

## 📄 Lizenz & Credits

**Entwickelt für:** Match-Event, Leibniz Universität Hannover  
**Version:** 1.0  
**Datum:** Oktober 2025  
**Framework:** Flask + Jinja2  
**Design:** Responsive CSS mit Firmenfarben

---

## 📚 Zusätzliche Dokumentation

- **[LEADERBOARD_RESET.md](LEADERBOARD_RESET.md)** - Leaderboards zurücksetzen (einzeln oder alle)
- **[NFC_INTEGRATION.md](NFC_INTEGRATION.md)** - Arduino NFC-Reader Integration
- **[LIVE_SCANNER_ANLEITUNG.md](LIVE_SCANNER_ANLEITUNG.md)** - Live NFC-Scanner Anleitung
- **[API_DOKUMENTATION.md](API_DOKUMENTATION.md)** - Vollständige API-Referenz
- **[TROUBLESHOOTING_QUICK.md](TROUBLESHOOTING_QUICK.md)** - Schnelle Problemlösungen
- **[TEST_UMBENENNEN.md](TEST_UMBENENNEN.md)** - Spieler umbenennen

---

## ✅ Quick Start Checkliste

- [ ] Python 3.8+ installiert
- [ ] Flask installiert (`pip install flask`)
- [ ] Server gestartet (`python server.py`)
- [ ] Browser geöffnet (`http://localhost:5000`)
- [ ] Admin-Panel geöffnet (`http://localhost:5000/admin`)
- [ ] Test-NFC-Chip hinzugefügt
- [ ] Test-Daten gesendet (siehe [Testing](#testing))
- [ ] Leaderboards geprüft
- [ ] Archiv-System getestet (Chip-Neuzuweisung)
- [ ] Urkunde generiert
- [ ] Reset-Funktion getestet (optional)

**🎉 Viel Erfolg mit dem Game Station Server!**
