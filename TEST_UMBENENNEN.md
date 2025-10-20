# 🧪 Test: Live-Scanner mit Umbenennen-Funktion

## Test-Szenario

### Vorbereitung
```powershell
# 1. Server starten
python server.py

# 2. Admin-Panel öffnen
Start-Process "http://127.0.0.1:5000/admin"
```

---

## Test 1: Neuen Chip scannen und Namen zuweisen

**Schritte:**
1. Admin-Panel öffnen
2. "▶️ Scanner starten" klicken
3. NFC-Tag scannen (oder simulieren):
   ```powershell
   $body = @{"nfc_id" = "TEST001"} | ConvertTo-Json
   Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
       -Method POST -Body $body -ContentType "application/json"
   ```

**Erwartetes Verhalten:**
```
🎯 NFC-Tag erkannt!
TEST001

⚠️ Chip noch nicht benannt

[Spielername eingeben] [✅ Namen zuweisen]
```

4. Namen eingeben: "Max Mustermann"
5. "✅ Namen zuweisen" klicken

**Erwartetes Ergebnis:**
```
✅ Name erfolgreich zugewiesen: Max Mustermann
```

---

## Test 2: Registrierten Chip scannen und umbenennen

**Schritte:**
1. Scanner ist aktiv
2. Denselben Chip erneut scannen:
   ```powershell
   $body = @{"nfc_id" = "TEST001"} | ConvertTo-Json
   Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
       -Method POST -Body $body -ContentType "application/json"
   ```

**Erwartetes Verhalten:**
```
🎯 NFC-Tag erkannt!
TEST001

✓ Chip bereits registriert: Max Mustermann
Chip kann jetzt für Spiele verwendet werden.

💡 Namen ändern?
Gib einen neuen Namen ein und klicke auf "Namen ändern"

[Max Mustermann] [✏️ Namen ändern]
    ↑ Vorausgefüllt + markiert
```

3. Namen ändern zu: "Anna Schmidt"
4. "✏️ Namen ändern" klicken

**Erwartetes Ergebnis:**
```
✅ Name erfolgreich geändert: Anna Schmidt
```

---

## Test 3: Vollständiger PowerShell-Test

```powershell
Write-Host "`n=== TEST: LIVE-SCANNER MIT UMBENENNEN ===`n" -ForegroundColor Cyan

# Test 1: Neuer Chip
Write-Host "Test 1: Neuen Chip scannen..." -ForegroundColor Yellow
$body1 = @{"nfc_id" = "RENAME_TEST"} | ConvertTo-Json
$result1 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body1 -ContentType "application/json"

Write-Host "  NFC-ID: $($result1.nfc_id)" -ForegroundColor White
Write-Host "  Hat Namen: $($result1.has_name)" -ForegroundColor White
Write-Host "  Name: $($result1.player_name)" -ForegroundColor White

if ($result1.has_name -eq $false) {
    Write-Host "  ✓ Korrekt: Chip ist unbenannt" -ForegroundColor Green
} else {
    Write-Host "  ✗ Fehler: Chip sollte unbenannt sein" -ForegroundColor Red
}

# Namen zuweisen
Write-Host "`nNamen zuweisen: 'Test Spieler'..." -ForegroundColor Yellow
$assignForm = @{nfc_id='RENAME_TEST'; name='Test Spieler'}
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/assign_name" `
    -Method POST -Body $assignForm -UseBasicParsing | Out-Null
Write-Host "  ✓ Name zugewiesen" -ForegroundColor Green

# Test 2: Erneut scannen (jetzt mit Namen)
Write-Host "`nTest 2: Chip erneut scannen (hat jetzt Namen)..." -ForegroundColor Yellow
$result2 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body1 -ContentType "application/json"

Write-Host "  NFC-ID: $($result2.nfc_id)" -ForegroundColor White
Write-Host "  Hat Namen: $($result2.has_name)" -ForegroundColor White
Write-Host "  Name: $($result2.player_name)" -ForegroundColor White

if ($result2.has_name -eq $true -and $result2.player_name -eq "Test Spieler") {
    Write-Host "  ✓ Korrekt: Chip hat Namen" -ForegroundColor Green
} else {
    Write-Host "  ✗ Fehler: Chip sollte Namen haben" -ForegroundColor Red
}

# Namen ändern
Write-Host "`nNamen ändern zu: 'Umbenannter Spieler'..." -ForegroundColor Yellow
$renameForm = @{nfc_id='RENAME_TEST'; name='Umbenannter Spieler'}
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/assign_name" `
    -Method POST -Body $renameForm -UseBasicParsing | Out-Null
Write-Host "  ✓ Name geändert" -ForegroundColor Green

