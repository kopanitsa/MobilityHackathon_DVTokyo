#include <WiFi.h>
#include "MobilityMqtt.h"

WiFiClient espClient;
long lastMsg = 0;
int value = 0;

const char* ssid = "PublicWork";
const char* password = "publicnet63!";

const int PIN_LED_GREEN = 13; 
const int PIN_RELAY     = 12; 
bool relay_enabled = false;

int wifi_trial = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW); // LED off 
  digitalWrite(PIN_RELAY, LOW);  // Relay off

  setup_wifi();
  mqtt_setup(&espClient);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_trial++;
    if (wifi_trial >= 40) {
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(PIN_LED_GREEN, HIGH);  // LED on
  
}

void handlePing() {
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    ++value;

//    イベントのjsonデータをサーバと合意する
    mqtt_ping(true);
    char msg[50];
  }
}

void loop() {
  mqtt_check_and_reconnect();
  mqtt_loop;
  handlePing();
}


