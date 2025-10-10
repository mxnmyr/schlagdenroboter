import requests

# Server-URL
url = "http://localhost:5000/update"

# Daten für das Leaderboard
data = {
    "Corrected-Time": 120,
    "errors": 2,
    "Difficulty": "medium"
}

# POST-Anfrage senden
response = requests.post(url, json=data)

# Server-Antwort ausgeben
print(f"Status Code: {response.status_code}")
print(f"Antwort: {response.text}")
