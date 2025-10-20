@echo off
chcp 65001 >nul
echo ═══════════════════════════════════════════════════════════
echo   🎮 Game Station Server - NFC-Reader Setup
echo ═══════════════════════════════════════════════════════════
echo.

REM Prüfe Python-Installation
python --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Python nicht gefunden!
    echo    Bitte Python 3.8+ installieren: https://www.python.org/
    pause
    exit /b 1
)

echo ✓ Python gefunden
echo.

REM Installiere Abhängigkeiten
echo 📦 Installiere Python-Pakete...
echo.
pip install flask pyserial requests --quiet --disable-pip-version-check

if errorlevel 1 (
    echo ❌ Installation fehlgeschlagen
    pause
    exit /b 1
)

echo.
echo ✓ Alle Pakete installiert
echo.

REM Starte Services
echo ═══════════════════════════════════════════════════════════
echo   Starte Game Station Server...
echo ═══════════════════════════════════════════════════════════
echo.
echo Server läuft auf: http://127.0.0.1:5000
echo Admin-Panel:      http://127.0.0.1:5000/admin
echo.
echo Drücke Ctrl+C zum Beenden
echo.

python server.py
