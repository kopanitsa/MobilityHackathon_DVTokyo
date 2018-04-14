#include "MobilityMqtt.h"

/**************
 * Constants, Macro
 **************/

const char* PUBLISH_TOPIC = "/mooba/status";
const char* SUBSCRIBE_TOPIC = "/mooba/request";
const char* UUID = "3336D1CD-954F-46B7-920C-24D6F937D4ED";
const char* MQTT_SERVER = "test.mosquitto.org";
#define MQTT_SERVER_PORT 1883

/**************
 * Local variables
 **************/

WiFiClient wifiClient;
PubSubClient mqttClient;
int mqtt_trial = 0;

/**************
 * Private functions
 **************/

bool reconnect() {
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
    Serial.print("Message arrived! [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    mqtt_subscribe_callback(String((char*)payload));
}


/**************
 * Public functions
 **************/


void mqtt_setup() {
    mqttClient.setClient(wifiClient);
    mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
    mqttClient.setCallback(handleSubscription);
}

void mqtt_loop() {
    mqttClient.loop();
}

eCONNECTION mqtt_check_and_reconnect() {
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

