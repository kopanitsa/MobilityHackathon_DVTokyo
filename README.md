# DVTokyo - BASE - IoT Device source

## brief description

- This is source for BASE IoT device.
- In BASE system, 
  - the device is connected to internet via Wi-Fi and communicate to server
  - the device is requested power supply via MQTT
  - the device controls hardware to enable/disable wireless power supply
  - the device notifies current status to server via MQTT for blockchain-based-payment

### hardware 

- ESP32 (Espressif, micro computer)
- N-ch FET is connected to PIN14
- LED is connected to PIN15

### network

- MQTT based
  - Host: test.mosquitto.org:1883
  - Protocol: mqtt / tcp
  - Publish from Server to Car: `/dvhack18/base/requestCarStatus/{carUUID}`
  - Publish from Car to Server: `/dvhack18/base/notifyCarStatus`
- Use cases
  - request to change status from server to device  
    ```
    {
      "request": "start",
      "usrId": "xxxxxxxxx",
      "expectedConsumption": "100"
    }
    ```
  - notify current status from device to server
    ```
    {
      "status": "inUse",
      "deviceId": "1234",
      "remainingPower": "10000"
    }
    ```

## build

- build: `platformio -f -c clion run`
- upload to device: `platformio -f -c clion run --target upload`
- clean: `platformio -f -c clion run --target clean`

## dependency

- [platformio](https://platformio.org/)
- [Espressif ESP32 SDK](https://github.com/espressif/arduino-esp32)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [ArduinoJSON](https://github.com/bblanchon/ArduinoJson)


