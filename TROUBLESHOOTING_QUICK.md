# 🔧 Schnelle Problembehebung

## ❌ "Port COM3 blockiert" / "Zugriff verweigert"

### Problem
```
❌ Fehler beim Öffnen von COM3: PermissionError(13, 'Zugriff verweigert')
```

### Ursache
Ein anderes Programm verwendet bereits den COM-Port.

### Lösung

**1. Arduino IDE Serial Monitor schließen**
```
Arduino IDE → Oben rechts "X" beim Serial Monitor
```
Dies ist die häufigste Ursache! ✅

**2. Andere Programme schließen**
- PuTTY
- Tera Term
- HTerm
- Andere Arduino Bridge Instanzen

**3. Arduino neu anschließen**
```powershell
# 1. USB-Kabel abziehen
# 2. 5 Sekunden warten
# 3. USB-Kabel wieder anschließen
# 4. arduino_bridge.py neu starten
python arduino_bridge.py
```

**4. Port-Nummer prüfen**
```powershell
# Windows: Geräte-Manager öffnen
devmgmt.msc

# Suche: "Anschlüsse (COM & LPT)"
# Finde "Arduino" oder "USB Serial"
# Notiere COM-Nummer (z.B. COM3, COM5, etc.)
```

**5. Als letztes Mittel: Computer neu starten**

---

## ❌ "Keine seriellen Ports gefunden"

### Lösung

**1. Arduino anschließen**
- USB-Kabel fest einstecken
- Rote LED auf Arduino sollte leuchten

**2. Treiber installieren**

**CH340 Treiber** (für Arduino Nano Clone):
- Download: http://www.wch.cn/downloads/CH341SER_EXE.html
- Installieren und PC neu starten

**FTDI Treiber** (für Original Arduino):
- Download: https://ftdichip.com/drivers/
- Installieren und PC neu starten

**3. Anderen USB-Port versuchen**
- USB 2.0 Ports bevorzugen
- Direkt am PC, nicht über USB-Hub

---

## ❌ "requests.py" Konflikt (Behoben)

### Problem
```
AttributeError: partially initialized module 'requests' has no attribute 'post'
```

### Lösung
✅ **Bereits behoben!** Die Datei wurde umbenannt zu `test_requests.py`

Falls das Problem erneut auftritt:
```powershell
# Prüfe ob requests.py existiert
Get-ChildItem "requests.py"

# Falls ja, umbenennen
Rename-Item "requests.py" "test_requests_backup.py"
```

---

## ✅ Erfolgreicher Start sieht so aus:

```
============================================================
🎮 Arduino NFC-Reader Bridge
============================================================
Verfügbare serielle Ports:
  1. COM3 - Arduino Uno (Arduino Uno)

✓ Arduino gefunden auf: COM3
✓ Verbunden mit COM3 @ 9600 baud

🔍 Warte auf NFC-Tags...
   (Drücke Ctrl+C zum Beenden)
```

**Dann bist du bereit!** NFC-Tag an Reader halten und scannen. 🎉

---

## 🆘 Immer noch Probleme?

1. **Teste Arduino direkt:**
   ```
   Arduino IDE → Tools → Serieller Monitor
   Baudrate: 9600
   NFC-Tag scannen → UID sollte erscheinen
   ```

2. **Prüfe Verkabelung:**
   - Siehe `NFC_INTEGRATION.md` → Hardware-Setup
   - Alle 7 Pins korrekt verbunden?
   - 3.3V (nicht 5V!)

3. **Server läuft?**
   ```powershell
   # Test
   curl http://127.0.0.1:5000
   
   # Falls nicht, starten
   python server.py
   ```

---

**Weitere Hilfe:** Siehe `NFC_INTEGRATION.md` → Troubleshooting-Sektion
