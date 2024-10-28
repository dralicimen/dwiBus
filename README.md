
# dwiBus Library

The `dwiBus` library is designed for UART communication between multiple devices. It supports both **SoftwareSerial** and **HardwareSerial** interfaces and provides **CRC-based error control**. It is compatible with both Arduino and ESP32 platforms.

## Installation

1. Clone this repository or download it as a ZIP file:
   [https://github.com/dralicimen/dwiBus](https://github.com/dralicimen/dwiBus)

2. Open Arduino IDE and go to **Sketch > Include Library > Add .ZIP Library**.
3. Select the downloaded ZIP file to install the library.

## Usage

Here are examples demonstrating how to use the dwiBus library:

---

### **Example 1: Basic Serial Chat (`SerialChat.ino`)**

This example demonstrates a simple chat system between two devices using the `dwiBus` protocol. Each device can send and receive messages interactively.

```cpp
#include <dwiBus.h>

// dwiBus object: RX = 11, TX = 12, control pin = 13, device address = 0x01 or 0x02
#define DEVICE_ADDRESS 0x01  // Change to 0x02 for the other device
#define TARGET_ADDRESS 0x02  // Target device address (reverse for other device)

dwiBus bus(11, 12, 13, DEVICE_ADDRESS);  // Initialize dwiBus

// Callback function to handle incoming messages
void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Message from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);
}

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    bus.begin(9600);  // Initialize dwiBus communication
    bus.onPacketReceived(onPacketReceived);  // Register the callback function

    Serial.println("Serial Chat Started!");
    Serial.println("Type a message and press Enter to send.");
}

void loop() {
    bus.handleReceive();  // Check for incoming messages

    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('
');
        message.trim();  // Remove trailing spaces or newlines

        if (bus.sendPacket(TARGET_ADDRESS, message.c_str(), message.length())) {
            Serial.println("Message sent.");
        } else {
            Serial.println("Failed to send message: Bus is busy.");
        }
    }
}
```

---

### **Example 2: Dynamic Address Chat (`DynamicSerialChat.ino`)**

In this example, the device address is set at runtime via the serial monitor. Users can specify the recipient's address and the message interactively.

```cpp
#include <dwiBus.h>

#define RX_PIN 11
#define TX_PIN 12
#define BUS_PIN 13

dwiBus* bus;  // Pointer to the dwiBus object
uint16_t deviceAddress;  // Device address

void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Message from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Enter your device address (1-255): ");

    while (Serial.available() == 0) {}  // Wait for input
    deviceAddress = Serial.parseInt();

    if (deviceAddress < 1 || deviceAddress > 255) {
        Serial.println("Invalid address! Using default address 1.");
        deviceAddress = 1;
    }

    bus = new dwiBus(RX_PIN, TX_PIN, BUS_PIN, deviceAddress);
    bus->begin(9600);
    bus->onPacketReceived(onPacketReceived);

    Serial.print("Device address set to: 0x");
    Serial.println(deviceAddress, HEX);
    Serial.println("You can now start chatting.");
}

void loop() {
    Serial.println("Enter recipient's address (1-255): ");
    while (Serial.available() == 0) {}
    uint16_t recipientAddress = Serial.parseInt();

    Serial.println("Enter your message: ");
    while (Serial.available() == 0) {}
    String message = Serial.readStringUntil('
');
    message.trim();

    if (bus->sendPacket(recipientAddress, message.c_str(), message.length())) {
        Serial.println("Message sent.");
    } else {
        Serial.println("Failed to send message: Bus is busy.");
    }

    bus->handleReceive();
}
```

---

## License

This project is licensed under the **GNU GENERAL PUBLIC LICENSE Version 3 (GPL v3)**. See the [LICENSE.txt](LICENSE.txt) file for details.

## Author

**Ali Çimen**  
Email: [cimenwd@gmail.com](mailto:cimenwd@gmail.com)  
GitHub: [https://github.com/dralicimen](https://github.com/dralicimen)

---

## Directory Structure

```
dwiBus/
│
├── src/
│   ├── dwiBus.h
│   ├── dwiBus.cpp
├── examples/
│   ├── SerialChat/
│   │   └── SerialChat.ino
│   ├── DynamicSerialChat/
│   │   └── DynamicSerialChat.ino
├── library.json
├── library.properties
├── LICENSE.txt
└── README.md
```

---

## Summary

The **dwiBus library** simplifies UART-based communication between multiple devices, providing real-time packet handling with CRC-based error control. With the provided examples, you can build chat systems or extend communication to multiple devices in a network.

The library is now ready to use with **Arduino IDE** and **PlatformIO** environments.
