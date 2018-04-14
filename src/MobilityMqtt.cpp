#include "MobilityMqtt.h"

/**************
 * Constants, Macro
 **************/

const char* PUBLISH_TOPIC = "/dvhack18/base/notifyCarStatus";
const char* SUBSCRIBE_TOPIC = "/dvhack18/base/requestCarStatus/1234";
const char* UUID = "3336D1CD-954F-46B7-920C-24D6F937D4ED";
const char* MQTT_SERVER = "test.mosquitto.org";
#define MQTT_SERVER_PORT 1883

/**************
 * Local variables
 **************/

WiFiClient mWifiClient;
PubSubClient mMqttClient;
int mqtt_trial = 0;

/**************
 * Private functions
 **************/

bool reconnect() {
    // Loop until we're reconnected
    while (!mMqttClient.connected()) {
        Serial.println("Attempting MQTT connection...");
        // Attempt to connect
        if (mMqttClient.connect(UUID)) {
            Serial.println("connected");
            char msg[50];
            memset(msg, 0, sizeof(msg));
            mMqttClient.subscribe(SUBSCRIBE_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mMqttClient.state());
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

void handle_subscription(char *topic, byte *payload, unsigned int length) {
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
    mMqttClient.setClient(mWifiClient);
    mMqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
    mMqttClient.setCallback(handle_subscription);
}

void mqtt_loop() {
    mMqttClient.loop();
}

bool mqtt_check_and_reconnect() {
    if (!mMqttClient.connected()) {
        reconnect();
    }
    return mMqttClient.connected();
}

void mqtt_ping(eSTATUS status) {
    char msg[256];
    memset(msg, 0, sizeof(msg));
    if (status == IN_USE) {
        sprintf(msg, "{\n"
                "\t\"status\": \"inUse\", \n"
                "\t\"deviceId\": \"1234\",\n"
                "\t\"remainingPower\": \"10000\"\n"
                "}");
    } else {
        sprintf(msg, "{\n"
                "\t\"status\": \"standby\", \n"
                "\t\"deviceId\": \"1234\",\n"
                "\t\"remainingPower\": \"10000\"\n"
                "}");
    }
    Serial.print("Publish message: ");
    Serial.println(msg);
    mMqttClient.publish(PUBLISH_TOPIC, msg);
}

