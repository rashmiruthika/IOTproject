
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Turkey";
const char* password = "mathematics";

ESP8266WebServer server(80);

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Room Temperature Fan Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #e2e8eb;
            color: #333;
            margin: 0;
            padding: 0;
        }
        .container {
            max-width: 800px;
            margin: 20px auto;
            padding: 20px;
            background-color: rgb(162, 211, 237);
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            position: relative; 
        }
        .navbar {
            overflow: hidden;
            background-color: #1553af;
            color: white;
            padding: 15px 20px;
            display: flex;
            justify-content: flex-end; 
            align-items: center;
            border-radius: 8px 8px 0 0; 
        }
        .navbar a {
            color: white;
            text-decoration: none;
            font-size: 18px; 
            margin-left: 20px; 
            transition: background-color 0.3s ease; 
            padding: 10px 15px;
            border-radius: 4px; 
        }
        .navbar a:hover {
            background-color: #555; 
        }
        .profile-popup {
            position: absolute;
            top: 60px; 
            right: 20px;
            padding: 20px;
            background-color: #f8f8f8;
            border-radius: 4px;
            border: 1px solid #ccc;
            z-index: 1;
            display: none;
            width: 200px;
        }
        .profile-popup.active {
            display: block;
        }
        .profile-popup form {
            display: flex;
            flex-direction: column;
            gap: 10px;
        }
        .profile-popup label {
            font-weight: bold;
        }
        .profile-popup input {
            padding: 8px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        .profile-popup button {
            padding: 10px 20px;
            font-size: 16px;
            cursor: pointer;
        }
        .status-box, .fan-speed, .temperature, .graph, .fan-control {
            background-color: #f8f8f8;
            border-radius: 4px;
            border: 1px solid #ccc;
            padding: 20px;
            margin-top: 20px;
        }
        footer {
            margin-top: 20px;
            text-align: center;
        }
    </style>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.4.0/Chart.min.js"></script>
</head>
<body>
    <div class="container">
        <div class="navbar">
            <a href="#" onclick="toggleProfilePopup()">Profile <span class="profile-symbol">👤</span></a>
        </div>
        <header>
            <h1>Room Temperature Fan Control</h1>
        </header>
        <div class="status-box">
            <div class="fan-speed">
                <span class="fan-symbol">🌀</span>
                <p>Fan Speed: <span id="currentFanSpeed">50</span>%</p>
            </div>
            <div class="temperature">
                <span class="temperature-symbol">🌡</span>
                <p>Temperature: <span id="currentTemperature">25</span>°C</p>
            </div>
        </div>
        <section class="graph">
            <h2>Temperature Graph</h2>
            <canvas id="myChart" width="400" height="200"></canvas>
            
        </section>
        
        <section class="fan-control">
            <h2>Fan Control</h2>
            <form id="fanControlForm">
                <label for="fan-speed">Fan Speed:</label>
                <input type="range" id="fan-speed" name="fan-speed" min="0" max="100" value="50">
                <br><br>
                <button type="submit" id="fanButton" style="width: 100%;">Turn On</button> 
            </form>
        </section>
        
        <footer>
            <p>&copy; 2024 Room Temperature Fan Control. All rights reserved.</p>
        </footer>

        
        <div class="profile-popup" id="profilePopup">
            <form id="loginForm">
                <label for="username">Username:</label>
                <input type="text" id="username" name="username">
                <label for="password">Password:</label>
                <input type="password" id="password" name="password">
                <button type="submit">Log In</button>
            </form>
            <button onclick="signOut()">Log Out</button>
        </div>

    </div>

    <script>
        document.getElementById('fanControlForm').addEventListener('submit', function(event) {
            event.preventDefault();
            var button = document.getElementById('fanButton');
            var currentAction = button.innerText;

            if (currentAction === 'Turn On') {
                
                console.log('Setting fan speed...');
                button.innerText = 'Turn Off';
            } else if (currentAction === 'Turn Off') {
               
                console.log('Turning off fan...');
                button.innerText = 'Turn On';
            }
        });

        function toggleProfilePopup() {
            var popup = document.getElementById('profilePopup');
            popup.classList.toggle('active');
        }

        function signOut() {
            alert("Signing out...");
            
        }

        
        var ctx = document.getElementById('myChart').getContext('2d');
        var chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: "My Chart",
                    data: [],
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',
                    borderColor: 'rgba(54, 162, 235, 1)',
                    borderWidth: 1
                }]
            },
            options: {
                responsive: true,
                title: {
                    display: true,
                    text: "Temperature Chart"
                },
                legend: {
                    display: true,
                    position: 'top'
                },
                tooltips: {
                    mode: 'index',
                    intersect: false
                },
                hover: {
                    mode: 'nearest',
                    intersect: true
                }
            }
        });
    </script>