# Test 3: Prüfe ob Name geändert wurde
Write-Host "`nTest 3: Chip erneut scannen (Name sollte geändert sein)..." -ForegroundColor Yellow
$result3 = Invoke-RestMethod -Uri "http://127.0.0.1:5000/api/nfc_scan" `
    -Method POST -Body $body1 -ContentType "application/json"

Write-Host "  NFC-ID: $($result3.nfc_id)" -ForegroundColor White
Write-Host "  Hat Namen: $($result3.has_name)" -ForegroundColor White
Write-Host "  Name: $($result3.player_name)" -ForegroundColor White

if ($result3.player_name -eq "Umbenannter Spieler") {
    Write-Host "  ✓ Korrekt: Name wurde erfolgreich geändert" -ForegroundColor Green
} else {
    Write-Host "  ✗ Fehler: Name wurde nicht geändert" -ForegroundColor Red
}

Write-Host "`n=== ALLE TESTS ABGESCHLOSSEN ===`n" -ForegroundColor Cyan
Write-Host "Öffne Admin-Panel und klicke 'Scanner starten' um Live-Updates zu sehen:" -ForegroundColor White
Write-Host "http://127.0.0.1:5000/admin" -ForegroundColor Cyan
```

---

## Erwartete Ausgabe

```
=== TEST: LIVE-SCANNER MIT UMBENENNEN ===

Test 1: Neuen Chip scannen...
  NFC-ID: RENAME_TEST
  Hat Namen: False
  Name: Unbenannt
  ✓ Korrekt: Chip ist unbenannt

Namen zuweisen: 'Test Spieler'...
  ✓ Name zugewiesen

Test 2: Chip erneut scannen (hat jetzt Namen)...
  NFC-ID: RENAME_TEST
  Hat Namen: True
  Name: Test Spieler
  ✓ Korrekt: Chip hat Namen

Namen ändern zu: 'Umbenannter Spieler'...
  ✓ Name geändert

Test 3: Chip erneut scannen (Name sollte geändert sein)...
  NFC-ID: RENAME_TEST
  Hat Namen: True
  Name: Umbenannter Spieler
  ✓ Korrekt: Name wurde erfolgreich geändert

=== ALLE TESTS ABGESCHLOSSEN ===

Öffne Admin-Panel und klicke 'Scanner starten' um Live-Updates zu sehen:
http://127.0.0.1:5000/admin
```

---

## UI-Vergleich

### Vorher (nur Zuweisen)
```
Unbenannter Chip:
[Spielername eingeben] [✅ Namen zuweisen]

Benannter Chip:
✓ Chip bereits registriert: Max
Chip kann jetzt für Spiele verwendet werden.
[KEINE BEARBEITUNGSOPTION]
```

### Nachher (Zuweisen + Umbenennen)
```
Unbenannter Chip:
[Spielername eingeben] [✅ Namen zuweisen]

Benannter Chip:
✓ Chip bereits registriert: Max
Chip kann jetzt für Spiele verwendet werden.

💡 Namen ändern?
[Max] [✏️ Namen ändern]
 ↑ Vorausgefüllt + markiert zum Bearbeiten
```

---

## Features

✅ **Neuen Chip registrieren**
- Scanner erkennt unbenannten Chip
- Feld ist leer
- Button: "✅ Namen zuweisen"

✅ **Registrierten Chip umbenennen**
- Scanner erkennt benannten Chip
- Aktueller Name wird angezeigt
- Feld ist vorausgefüllt
- Text ist markiert (schnell überschreibbar)
- Button: "✏️ Namen ändern"
- Hinweis: "💡 Namen ändern?"

✅ **Erfolgs-Feedback**
- "✅ Name erfolgreich zugewiesen" (neu)
- "✅ Name erfolgreich geändert" (umbenennen)

---

## Tastatur-Shortcuts

**Neuen Chip:**
1. Scanner startet → Tag scannen
2. Fokus ist automatisch im Namensfeld
3. Namen tippen
4. Enter drücken → Fertig!

**Umbenennen:**
1. Scanner startet → Tag scannen
2. Fokus ist im Namensfeld (alter Name markiert)
3. Neuen Namen tippen (überschreibt automatisch)
4. Enter drücken → Fertig!

**Alternativ:**
- Tab-Taste → Zum Button springen
- Enter → Absenden

---

## Hinweise

**Auto-Reload:**
Nach erfolgreicher Zuweisung/Änderung lädt die Seite nach 2 Sekunden neu.
→ Chip erscheint in der Tabelle unten mit neuem Namen

**Duplikat-Schutz:**
Derselbe Chip kann innerhalb von 5 Sekunden nicht erneut gescannt werden.
→ Verhindert versehentliche Mehrfach-Scans

**Archivierung:**
Wenn ein Chip mit Spielen neu zugewiesen wird, wandern die alten Daten ins Archiv.
→ Alter Name bleibt in Leaderboards erhalten
