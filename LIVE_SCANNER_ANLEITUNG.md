# 🎯 Live NFC-Scanner - Anleitung

## So funktioniert der Live-Scanner im Admin-Panel

### 📋 Setup

**1. Server starten**
```powershell
python server.py
```
Server läuft auf: http://127.0.0.1:5000

**2. Arduino Bridge starten** (in neuem Terminal)
```powershell
python arduino_bridge.py
```

**3. Admin-Panel öffnen**
```
http://127.0.0.1:5000/admin
```

---

## 🚀 Verwendung

### Schritt-für-Schritt

1. **Im Admin-Panel auf "▶️ Scanner starten" klicken**
   - Status ändert sich zu: "🟢 Scanner aktiv"
   - System wartet auf NFC-Tags

2. **NFC-Tag an Arduino-Reader halten**
   - Arduino liest NFC-ID
   - `arduino_bridge.py` sendet ID an Server
   - Admin-Panel zeigt ID automatisch an!

3. **Zwei Szenarien:**

   **A) Chip ist NEU / Unbenannt:**
   ```
   ⚠️ Chip noch nicht benannt
   
   [Spielername eingeben] [✅ Namen zuweisen]
   ```
   - Namen eingeben
   - "✅ Namen zuweisen" klicken
   - Chip ist registriert!

   **B) Chip ist bereits registriert:**
   ```
   ✓ Chip bereits registriert: Max Mustermann
   Chip kann jetzt für Spiele verwendet werden.
   ```
   - Keine Aktion nötig
   - Chip ist bereit

4. **Scanner stoppen**
   - Klicke "⏸️ Scanner stoppen" wenn fertig

---

## 🔄 Workflow-Diagramm

```
┌────────────────────────────────────────────────┐
│  Admin-Panel: "Scanner starten" klicken        │
└───────────────────┬────────────────────────────┘
                    │
                    ▼
┌────────────────────────────────────────────────┐
│  Browser: Polling alle 500ms nach neuen Scans  │
│  GET /api/last_nfc_scan                        │
└───────────────────┬────────────────────────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  NFC-Tag an Reader   │
         └──────────┬───────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  Arduino liest UID   │
         └──────────┬───────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  arduino_bridge.py   │
         │  POST /api/nfc_scan  │
         └──────────┬───────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  server.py speichert │
         │  in last_scanned_nfc │
         └──────────┬───────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  Browser holt Daten  │
         │  per Polling         │
         └──────────┬───────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  Admin-Panel zeigt   │
         │  NFC-ID + Status     │
         └──────────────────────┘
```

---

## 💡 Technische Details

### Polling-Mechanismus

**Browser-Seite (JavaScript):**
```javascript
// Alle 500ms den Server fragen
setInterval(async () => {
    const response = await fetch('/api/last_nfc_scan');
    const data = await response.json();
    
    if (data.nfc_id && data.nfc_id !== lastScannedNfc) {
        // Neuer Scan! Anzeigen
        displayScan(data);
    }
}, 500);
```

**Server-Seite (Python):**
```python
# Global: Letzter Scan
last_scanned_nfc = {
    "nfc_id": None,
    "timestamp": None,
    "exists": False,
    "has_name": False,
    "player_name": None
}

# Bei jedem NFC-Scan aktualisieren
@app.route('/api/nfc_scan', methods=['POST'])
def nfc_scan():
    global last_scanned_nfc
    # ... Chip verarbeiten ...
    last_scanned_nfc = {
        "nfc_id": nfc_id,
        "timestamp": datetime.now().isoformat(),
        "exists": chip_exists,
        "has_name": chip_has_name,
        "player_name": nfc_mapping.get(nfc_id, "Unbenannt")
    }
    return jsonify(...)

# Browser kann letzten Scan abrufen
@app.route('/api/last_nfc_scan', methods=['GET'])
def get_last_nfc_scan():
    return jsonify(last_scanned_nfc)
```

---

## 🧪 Testen

### Test 1: Manueller API-Test

```powershell
# Terminal 1: Server läuft
# Terminal 2: Arduino Bridge läuft
# Terminal 3: API testen

# Simuliere NFC-Scan
$body = @{"nfc_id" = "TEST999"} | ConvertTo-Json
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body -ContentType "application/json"

# Prüfe letzten Scan
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/last_nfc_scan" -Method GET
```

**Erwartete Ausgabe:**
```json
{
  "nfc_id": "TEST999",
  "timestamp": "2025-10-20T15:30:00.123456",
  "exists": false,
  "has_name": false,
  "player_name": "Unbenannt"
}
```

### Test 2: Live-Scanner im Browser

1. Admin-Panel öffnen: http://127.0.0.1:5000/admin
2. Browser-Konsole öffnen: F12 → Console
3. Scanner starten
4. NFC-Tag scannen
5. In Konsole sollte erscheinen: "Neuer Scan: TEST999"

### Test 3: Vollständiger Workflow