</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}








//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <LiquidCrystal.h>
//
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
//int tempPin = A1;
//int fan = D1; // Replace with your actual fan pin (e.g., D1 for NodeMCU)
//int ledPin = 0;
//int temp;
//int tempMin = 30;
//int tempMax = 60;
//int fanSpeed;
//int fanLCD;
//const char* ssid = "your-SSID"; // Replace with your Wi-Fi SSID
//const char* password = "your-PASSWORD"; // Replace with your Wi-Fi password
//const char* serverName = "http://your-server-ip:5000/api/data/save"; // Replace with your backend server URL
//
//void setup() {
//  pinMode(fan, OUTPUT);
//  pinMode(ledPin, OUTPUT);
//  pinMode(tempPin, INPUT);
//  lcd.begin(16, 2);
//  Serial.begin(115200);
//  delay(1000);
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi...");
//  }
//  Serial.println("Connected to WiFi");
//}
//
//void loop() {
//  temp = readTemp();
//  Serial.print(temp);
//
//  if (temp < tempMin) {
//    fanSpeed = 0;
//    analogWrite(fan, fanSpeed);
//    fanLCD = 0;
//    digitalWrite(fan, LOW);
//  } else {
//    fanSpeed = map(temp, tempMin, tempMax, 0, 255);
//    fanSpeed = constrain(fanSpeed, 0, 255);
//    analogWrite(fan, fanSpeed);
//    fanLCD = fanSpeed / 2.55;
//  }
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Temp: ");
//  lcd.print(temp);
//  lcd.print(" C");
//  lcd.setCursor(0, 1);
//  lcd.print("Fan: ");
//  lcd.print(fanLCD);
//  lcd.print(" %");
//
//  sendToServer(temp, fanLCD);
//
//  delay(1000);
//}
//
//int readTemp() {
//  int analogValue = analogRead(tempPin);
//  float voltage = analogValue * 5.0 / 1023.0;
//  float temperatureC = (voltage - 0.5) * 100.0;
//  return (int)temperatureC;
//}
//
//void sendToServer(int temperature, int fanSpeed) {
//  if (WiFi.status() == WL_CONNECTED) {
//    HTTPClient http;
//
//    http.begin(serverName);
//    http.addHeader("Content-Type", "application/json");
//
//    String postData = "{\"temperature\":" + String(temperature) + ",\"fanSpeed\":" + String(fanSpeed) + "}";
//
//    int httpResponseCode = http.POST(postData);
//
//    if (httpResponseCode > 0) {
//      String response = http.getString();
//      Serial.print("HTTP Response code: ");
//      Serial.println(httpResponseCode);
//      Serial.println(response);
//    } else {
//      Serial.print("Error on sending POST: ");
//      Serial.println(httpResponseCode);
//    }
//
//    http.end();
//  } else {
//    Serial.println("WiFi Disconnected");
//  }
//}

//#include <LiquidCrystal.h>
//
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
//int tempPin = A1;
//int fan = 11;
//int ledPin = 0;
//int temp;
//int tempMin = 30;
//int tempMax = 60;
//int fanSpeed;
//int fanLCD;
//
//int readTemp() {
//  int analogValue = analogRead(tempPin);
//  float voltage = analogValue * 5.0 / 1023.0;
//  float temperatureC = (voltage - 0.5) * 100.0;
//  return (int)temperatureC;
//}
//
//void setup() {
//  pinMode(fan, OUTPUT);
//  pinMode(ledPin, OUTPUT);
//  pinMode(tempPin, INPUT);
//  lcd.begin(16, 2);
//  Serial.begin(9600);
//}
//
//void loop() {
//  temp = readTemp();
//  Serial.print(temp);
//
//  if (temp < tempMin) {
//    fanSpeed = 0;
//    analogWrite(fan, fanSpeed);
//    fanLCD = 0;
//    digitalWrite(fan, LOW);
//  } else {
//    fanSpeed = map(temp, tempMin, tempMax, 0, 255);
//    fanSpeed = constrain(fanSpeed, 0, 255);
//    analogWrite(fan, fanSpeed);
//    fanLCD = fanSpeed / 2.55;
//  }
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Temp: ");
//  lcd.print(temp);
//  lcd.print(" C");
//  lcd.setCursor(0, 1);
//  lcd.print("Fan: ");
//  lcd.print(fanLCD);
//  lcd.print(" %");
//
//  delay(1000);
//}
