#include "MobilityMqtt.h"
#include "MobilityWifi.h"
#include "MobilityGpio.h"

long lastMsg = 0;
int value = 0;

eCONNECTION mqttConnectionStatus = Disconnected;

void (*mqtt_subscribe_callback)(String result);

void setup() {
    Serial.begin(115200);
    gpio_setup();
    delay(10);
    wifi_setup();
    mqtt_setup();
    mqtt_subscribe_callback = &subscribe_callback;

    // after setting finished, LED lights ON.
    gpio_led_on(HIGH);
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

    // TODO 状態に応じて値を変える
    gpio_relay_on(true);
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