```powershell
Write-Host "`n=== LIVE-SCANNER TEST ===`n"

# 1. Server prüfen
Write-Host "1. Server-Status..."
try {
    Invoke-RestMethod -Uri "http://127.0.0.1:5000/" -TimeoutSec 2 | Out-Null
    Write-Host "   ✓ Server läuft" -ForegroundColor Green
} catch {
    Write-Host "   ✗ Server läuft nicht!" -ForegroundColor Red
    exit
}

# 2. Admin-Panel öffnen
Write-Host "`n2. Öffne Admin-Panel..."
Start-Process "http://127.0.0.1:5000/admin"
Write-Host "   ✓ Browser geöffnet" -ForegroundColor Green

# 3. NFC-Scan simulieren
Write-Host "`n3. Simuliere NFC-Scan..."
$body = @{"nfc_id" = "LIVETEST123"} | ConvertTo-Json
$result = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body -ContentType "application/json"
Write-Host "   ✓ Scan gesendet: $($result.nfc_id)" -ForegroundColor Green

# 4. Prüfe ob im Browser sichtbar
Write-Host "`n4. Prüfe letzten Scan..."
$lastScan = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/last_nfc_scan"
Write-Host "   NFC-ID: $($lastScan.nfc_id)" -ForegroundColor Cyan
Write-Host "   Name: $($lastScan.player_name)" -ForegroundColor Cyan
Write-Host "   Zeitstempel: $($lastScan.timestamp)" -ForegroundColor Cyan

Write-Host "`n✓ Test erfolgreich!" -ForegroundColor Green
Write-Host "Im Browser sollte der Scan jetzt sichtbar sein."
Write-Host "Klicke im Admin-Panel auf 'Scanner starten' um Live-Updates zu sehen."
```

---

## 🔍 Troubleshooting

### Problem: Scanner zeigt nichts an

**1. Prüfe ob Scanner läuft:**
- Status sollte "🟢 Scanner aktiv" sein
- Falls nicht: Auf "▶️ Scanner starten" klicken

**2. Prüfe ob Arduino Bridge läuft:**
```powershell
# Sollte laufen
# 🔍 Warte auf NFC-Tags...
```

**3. Teste API manuell:**
```powershell
# Simuliere Scan
$body = @{"nfc_id" = "TEST123"} | ConvertTo-Json
Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body -ContentType "application/json"

# Prüfe ob im Admin-Panel erscheint
```

**4. Browser-Konsole prüfen:**
- F12 → Console
- Suche nach Fehlermeldungen
- "Polling-Fehler" → Server nicht erreichbar

### Problem: Alter Scan wird angezeigt

**Ursache:** Browser zeigt letzten Scan, auch wenn Scanner gestoppt war

**Lösung:** 
- Scanner stoppen und neu starten
- Oder: Seite neu laden (F5)

### Problem: Namen zuweisen funktioniert nicht

**Prüfe:**
1. ✅ NFC-ID korrekt angezeigt?
2. ✅ Name eingegeben?
3. ✅ "✅ Namen zuweisen" geklickt?
4. ✅ Warte 2 Sekunden → Seite lädt neu

**Falls nicht funktioniert:**
- Manuell in Tabelle unten zuweisen
- Oder: Admin-Panel neu laden

---

## 🎨 UI-Zustände

### Scanner gestoppt
```
⏸️ Scanner gestoppt

[▶️ Scanner starten]
```

### Scanner aktiv, warte auf Tag
```
🟢 Scanner aktiv - Halte NFC-Tag an den Leser...

[⏸️ Scanner stoppen]
```

### Tag gescannt - Unbenannt
```
🎯 NFC-Tag erkannt!
1A2B3C4D

⚠️ Chip noch nicht benannt

[Spielername eingeben] [✅ Namen zuweisen]
```

### Tag gescannt - Benannt
```
🎯 NFC-Tag erkannt!
1A2B3C4D

✓ Chip bereits registriert: Max Mustermann
Chip kann jetzt für Spiele verwendet werden.
```

---

## 📊 Performance

**Polling-Intervall:** 500ms (0.5 Sekunden)
- Gut für normale Verwendung
- Schnelle Reaktion auf neue Scans
- Minimale Server-Last

**Anpassung möglich:**
```javascript
// In templates/admin.html Zeile ~232
setInterval(async () => {
    // ...
}, 500);  // ← Hier ändern (in Millisekunden)

// 1000 = 1 Sekunde (langsamer, weniger Last)
// 250 = 0.25 Sekunden (schneller, mehr Last)
```

---

## ✅ Checkliste

- [ ] Server läuft (`python server.py`)
- [ ] Arduino Bridge läuft (`python arduino_bridge.py`)
- [ ] Admin-Panel geöffnet (`http://127.0.0.1:5000/admin`)
- [ ] Auf "▶️ Scanner starten" geklickt
- [ ] Status: "🟢 Scanner aktiv"
- [ ] NFC-Tag an Reader halten
- [ ] Admin-Panel zeigt Tag automatisch an
- [ ] Namen zuweisen
- [ ] ✅ System funktioniert!

---

**🎉 Viel Erfolg mit dem Live-Scanner!**
