//
// Created by Okada, Takahiro on 2018/04/14.
//

#include "MobilityWifi.h"
#include "WiFi.h"

const char* ssid = "GlocalMe_86859";
const char* password = "42000856";
//const char* ssid = "PublicWork";
//const char* password = "publicnet63!";

int wifi_trial = 0;

void wifi_setup() {
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
}
