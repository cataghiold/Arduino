# ESP32 Hearthstone Companion

An embedded application developed with **ESP32**, **Wi-Fi**, **Bluetooth Serial** and **REST APIs** that retrieves Hearthstone card information from an external API and sends it to a mobile application via Bluetooth.

The project demonstrates the integration between embedded systems, web services and mobile communication protocols.

## Features

* Wi-Fi connectivity using ESP32
* Communication with external REST APIs
* Bluetooth Serial communication with a mobile application
* JSON serialization and deserialization using ArduinoJson
* Fetching and displaying Hearthstone card information
* Retrieval of detailed card information, including:

  * Name
  * Card type
  * Cost
  * Attack
  * Health
  * Image URL
  * Description

## System Architecture

```text
Mobile App
     │
 Bluetooth Serial
     │
     ▼
    ESP32
     │
   Wi-Fi
     │
 REST API
     │
Hearthstone Database
```

## Application Workflow

1. The mobile application sends a request via Bluetooth.
2. ESP32 receives and parses the JSON message.
3. Depending on the requested action:

   * `fetchData`
   * `fetchDetails`
4. ESP32 performs an HTTP request to the external API.
5. The API response is deserialized and processed.
6. The processed data is serialized back to JSON.
7. The information is transmitted to the mobile application through Bluetooth.

## Technologies

* C++
* Arduino Framework
* ESP32
* Wi-Fi
* Bluetooth Serial
* REST APIs
* ArduinoJson
* HTTPClient

## Project Structure

```text
esp32-hearthstone-companion/
│
├── main.ino
├── README.md
└── platformio.ini (optional)
```

## Dependencies

```text
ArduinoJson
WiFi
HTTPClient
BluetoothSerial
```

## Example JSON Request

### Fetch all cards

```json
{
  "action": "fetchData"
}
```

### Fetch card details

```json
{
  "action": "fetchDetails",
  "cardId": "EX1_001"
}
```

## Example JSON Response

```json
{
  "cardId": "EX1_001",
  "name": "Lightwarden",
  "img": "https://...",
  "description": "Card set: Expert1\nType: Minion\nCost: 1\nAttack: 1\nHealth: 2"
}
```

## Concepts Demonstrated

* Embedded Systems Development
* API Integration
* Bluetooth Communication
* Network Programming
* JSON Processing
* Event-Driven Programming
* Client-Server Architecture

## Future Improvements

* Support for BLE characteristics instead of Bluetooth Serial.
* Local caching of card information.
* Offline mode.
* Improved error handling and reconnection mechanisms.
* Support for additional Hearthstone endpoints.

## Author

**George-Catalin Ghiold**

Faculty of Electronics, Telecommunications and Information Technology (TST)
National University of Science and Technology POLITEHNICA Bucharest
