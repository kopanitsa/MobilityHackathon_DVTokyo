#ifndef __MOBILITY_MQTT_H__
#define __MOBILITY_MQTT_H__

#include "WiFi.h"
#include <PubSubClient.h>
#include "MobilityStatus.h"

void mqtt_setup();
void mqtt_loop();
bool mqtt_check_and_reconnect();
void mqtt_ping(eSTATUS status);

extern void (*mqtt_subscribe_callback)(String result);

#endif // __MOBILITY_MQTT_H__



