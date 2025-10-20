# 🗑️ Leaderboard Reset Funktionalität

## Übersicht

Die Leaderboard-Reset-Funktion ermöglicht es, Spielstatistiken einzelner Spiele oder aller Spiele auf einmal zu löschen. Dies ist nützlich für:

- **Saisonende**: Neue Saison mit leeren Leaderboards starten
- **Testzwecke**: Nach Testläufen alles zurücksetzen
- **Fehlerbehebung**: Korrupte Daten entfernen
- **Events**: Zwischen verschiedenen Events wechseln

## ⚠️ Wichtige Sicherheitsvorkehrungen

### Automatisches Backup
- **Vor jedem Reset** wird automatisch ein Backup erstellt
- Backup-Dateiname: `leaderboard_backup_{spieltyp}_{timestamp}.json`
- Beispiel: `leaderboard_backup_heisser_draht_20251020_143022.json`
- Enthält: Alle Spieldaten + Archiv + Zeitstempel

### Bestätigungsdialoge
- **Einzelne Spiele**: 1 Bestätigung erforderlich
- **Alle Spiele**: 2 Bestätigungen erforderlich (doppelte Sicherheit)

### Wiederherstellung
Falls versehentlich gelöscht wurde:
```python
# Backup-Datei laden
import json
with open('leaderboard_backup_all_20251020_143022.json', 'r', encoding='utf-8') as f:
    backup = json.load(f)

# Daten wiederherstellen
with open('game_data.json', 'w', encoding='utf-8') as f:
    json.dump(backup['game_data'], f, indent=4, ensure_ascii=False)

with open('game_archive.json', 'w', encoding='utf-8') as f:
    json.dump(backup['game_archive'], f, indent=4, ensure_ascii=False)
```

## 🎮 Verwendung

### Im Admin-Panel (Web-Interface)

1. **Server starten**
   ```
   start_server.bat
   ```

2. **Admin-Panel öffnen**
   ```
   http://localhost:5000/admin
   ```

3. **Zum Reset-Bereich scrollen**
   - Abschnitt "🗑️ Leaderboard zurücksetzen"

4. **Leaderboard auswählen**
   - 🔥 **Heißer Draht**: Nur Heißer-Draht-Daten löschen
   - 🎲 **Vier Gewinnt**: Nur Vier-Gewinnt-Daten löschen
   - 🧩 **Puzzle**: Nur Puzzle-Daten löschen
   - 💣 **Alle Leaderboards**: ALLE Spieldaten löschen

5. **Reset-Button klicken**
   - Bestätigungsdialog erscheint
   - Warnung lesen
   - Bestätigen oder Abbrechen

6. **Erfolgsmeldung**
   - Zeigt Backup-Dateinamen
   - Seite lädt automatisch neu

### Via API (PowerShell)

#### Einzelnes Spiel zurücksetzen
```powershell
# Heißer Draht
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/reset_leaderboard" `
    -Method POST `
    -Body @{game_type='heisser_draht'} `
    -UseBasicParsing

# Vier Gewinnt
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/reset_leaderboard" `
    -Method POST `
    -Body @{game_type='vier_gewinnt'} `
    -UseBasicParsing

# Puzzle
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/reset_leaderboard" `
    -Method POST `
    -Body @{game_type='puzzle'} `
    -UseBasicParsing
```

#### Alle Spiele zurücksetzen
```powershell
Invoke-WebRequest -Uri "http://127.0.0.1:5000/admin/reset_leaderboard" `
    -Method POST `
    -Body @{game_type='all'} `
    -UseBasicParsing
```

### Mit Test-Script

```powershell
# Interaktives Test-Script ausführen
.\test_reset_leaderboard.ps1
```

Das Script testet:
- ✅ Einzelne Leaderboard-Resets
- ✅ Alle Leaderboards zurücksetzen (mit Bestätigung)
- ✅ Fehlerbehandlung (ungültige Parameter)
- ✅ Backup-Erstellung

## 📡 API Endpoint

### POST /admin/reset_leaderboard

**Parameter:**
- `game_type` (required): `'heisser_draht'`, `'vier_gewinnt'`, `'puzzle'`, oder `'all'`

**Response (Success):**
```json
{
    "success": true,
    "message": "Leaderboard 'Heißer Draht' wurde zurückgesetzt",
    "backup_file": "leaderboard_backup_heisser_draht_20251020_143022.json"
}
```

**Response (Error):**
```json
{
    "success": false,
    "message": "Ungültiger Spieltyp"
}
```

**HTTP Status Codes:**
- `200 OK`: Reset erfolgreich
- `400 Bad Request`: Ungültiger/fehlender Parameter

## 🔄 Was wird zurückgesetzt?

