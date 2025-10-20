import requests

url = "http://192.168.4.1:5000/update"
data = {
	"Corrected-Time": 150,
	"Errors": 2,
	"Time": 130,
	"Difficulty": "medium"
	}
	
try:
	response = requests.post(url, json=data)
	if response.status_code == 200:
		print("Erfolgreich", response.json())
	else:
		print("Fehler", response.status_code, response.text)
except requests.exceptions.RequestException as e:
	print("Verbindungsfehler", e)
