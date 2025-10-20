"""
Arduino NFC-Reader Bridge
Liest NFC-IDs vom Arduino über serielle Schnittstelle und sendet sie an den Server

Voraussetzungen:
pip install pyserial requests

Verwendung:
python arduino_bridge.py
"""

import serial
import serial.tools.list_ports
import requests
import time
import sys

# Konfiguration
SERVER_URL = "http://127.0.0.1:5000/api/nfc_scan"
BAUD_RATE = 9600
TIMEOUT = 1

def find_arduino_port():
    """Sucht automatisch den Arduino-Port"""
    ports = serial.tools.list_ports.comports()
    
    print("Verfügbare serielle Ports:")
    for i, port in enumerate(ports, 1):
        print(f"  {i}. {port.device} - {port.description}")
    
    if not ports:
        print("❌ Keine seriellen Ports gefunden!")
        return None
    
    # Arduino automatisch erkennen
    for port in ports:
        if 'Arduino' in port.description or 'CH340' in port.description or 'USB' in port.description:
            print(f"\n✓ Arduino gefunden auf: {port.device}")
            return port.device
    
    # Manuell auswählen
    print("\nArduino nicht automatisch gefunden.")
    try:
        choice = int(input("Wähle Port-Nummer: "))
        if 1 <= choice <= len(ports):
            return ports[choice-1].device
    except:
        pass
    
    return None

def send_to_server(nfc_id):
    """Sendet NFC-ID an den Server"""
    try:
        response = requests.post(
            SERVER_URL,
            json={"nfc_id": nfc_id},
            timeout=5
        )
        
        if response.status_code == 200:
            data = response.json()
            print(f"  ✓ Server-Antwort: {data.get('status')}")
            
            if data.get('has_name'):
                print(f"  👤 Spieler: {data.get('player_name')}")
            else:
                print(f"  ⚠️  Chip noch nicht benannt")
            
            return True
        else:
            print(f"  ❌ Server-Fehler: {response.status_code}")
            return False
            
    except requests.exceptions.RequestException as e:
        print(f"  ❌ Verbindungsfehler: {e}")
        return False

def main():
    print("=" * 60)
    print("🎮 Arduino NFC-Reader Bridge")
    print("=" * 60)
    
    # Arduino-Port finden
    port = find_arduino_port()
    if not port:
        print("\n❌ Kein Port ausgewählt. Programm wird beendet.")
        sys.exit(1)
    
    # Serielle Verbindung öffnen
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=TIMEOUT)
        print(f"\n✓ Verbunden mit {port} @ {BAUD_RATE} baud")
        print("\n🔍 Warte auf NFC-Tags...")
        print("   (Drücke Ctrl+C zum Beenden)\n")
        
        # Warte kurz bis Arduino bereit ist
        time.sleep(2)
        ser.reset_input_buffer()
        
        last_nfc_id = None
        last_scan_time = 0
        
        while True:
            try:
                # Lese Zeile vom Arduino
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8', errors='ignore').strip()
                    
                    # Debug: Zeige alle Zeilen (optional auskommentieren)
                    # print(f"[DEBUG] {line}")
                    
                    # Suche nach NFC_ID: Marker (für seriellen Modus)
                    if line.startswith("NFC_ID:"):
                        nfc_id = line.replace("NFC_ID:", "").strip()
                        
                        # Cooldown: Verhindere mehrfaches Scannen derselben Karte
                        current_time = time.time()
                        if nfc_id == last_nfc_id and (current_time - last_scan_time) < 3:
                            continue
                        
                        print("-" * 60)
                        print(f"📡 NFC-Tag erkannt: {nfc_id}")
                        
                        # An Server senden
                        success = send_to_server(nfc_id)
                        
                        if success:
                            last_nfc_id = nfc_id
                            last_scan_time = current_time
                        
                        print("-" * 60 + "\n")
                    
                    # Alternative: Suche nach "NFC-Tag erkannt:" im Output
                    elif "NFC-Tag erkannt:" in line:
                        # Extrahiere NFC-ID aus: "✓ NFC-Tag erkannt: 1A2B3C4D"
                        parts = line.split(":")
                        if len(parts) >= 2:
                            nfc_id = parts[-1].strip()
                            
                            current_time = time.time()
                            if nfc_id == last_nfc_id and (current_time - last_scan_time) < 3:
                                continue
                            
                            print("-" * 60)
                            print(f"📡 NFC-Tag erkannt: {nfc_id}")
                            send_to_server(nfc_id)
                            last_nfc_id = nfc_id
                            last_scan_time = current_time
                            print("-" * 60 + "\n")
                
                time.sleep(0.1)
                
            except KeyboardInterrupt:
                print("\n\n⏸️  Programm beendet durch Benutzer")
                break
            except Exception as e:
                print(f"❌ Fehler beim Lesen: {e}")
                time.sleep(1)
        
    except serial.SerialException as e:
        print(f"\n❌ Fehler beim Öffnen von {port}: {e}")
        print("\nMögliche Lösungen:")
        print("  1. Prüfe ob Arduino angeschlossen ist")
        print("  2. Prüfe ob der richtige Port ausgewählt wurde")
        print("  3. Schließe Arduino IDE (Serial Monitor blockiert Port)")
        sys.exit(1)
    
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("✓ Serielle Verbindung geschlossen")

if __name__ == "__main__":
    main()