### Einzelnes Spiel (z.B. Heißer Draht)
```
✅ Gelöscht:
- Alle Spieldaten für "heisser_draht" in game_data.json
- Alle Spieldaten für "heisser_draht" in game_archive.json

❌ Bleibt erhalten:
- NFC-Chip-Zuordnungen (nfc_mapping.json)
- Spieldaten anderer Spiele
- Spieler-Namen
```

### Alle Spiele
```
✅ Gelöscht:
- Alle Spieldaten in game_data.json (alle 3 Spiele)
- Gesamtes game_archive.json

❌ Bleibt erhalten:
- NFC-Chip-Zuordnungen (nfc_mapping.json)
- Spieler-Namen
```

## 📊 Datenstruktur

### Vor Reset
```json
{
    "TEST001": {
        "heisser_draht": [
            {"name": "Spieler 1", "time": 12.5, "errors": 2}
        ],
        "vier_gewinnt": [
            {"name": "Spieler 1", "result": "won"}
        ],
        "puzzle": [
            {"name": "Spieler 1", "time": 45.3}
        ]
    }
}
```

### Nach Reset (heisser_draht)
```json
{
    "TEST001": {
        "heisser_draht": [],
        "vier_gewinnt": [
            {"name": "Spieler 1", "result": "won"}
        ],
        "puzzle": [
            {"name": "Spieler 1", "time": 45.3}
        ]
    }
}
```

### Nach Reset (all)
```json
{
    "TEST001": {
        "heisser_draht": [],
        "vier_gewinnt": [],
        "puzzle": []
    }
}
```

## 💾 Backup-Struktur

```json
{
    "timestamp": "20251020_143022",
    "game_type": "heisser_draht",
    "game_data": {
        "TEST001": {
            "heisser_draht": [...],
            "vier_gewinnt": [...],
            "puzzle": [...]
        }
    },
    "game_archive": [
        {
            "name": "Alter Spieler",
            "heisser_draht": [...],
            "vier_gewinnt": [...],
            "puzzle": [...],
            "archived_date": "2025-10-19T10:30:00"
        }
    ]
}
```

## 🛠️ Troubleshooting

### Problem: Reset-Button funktioniert nicht

**Lösung 1**: JavaScript-Konsole prüfen
```
F12 → Console → Fehlermeldungen suchen
```

**Lösung 2**: Server-Status prüfen
```powershell
# Ist der Server erreichbar?
Invoke-WebRequest -Uri "http://127.0.0.1:5000/" -UseBasicParsing
```

**Lösung 3**: Endpoint direkt testen
```powershell
.\test_reset_leaderboard.ps1
```

### Problem: Backup nicht gefunden

**Ursache**: Backup wird im Server-Verzeichnis gespeichert

**Lösung**:
```powershell
cd "C:\Users\maxim\OneDrive\Desktop\Uni\Match\Heißer Draht\Heißer Draht Server"
ls leaderboard_backup_*.json
```

### Problem: Versehentlich gelöscht

**Lösung**: Aus Backup wiederherstellen (siehe oben)

Oder manuell:
1. Neuestes Backup finden
2. Datei in Texteditor öffnen
3. `game_data` Inhalt kopieren → `game_data.json` einfügen
4. `game_archive` Inhalt kopieren → `game_archive.json` einfügen
5. Server neu starten

## 📋 Best Practices

### ✅ Empfohlen

1. **Vor Events/Turnieren**
   - Alte Daten archivieren
   - Leaderboards zurücksetzen
   - Frisch starten

2. **Nach Testzwecken**
   - Test-Daten entfernen
   - Saubere Produktionsdaten

3. **Regelmäßige Backups**
   - Vor großen Reset-Aktionen
   - Manuelles Backup zusätzlich erstellen

4. **Dokumentation**
   - Notieren wann und warum zurückgesetzt wurde
   - Backup-Dateien beschriften

### ❌ Vermeiden

1. **Nicht**: Reset ohne Backup-Überprüfung
2. **Nicht**: "Alle löschen" ohne doppelte Bestätigung
3. **Nicht**: Alte Backups sofort löschen (mindestens 7 Tage aufbewahren)
4. **Nicht**: Reset während laufendem Turnier

## 🔗 Siehe auch

- [README.md](README.md) - Haupt-Dokumentation
- [API_DOKUMENTATION.md](API_DOKUMENTATION.md) - Alle API-Endpoints
- [TROUBLESHOOTING_QUICK.md](TROUBLESHOOTING_QUICK.md) - Problemlösungen
- `test_reset_leaderboard.ps1` - Test-Script

## 📞 Support

Bei Problemen:
1. Test-Script ausführen: `.\test_reset_leaderboard.ps1`
2. Server-Logs prüfen (Terminal wo `start_server.bat` läuft)
3. Backup-Dateien überprüfen
4. Notfalls: Manuelle Wiederherstellung aus Backup
