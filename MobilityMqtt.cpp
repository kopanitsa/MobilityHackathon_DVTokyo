#include "MobilityMqtt.h"
#include <PubSubClient.h>

/**************
 * Constants, Macro
 **************/

const char* PUBLISH_TOPIC = "/mooba/status";
const char* SUBSCRIBE_TOPIC = "/mooba/event";
const char* UUID = "3336D1CD-954F-46B7-920C-24D6F937D4ED";
const char* MQTT_SERVER = "test.mosquitto.org";
#define MQTT_SERVER_PORT 1883

/**************
 * Local variables
 **************/

PubSubClient mqttClient;
int mqtt_trial = 0;

/**************
 * Private functions
 **************/

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(UUID)) {
      Serial.println("connected");
      char msg[50];
      memset(msg, 0, sizeof(msg));
      snprintf (msg, 50, "{status: %s}", "connected");
      mqttClient.publish(PUBLISH_TOPIC, msg);
      mqttClient.subscribe(SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      mqtt_trial++;
      if (mqtt_trial >= 40) {
        ESP.restart();
      }
    }
  }
}

void handleSubscription(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

//イベントの内容で判定

//  // switch relay status
//  if (relay_enabled) {
//    Serial.println("power off");
//    digitalWrite(PIN_RELAY, LOW); 
//    relay_enabled = false;
//  } else {
//    Serial.println("power on");
//    digitalWrite(PIN_RELAY, HIGH); 
//    relay_enabled = true;
//  }
}


/**************
 * Public functions
 **************/

void mqtt_setup(WiFiClient* wifiClient) {
  mqttClient.setClient(*wifiClient);
  mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
  mqttClient.setCallback(handleSubscription);
}

void mqtt_loop() {
  mqttClient.loop();  
}

void mqtt_check_and_reconnect() {
  if (!mqttClient.connected()) {
    reconnect();
  }  
}

void mqtt_ping(bool) {
    char msg[50];
    memset(msg, 0, sizeof(msg));
    snprintf (msg, 50, "{status: %s}", "connected");
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish(PUBLISH_TOPIC, msg);  
}

