# 📡 API Dokumentation - Game Station Server

Vollständige API-Referenz für alle Endpunkte des Game Station Servers.

---

## 📋 Übersicht

| Endpunkt | Methode | Beschreibung |
|----------|---------|--------------|
| `/api/nfc_scan` | POST | NFC-Tag scannen und registrieren |
| `/api/heisser_draht` | POST | Heißer Draht Ergebnis senden |
| `/api/vier_gewinnt` | POST | Vier Gewinnt Ergebnis senden |
| `/api/puzzle` | POST | Puzzle Ergebnis senden |

---

## 🎯 API-Endpunkte

### 1. NFC-Scan (NEU!)

**Endpunkt:** `POST /api/nfc_scan`

**Beschreibung:** Registriert einen NFC-Chip im System. Wird automatisch vom Arduino NFC-Reader aufgerufen.

**Request Body:**
```json
{
  "nfc_id": "1A2B3C4D"
}
```

**Response (Neuer Chip):**
```json
{
  "status": "success",
  "nfc_id": "1A2B3C4D",
  "exists": false,
  "has_name": false,
  "player_name": "Unbenannt"
}
```

**Response (Bestehender Chip):**
```json
{
  "status": "success",
  "nfc_id": "1A2B3C4D",
  "exists": true,
  "has_name": true,
  "player_name": "Max Mustermann"
}
```

**PowerShell Beispiel:**
```powershell
$body = @{"nfc_id" = "1A2B3C4D"} | ConvertTo-Json
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body -ContentType "application/json"
```

**cURL Beispiel:**
```bash
curl -X POST http://127.0.0.1:5000/api/nfc_scan \
  -H "Content-Type: application/json" \
  -d '{"nfc_id":"1A2B3C4D"}'
```

**Arduino Beispiel:**
```cpp
HTTPClient http;
http.begin("http://192.168.1.100:5000/api/nfc_scan");
http.addHeader("Content-Type", "application/json");

String jsonPayload = "{\"nfc_id\":\"" + nfcId + "\"}";
int httpCode = http.POST(jsonPayload);

if (httpCode == 200) {
  String response = http.getString();
  Serial.println(response);
}
http.end();
```

---

### 2. Heißer Draht

**Endpunkt:** `POST /api/heisser_draht`

**Request Body:**
```json
{
  "nfc_id": "1A2B3C4D",
  "time": 12.50,
  "errors": 2,
  "difficulty": "Mittel"
}
```

**Parameter:**
- `nfc_id` (string, required): NFC-Chip ID
- `time` (float, required): Zeit in Sekunden
- `errors` (int, required): Anzahl Fehler
- `difficulty` (string, required): "Leicht", "Mittel", oder "Schwer"

**Response:**
```json
{
  "status": "success",
  "player_name": "Max Mustermann"
}
```

**PowerShell Beispiel:**
```powershell
$body = @{
    "nfc_id" = "1A2B3C4D"
    "time" = 12.50
    "errors" = 2
    "difficulty" = "Mittel"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/heisser_draht" `
    -Method POST -Body $body -ContentType "application/json"
```

**ESP32 Beispiel:**
```cpp
StaticJsonDocument<200> doc;
doc["nfc_id"] = "1A2B3C4D";
doc["time"] = 12.50;
doc["errors"] = 2;
doc["difficulty"] = "Mittel";

String jsonString;
serializeJson(doc, jsonString);

