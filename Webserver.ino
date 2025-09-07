#include <WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiServer server(80);   
#define LED_PIN 2        

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {
    Serial.println("New Client Connected.");
    String request = "";
    unsigned long timeout = millis();
    
    while (client.connected() && millis() - timeout < 2000) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n') {  // End of HTTP request headers
          break;
        }
      }
    }

    Serial.println(request);

    // Control LED based on request
    if (request.indexOf("GET /on") != -1) {
      digitalWrite(LED_PIN, HIGH);
    } 
    else if (request.indexOf("GET /off") != -1) {
      digitalWrite(LED_PIN, LOW);
    }

    // Build HTTP response (webpage)
    String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    html += "<!DOCTYPE html><html><head><title>ESP32 LED</title></head><body>";
    html += "<h1>ESP32 LED Control</h1>";
    html += "<p><a href=\"/on\"><button>ON</button></a></p>";
    html += "<p><a href=\"/off\"><button>OFF</button></a></p>";
    html += "</body></html>";

    client.print(html);
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
