@echo off
chcp 65001 >nul
echo ═══════════════════════════════════════════════════════════
echo   📡 Arduino NFC-Reader Bridge
echo ═══════════════════════════════════════════════════════════
echo.

REM Prüfe ob Server läuft
curl -s http://127.0.0.1:5000 >nul 2>&1
if errorlevel 1 (
    echo ⚠️  WARNUNG: Server scheint nicht zu laufen!
    echo    Bitte starte zuerst: start_server.bat
    echo.
    echo Fortfahren? (J/N^)
    set /p continue=
    if /i not "%continue%"=="J" exit /b 1
)

echo ✓ Server erreichbar
echo.

REM Prüfe pyserial
python -c "import serial" >nul 2>&1
if errorlevel 1 (
    echo ❌ pyserial nicht installiert!
    echo    Installiere mit: pip install pyserial
    pause
    exit /b 1
)

echo ═══════════════════════════════════════════════════════════
echo   Starte Arduino-Bridge...
echo ═══════════════════════════════════════════════════════════
echo.
echo 1. Verbinde Arduino per USB
echo 2. Schließe Arduino IDE Serial Monitor
echo 3. Wähle COM-Port wenn gefragt
echo.
echo Drücke eine Taste um fortzufahren...
pause >nul

python arduino_bridge.py