HTTPClient http;
http.begin("http://192.168.1.100:5000/api/heisser_draht");
http.addHeader("Content-Type", "application/json");
http.POST(jsonString);
http.end();
```

---

### 3. Vier Gewinnt

**Endpunkt:** `POST /api/vier_gewinnt`

**Request Body:**
```json
{
  "nfc_id": "1A2B3C4D",
  "moves": 25,
  "difficulty": "Mittel"
}
```

**Parameter:**
- `nfc_id` (string, required): NFC-Chip ID
- `moves` (integer, required): Anzahl der Züge bis zum Spielende
- `difficulty` (string, optional): "Leicht", "Mittel", oder "Schwer" (Standard: "Mittel")

**Response:**
```json
{
  "status": "success",
  "player_name": "Max Mustermann",
  "moves": 25
}
```

**PowerShell Beispiel:**
```powershell
$body = @{
    "nfc_id" = "1A2B3C4D"
    "moves" = 25
    "difficulty" = "Mittel"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/vier_gewinnt" `
    -Method POST -Body $body -ContentType "application/json"
```

**Hinweise:**
- Niedrigere Zugzahl = besseres Ergebnis
- Leaderboard zeigt Spieler mit wenigsten Zügen oben
- Durchschnittszüge und beste Züge werden berechnet

---

### 4. Puzzle

**Endpunkt:** `POST /api/puzzle`

**Request Body:**
```json
{
  "nfc_id": "1A2B3C4D",
  "time": 38.75,
  "difficulty": "Schwer"
}
```

**Parameter:**
- `nfc_id` (string, required): NFC-Chip ID
- `time` (float, required): Benötigte Zeit in Sekunden
- `difficulty` (string, optional): "Leicht", "Mittel", oder "Schwer" (Standard: "Mittel")

**Parameter:**
- `nfc_id` (string, required): NFC-Chip ID
- `time` (float, required): Zeit in Sekunden
- `difficulty` (string, required): "Leicht", "Mittel", oder "Schwer"

**Response:**
```json
{
  "status": "success",
  "player_name": "Max Mustermann"
}
```

**PowerShell Beispiel:**
```powershell
$body = @{
    "nfc_id" = "1A2B3C4D"
    "time" = 38.75
    "difficulty" = "Schwer"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/puzzle" `
    -Method POST -Body $body -ContentType "application/json"
```

---

## 🧪 Vollständiges Test-Skript

### NFC-Workflow komplett testen

```powershell
Write-Host "`n=== KOMPLETTER NFC-WORKFLOW TEST ===`n"

# 1. NFC-Chip scannen (simuliert)
Write-Host "Schritt 1: NFC-Chip scannen"
$nfcScan = @{"nfc_id" = "TEST123"} | ConvertTo-Json
$result = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $nfcScan -ContentType "application/json"
Write-Host "  Status: $($result.status)"
Write-Host "  Chip existiert: $($result.exists)"
Write-Host "  Hat Namen: $($result.has_name)"
Write-Host ""

# 2. Namen zuweisen (über Web-Interface nötig)
Write-Host "Schritt 2: Namen zuweisen"
Write-Host "  → Öffne Admin-Panel: http://127.0.0.1:5000/admin"
Write-Host "  → Gib Namen ein und klicke 'Zuweisen'"
Write-Host ""
Read-Host "Drücke Enter wenn Namen zugewiesen wurde"

# 3. Heißer Draht spielen
Write-Host "Schritt 3: Heißer Draht"
$hd = @{
    "nfc_id" = "TEST123"
    "time" = 15.30
    "errors" = 1
    "difficulty" = "Mittel"
} | ConvertTo-Json
$result = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/heisser_draht" `
    -Method POST -Body $hd -ContentType "application/json"
Write-Host "  ✓ Heißer Draht: $($result.player_name)"
Write-Host ""

# 4. Vier Gewinnt spielen
Write-Host "Schritt 4: Vier Gewinnt"
$vg = @{
    "nfc_id" = "TEST123"
    "result" = "won"
    "difficulty" = "Schwer"
} | ConvertTo-Json
$result = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/vier_gewinnt" `
    -Method POST -Body $vg -ContentType "application/json"
Write-Host "  ✓ Vier Gewinnt: $($result.player_name)"
Write-Host ""

# 5. Puzzle spielen
Write-Host "Schritt 5: Puzzle"
$pz = @{
    "nfc_id" = "TEST123"
    "time" = 42.10
    "difficulty" = "Leicht"
} | ConvertTo-Json
$result = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/puzzle" `
    -Method POST -Body $pz -ContentType "application/json"
Write-Host "  ✓ Puzzle: $($result.player_name)"
Write-Host ""

Write-Host "=== TEST ABGESCHLOSSEN ==="
Write-Host "Alle 3 Spiele absolviert!"
Write-Host "Leaderboards: http://127.0.0.1:5000"
Write-Host "Urkunde: http://127.0.0.1:5000/admin"
```

---

## 🔄 Workflow-Diagramm

```
┌───────────────┐
│  NFC-Tag      │
│  scannen      │
└───────┬───────┘
        │
        ▼
┌────────────────────────┐
│ POST /api/nfc_scan     │
│ {"nfc_id": "..."}      │
└───────┬────────────────┘
        │
        ├─── Neu ────────▶ Chip erstellt, "Unbenannt"
        │
        └─── Bekannt ────▶ Spielername anzeigen
        
        ▼
┌────────────────────────┐
│  Admin-Panel           │
│  Namen zuweisen        │
└───────┬────────────────┘
        │
        ▼
┌────────────────────────┐
│  Spiele spielen        │
│  - Heißer Draht        │
│  - Vier Gewinnt        │
│  - Puzzle              │
└───────┬────────────────┘
        │
        ▼
┌────────────────────────┐
│  Leaderboards          │
│  aktualisiert          │
└────────────────────────┘
```

---

## ⚠️ Fehlerbehandlung

### Ungültige NFC-ID

**Request:**
```json
{
  "nfc_id": ""
}
```

**Response (400):**
```json
{
  "status": "error",
  "message": "Keine NFC-ID empfangen"
}
```

### Unbekannter NFC-Chip bei Spiel

**Verhalten:** 
- Chip wird automatisch erstellt
- Spiel wird mit Namen "Unbenannt" gespeichert
- Im Admin-Panel Namen nachträglich zuweisen möglich

### Server nicht erreichbar

**Arduino/ESP Verhalten:**
- Fehler in Serial Monitor anzeigen
- Scan wiederholen bei nächstem Tag
- Keine Daten gehen verloren

---

## 🔐 Sicherheit

### Produktions-Empfehlungen

1. **Debug-Modus deaktivieren:**
   ```python
   # In server.py Zeile 479:
   app.run(host='0.0.0.0', port=5000, debug=False)
   ```

2. **Firewall konfigurieren:**
   - Port 5000 nur im lokalen Netzwerk öffnen
   - Kein direkter Internet-Zugang

3. **Backup-Strategie:**
   ```powershell
   # Automatisches Backup alle 24h
   $date = Get-Date -Format "yyyy-MM-dd"
   Copy-Item "*.json" -Destination "backup_$date/"
   ```

---

## 📊 Rate Limits

Aktuell keine Rate Limits implementiert.

**Empfehlungen für Produktion:**
- Max. 1 Request/Sekunde pro NFC-ID
- Cooldown von 2-3 Sekunden zwischen Scans (bereits in Arduino-Code)
- Server kann ~100 gleichzeitige Requests verarbeiten

---

## 🆘 Support

Bei API-Problemen:

1. ✅ Prüfe Server-Logs (Konsole)
2. ✅ Teste mit PowerShell-Befehlen
3. ✅ Prüfe JSON-Format
4. ✅ Prüfe Netzwerk-Verbindung

**Häufige Fehler:**

| Fehler | Ursache | Lösung |
|--------|---------|--------|
| Connection refused | Server läuft nicht | `python server.py` |
| Invalid JSON | Falsches Format | JSON-Validator verwenden |
| 404 Not Found | Falsche URL | URL-Endpunkt prüfen |
| Timeout | Netzwerk-Problem | IP-Adresse/Firewall prüfen |

---

## 📚 Beispiel-Integration

### Python Client

```python
import requests
import json

def send_nfc_scan(nfc_id, server_url="http://127.0.0.1:5000"):
    endpoint = f"{server_url}/api/nfc_scan"
    data = {"nfc_id": nfc_id}
    
    response = requests.post(endpoint, json=data)
    
    if response.status_code == 200:
        result = response.json()
        print(f"✓ Chip: {result['nfc_id']}")
        print(f"  Name: {result['player_name']}")
        return result
    else:
        print(f"✗ Fehler: {response.status_code}")
        return None

# Verwendung
send_nfc_scan("1A2B3C4D")
```

### JavaScript/Node.js Client

```javascript
const axios = require('axios');

async function sendNfcScan(nfcId, serverUrl = 'http://127.0.0.1:5000') {
  try {
    const response = await axios.post(`${serverUrl}/api/nfc_scan`, {
      nfc_id: nfcId
    });
    
    console.log('✓ Chip:', response.data.nfc_id);
    console.log('  Name:', response.data.player_name);
    return response.data;
  } catch (error) {
    console.error('✗ Fehler:', error.message);
    return null;
  }
}

// Verwendung
sendNfcScan('1A2B3C4D');
```

---

**Letzte Aktualisierung:** Oktober 2025  
**Version:** 2.0 (mit NFC-Integration)
