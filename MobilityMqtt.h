#include <WiFi.h>

void mqtt_setup(WiFiClient* wifiClient);
void mqtt_loop();
void mqtt_check_and_reconnect();
void mqtt_ping(bool);

