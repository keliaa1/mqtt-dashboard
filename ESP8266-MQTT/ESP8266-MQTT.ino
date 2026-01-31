#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h> // Required for structured JSON payloads

/* ---------------- CONFIGURATION ---------------- */
const char* ssid     = "EdNet";
const char* password = "Huawei@123";
const char* mqtt_server = "157.173.101.159";

// Topics aligned with book's design [cite: 353, 457]
#define TOPIC_DATA      "sensors/dht"
#define TOPIC_LED_CMD   "control/led"
#define TOPIC_LED_STATE "control/led/status"
#define TOPIC_STATUS    "iot/status/esp8266_arduino"

/* ---------------- HARDWARE SETUP ---------------- */
#define DHTPIN 5        // D1 (GPIO5) as per book's wiring [cite: 341]
#define LEDPIN 4        // D2 (GPIO4) as per book's wiring [cite: 341]
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

/* ---------------- MQTT CALLBACK (LED CONTROL) ---------------- */
// Handles incoming commands from the dashboard 
void callback(char* topic, byte* payload, unsigned long length) {
  String message;
  for (int i = 0; i < length; i++) message += (char)payload[i];

  if (String(topic) == TOPIC_LED_CMD) {
    if (message == "ON" || message == "1") {
      digitalWrite(LEDPIN, HIGH);
    } else {
      digitalWrite(LEDPIN, LOW);
    }
    // Publish feedback to dashboard [cite: 353, 500]
    String state = (digitalRead(LEDPIN) == HIGH) ? "ON" : "OFF";
    client.publish(TOPIC_LED_STATE, state.c_str(), true);
  }
}

void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

/* ---------------- RECONNECT WITH LAST WILL ---------------- */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Connect with Last Will to signal "offline" if connection drops [cite: 300, 385, 501]
    if (client.connect("ESP8266_Arduino", TOPIC_STATUS, 1, true, "offline")) {
      Serial.println("connected");
      client.publish(TOPIC_STATUS, "online", true); // Signal online [cite: 501]
      client.subscribe(TOPIC_LED_CMD);             // Subscribe to commands [cite: 353]
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  // Publish every 5 seconds as per firmware example [cite: 457, 462]
  if (now - lastMsg > 5000) {
    lastMsg = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
      // Create structured JSON payload 
      StaticJsonDocument<128> doc;
      doc["temperature"] = t;
      doc["humidity"] = h;
      doc["ts"] = 0; // In a full implementation, you'd add NTP time here [cite: 380]

      char buffer[128];
      serializeJson(doc, buffer);
      
      // Publish as retained so dashboard sees data immediately on refresh 
      client.publish(TOPIC_DATA, buffer, true);
      Serial.println("Published JSON: " + String(buffer));
    }
  }
}