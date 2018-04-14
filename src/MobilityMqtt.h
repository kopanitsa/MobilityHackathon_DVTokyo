#ifndef __MOBILITY_MQTT_H__
#define __MOBILITY_MQTT_H__

#include "WiFi.h"
#include <PubSubClient.h>

typedef enum CONNECTION {
    Connected,
    Disconnected
} eCONNECTION;

void mqtt_setup();
void mqtt_loop();
eCONNECTION mqtt_check_and_reconnect();
void mqtt_ping(bool);

extern void (*mqtt_subscribe_callback)(String result);

#endif // __MOBILITY_MQTT_H__



