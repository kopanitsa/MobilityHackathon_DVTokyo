#include "MobilityMqtt.h"
#include "MobilityWifi.h"

long lastMsg = 0;
int value = 0;

const int PIN_LED_GREEN = 13;
const int PIN_RELAY     = 12;
bool relay_enabled = false;

eCONNECTION mqttConnectionStatus = Disconnected;

void (*mqtt_subscribe_callback)(String result);

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW); // LED off
    digitalWrite(PIN_RELAY, LOW);  // Relay off

    delay(10);
    wifi_setup();
    mqtt_setup();
    mqtt_subscribe_callback = &subscribe_callback;
    digitalWrite(PIN_LED_GREEN, HIGH);  // LED on
}

// TODO Controllerクラスで実施
void handlePing() {
    long now = millis();
    if (now - lastMsg > 60000) {
        lastMsg = now;
        ++value;

        // イベントのjsonデータをサーバと合意する
        mqtt_ping(true);
        char msg[50];
    }
}

void subscribe_callback(String result) {
    Serial.println("get event");
    Serial.println(result);

    // TODO
//    // switch relay status
//    if (relay_enabled) {
//        Serial.println("power off");
//        digitalWrite(PIN_RELAY, LOW);
//        relay_enabled = false;
//    } else {
//        Serial.println("power on");
//        digitalWrite(PIN_RELAY, HIGH);
//        relay_enabled = true;
//    }

}

void loop() {
    eCONNECTION ret = mqtt_check_and_reconnect();
    if (ret != mqttConnectionStatus) {
        // TODO 状態更新をcontrollerにおくる
        mqttConnectionStatus = ret;
    }
    mqtt_loop();
    handlePing();
}


