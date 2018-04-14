
# Protocol between Server and Car

## Broker Server

- Host: `test.mosquitto.org:1883`
- Protocol: `mqtt / tcp`

## Channel

- Publish from Server to Car: `/dvhack18/base/requestCarStatus/{carUUID}`
    - `{carUUID}`は車ごとに異なる。
    - デモでは`1234`で固定
- Publish from Car to Server: `/dvhack18/base/notifyCarStatus`

## Use Cases

### 1. サーバから車へ電源供給開始を要求する (SYN)

- Direction: `Server -> Car`
- Channel: `/dvhack18/base/requestCarStatus/1234`
- Data:

```
{
	"request": "start",
	"usrId": "xxxxxxxxx",
	"expectedConsumption": "100"
}
```

- Note:
    - userId, expectedConsumptionはダミーデータ(デバイス側では処理しない)
    - 他に色々必要だが、一旦考えない

### 2. 車からサーバに電源供給開始を通知する (ACK)

- Direction: `Car -> Server`
- Channel: `/dvhack18/base/notifyCarStatus`
- Data:

```
{
	"status": "inUse",
	"deviceId": "1234",
	"remainingPower": "10000"
}
```

- Note:
    - deviceIdは1234固定
    - remainingPowerはダミーデータ(10000固定)

### 3. 車からサーバに電源供給終了を通知する (SYN)

- Direction: `Server -> Car`
- Channel: `/dvhack18/base/requestCarStatus/1234`
- Data:

```
{
	"request": "stop",
	"usrId": "xxxxxxxxx",
	"expectedConsumption": "100"
}
```

### 4. 車からサーバに電源供給終了を通知する (ACK)

- Direction: `Car -> Server`
- Channel: `/dvhack18/base/notifyCarStatus`
- Data:

```
{
	"status": "standby",
	"deviceId": "1234",
	"remainingPower": "10000"
}
```

### 5. 車からサーバに現在の状態を定期的に通知する

- Direction: `Car -> Server`
- Channel: `/dvhack18/base/notifyCarStatus`
- Data:

```
{
	"status": "standby|inUse", 
	"deviceId": "1234",
	"remainingPower": "10000"
}
```

- Note:
    - 60秒ごとに通知