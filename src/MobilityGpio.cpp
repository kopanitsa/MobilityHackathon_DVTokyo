//
// Created by Okada, Takahiro on 2018/04/14.
//

#include "MobilityGpio.h"
#include "WiFi.h"

const int PIN_LED_GREEN = 13;
const int PIN_RELAY     = 12;
bool relay_enabled = false;

void gpio_setup() {
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW); // LED off
    digitalWrite(PIN_RELAY, LOW);  // Relay off
}

void gpio_led_on(bool on) {
    digitalWrite(PIN_LED_GREEN, on);  // LED on
}

void gpio_relay_on(bool on) {
    Serial.print("start supply: ");
    Serial.println(on);
    digitalWrite(PIN_RELAY, on);
}