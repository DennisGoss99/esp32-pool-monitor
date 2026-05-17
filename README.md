# ESP32 Pool Monitor

An ESP32-based outdoor pool monitor that measures water temperature and light levels, sending data via MQTT to Home Assistant.

## System Architecture

```mermaid
flowchart LR
    subgraph Sensors
        DS18B20[DS18B20\nWater Temp]
        BH1750[BH1750\nLight Sensor]
    end

    subgraph ESP32[ESP32 WROOM-32]
        GPIO4[GPIO4]
        GPIO21[GPIO21 SDA]
        GPIO22[GPIO22 SCL]
    end

    subgraph Server
        MQTT[Mosquitto\nMQTT Broker]
        HA[Home Assistant]
    end

    DS18B20 -->|OneWire| GPIO4
    BH1750 -->|I2C| GPIO21
    BH1750 -->|I2C| GPIO22
    ESP32 -->|WiFi| MQTT
    MQTT --> HA
```

## Wiring

### Power Supply

```mermaid
flowchart LR
    PSU[12V DC\nOutdoor PSU] -->|12V| BUCK[Buck Converter\nLM2596]
    BUCK -->|5V| VIN[ESP32 VIN Pin]
    VIN -->|3.3V internal| ESP32[ESP32]
```

### DS18B20 Temperature Sensor

```mermaid
flowchart TB
    subgraph ESP32
        VCC1[3.3V]
        GND1[GND]
        GPIO4[GPIO4]
    end

    subgraph DS18B20
        RED[VCC - Red]
        BLACK[GND - Black]
        YELLOW[DATA - Yellow]
    end

    R[4.7kΩ Resistor]

    VCC1 --> RED
    VCC1 --> R
    R --> YELLOW
    GND1 --> BLACK
    GPIO4 --> YELLOW
```

### BH1750 Light Sensor

```mermaid
flowchart TB
    subgraph ESP32
        VCC2[3.3V]
        GND2[GND]
        SDA[GPIO21 - SDA]
        SCL[GPIO22 - SCL]
        GND3[GND]
    end

    subgraph BH1750
        V[VCC]
        G[GND]
        SD[SDA]
        SC[SCL]
        AD[ADDR]
    end

    VCC2 --> V
    GND2 --> G
    SDA --> SD
    SCL --> SC
    GND3 -->|0x23 address| AD
```

## MQTT Topics

| Topic | Description | Unit |
|-------|-------------|------|
| `pool/temperature` | Water temperature (avg of last 10 readings) | °C |
| `pool/light` | Light level (avg of last 10 readings) | lx |

## Project Structure

```
src/
├── main.cpp            # Main setup and loop
├── config.h            # WiFi & MQTT credentials (not in git)
├── temperature.h/.cpp  # DS18B20 sensor
├── light.h/.cpp        # BH1750 sensor
├── mqtt.h/.cpp         # WiFi & MQTT connection
└── movingaverage.h     # Moving average helper
```

## Configuration

Copy `src/config.h.example` to `src/config.h` and fill in your credentials:

```cpp
#define WIFI_SSID     "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

#define MQTT_HOST     "your-mqtt-host"
#define MQTT_PORT     1883
#define MQTT_USER     "your-mqtt-user"
#define MQTT_PASSWORD "your-mqtt-password"

#define MQTT_TOPIC_TEMPERATURE  "pool/temperature"
#define MQTT_TOPIC_LIGHT        "pool/light"
```

## Home Assistant

Add to `configuration.yaml`:

```yaml
mqtt:
  sensor:
    - name: "Pool Temperatur"
      state_topic: "pool/temperature"
      unit_of_measurement: "°C"
      device_class: temperature
      unique_id: "pool_temperature"
      device:
        identifiers: ["esp32_pool"]
        name: "ESP32 Pool Sensor"
        model: "ESP32-WROOM-32"
        manufacturer: "AZ-Delivery"

    - name: "Pool Helligkeit"
      state_topic: "pool/light"
      unit_of_measurement: "lx"
      device_class: illuminance
      unique_id: "pool_light"
      device:
        identifiers: ["esp32_pool"]
        name: "ESP32 Pool Sensor"
        model: "ESP32-WROOM-32"
        manufacturer: "AZ-Delivery"
```
