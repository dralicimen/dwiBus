
# dwiBus Library

The `dwiBus` library provides a simple way to implement UART communication between multiple devices using either **SoftwareSerial** or **HardwareSerial** interfaces. It offers **CRC-based error control** to ensure data integrity. The library is compatible with both **Arduino** and **ESP32** platforms, supporting multiple devices on a shared line.

---

## Installation

1. Clone this repository or download it as a ZIP file:
   [https://github.com/dralicimen/dwiBus](https://github.com/dralicimen/dwiBus)

2. Open Arduino IDE and go to **Sketch > Include Library > Add .ZIP Library**.
3. Select the downloaded ZIP file to install the library.

---

## Hardware Configuration

For **multi-device UART communication** using a shared line, follow this configuration:

### 3-Device UART Configuration (TX with 1K Resistors)

```
Device 1 (Arduino Uno)      Device 2 (Arduino Nano)      Device 3 (Arduino Mega)
   (TX = 12)                  (TX = 12)                    (TX = 18)
      │                          │                            │
    1KΩ                        1KΩ                          1KΩ
      │                          │                            │
      └───┬──────────────────────┴────────────┬──────────────┘
          │                                   │
     Shared RX Line (RX = 11, 11, 19 for Uno, Nano, Mega)
          │
        RX 11 (Uno)
        RX 11 (Nano)
        RX 19 (Mega)
```

---

### Wiring Explanation

1. **TX Lines with 1K Resistors**:
   - Each device's TX line is connected to the shared RX line **through a 1K ohm resistor** to prevent signal conflicts.

2. **Shared RX Line**:
   - All devices' RX pins are connected to the **shared RX line**, allowing them to receive data on the same bus.

3. **No Ground Loop Issues**:
   - The devices communicate over the shared RX line, without requiring separate ground connections for the UART interface.

---

## Usage

### Example 1: Basic Serial Chat (`SerialChat.ino`)

This example demonstrates a basic chat system where devices send and receive messages interactively.

```cpp
#include <dwiBus.h>

#define DEVICE_ADDRESS 0x01  // Set to 0x02 for the other device
#define TARGET_ADDRESS 0x02  // Reverse for the other device

dwiBus bus(11, 12, 13, DEVICE_ADDRESS);  // Initialize dwiBus

void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Message from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);
}

void setup() {
    Serial.begin(9600);
    bus.begin(9600);
    bus.onPacketReceived(onPacketReceived);

    Serial.println("Serial Chat Started! Type a message.");
}

void loop() {
    bus.handleReceive();

    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');
        message.trim();
        if (bus.sendPacket(TARGET_ADDRESS, message.c_str(), message.length())) {
            Serial.println("Message sent.");
        } else {
            Serial.println("Failed to send: Bus is busy.");
        }
    }
}
```

---

### Example 2: Dynamic Address Chat (`DynamicSerialChat.ino`)

This example allows setting the device's address via the **serial monitor** at runtime.

---

### Example 3: Client with Address 0x01 (`Clien1.ino`)

A simple client with **address 0x01** that sends and acknowledges messages.

---

### Example 4: Client with Address 0x02 (`Client2.ino`)

A simple client with **address 0x02** that sends and acknowledges messages.

---

## Library Directory Structure

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
│   ├── Client0x01/
│   │   └── Client0x01.ino
│   ├── Client0x02/
│   │   └── Client0x02.ino
├── library.json
├── library.properties
├── LICENSE.txt
└── README.md
```

---

## CRC Error Control

The library uses **CRC-8** to ensure data integrity. Each packet contains:
2 Bytes Sync: Marks the start of the packet.
1 Byte Receiver Address: Identifies the intended recipient.
1 Byte Sender Address: Specifies the sender's address.
1 Byte Packet Number: Current packet number in a sequence.
1 Byte Total Packets: Total number of packets in the message.
1 Byte Length: Length of the data in the current packet.
Data (up to 50 bytes): Contains the actual message content.
1 Byte CRC: Ensures error checking.
2 Bytes End Marker: Packet end indicator (\r\n).

---

## License

This project is licensed under the **GNU GENERAL PUBLIC LICENSE Version 3 (GPL v3)**.  
See the [LICENSE.txt](LICENSE.txt) file for details.

---

## Author

**Ali Çimen**  
Email: [cimenwd@gmail.com](mailto:cimenwd@gmail.com)  
GitHub: [https://github.com/dralicimen](https://github.com/dralicimen)

---

## Summary

The **dwiBus library** simplifies UART communication between multiple devices with **error control** using CRC. The provided examples demonstrate how to build chat systems or extend communication between multiple devices. With **TX lines managed by resistors**, the library ensures smooth communication on a shared line.
