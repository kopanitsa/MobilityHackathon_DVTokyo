//
// Created by Okada, Takahiro on 2018/04/14.
//

#include "MobilityController.h"
#include "MobilityMqtt.h"
#include "MobilityWifi.h"
#include "MobilityGpio.h"

void subscribe_callback(String result);
void handlePing();

eCONNECTION mqttConnectionStatus = Disconnected;
long lastMsg = 0;
int value = 0;

void (*mqtt_subscribe_callback)(String result);

void controller_setup() {
    Serial.begin(115200);
    gpio_setup();
    delay(10);
    wifi_setup();
    mqtt_setup();
    mqtt_subscribe_callback = &subscribe_callback;

    // after setting finished, LED lights ON.
    gpio_led_on(true);
}

void controller_loop() {
    eCONNECTION ret = mqtt_check_and_reconnect();
    if (ret != mqttConnectionStatus) {
        // TODO 状態更新をcontrollerにおくる
        mqttConnectionStatus = ret;
    }
    mqtt_loop();
    handlePing();
}

void subscribe_callback(String result) {
    Serial.println("get event");
    Serial.println(result);

    // TODO 状態に応じて値を変える
    gpio_relay_on(true);
}

void handlePing() {
    long now = millis();
    if (now - lastMsg > 60000) {
        lastMsg = now;
        ++value;

        // TODO 定期イベントを投げる
        mqtt_ping(true);
    }
}
