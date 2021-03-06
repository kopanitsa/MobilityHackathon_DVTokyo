//
// Created by Okada, Takahiro on 2018/04/14.
//

#include "MobilityController.h"
#include "MobilityMqtt.h"
#include "MobilityWifi.h"
#include "MobilityGpio.h"
#include <ArduinoJson.h>

/**************
 * Constants, Macro
 **************/

eSTATUS mStatus = DISCONNECTED;
eSTATUS mPreviousStatus = DISCONNECTED;

/**************
 * Local variables
 **************/

long mLastMsg = 0;

void (*mqtt_subscribe_callback)(String result);

/**************
 * Private functions
 **************/

void subscribe_callback(const String result) {
    Serial.println("get event");
    Serial.println(result);

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(result);
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
    }
    const char* req = root["request"];
    if (strcmp(req, "start") == 0) {
        gpio_relay_on(true);
    } else if (strcmp(req, "stop") == 0) {
        gpio_relay_on(false);
    }

    jsonBuffer.clear();
}

void handle_ping() {
    long now = millis();
    if (now - mLastMsg > 60000) {
        mLastMsg = now;
        mqtt_ping(mStatus);
    }
}

void update_state() {
    // check status has been changed
    if (mStatus != mPreviousStatus) { return; }

    // exec something based on new status
    switch (mStatus) {
        case DISCONNECTED:
            gpio_led_on(false);
            break;
        case LOW_POWER:
            break;
        case STANDBY:
            gpio_led_on(true); // after setting finished, LED lights ON.
            break;
        case RESERVED:
            break;
        case IN_USE:
            break;
        default:
            break;
    }
    mPreviousStatus = mStatus;
}

/**************
 * Public functions
 **************/

void controller_setup() {
    Serial.begin(115200);
    gpio_setup();
    delay(10);
    wifi_setup();
    mqtt_setup();
    mqtt_subscribe_callback = &subscribe_callback;
}

void controller_loop() {
    bool connected = mqtt_check_and_reconnect();
    if (connected) {
        mStatus = STANDBY; // TODO handle reserved etc
    } else{
        mStatus = DISCONNECTED;
    }
    mqtt_loop();
    handle_ping();
    update_state();
}
