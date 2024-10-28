
# dwiBus Library

The `dwiBus` library is designed for UART communication between multiple devices. It supports both **SoftwareSerial** and **HardwareSerial** interfaces and provides **CRC-based error control**. It is compatible with both Arduino and ESP32 platforms.

## Installation

1. Clone this repository or download it as a ZIP file:
   https://github.com/dralicimen/dwiBus

2. Open Arduino IDE and go to **Sketch > Include Library > Add .ZIP Library**.
3. Select the downloaded ZIP file to install the library.

## Usage

Here is a simple example of using the dwiBus library:

```cpp
#include <dwiBus.h>
#include <SoftwareSerial.h>

// Initialize SoftwareSerial on pins RX = 11, TX = 12
SoftwareSerial softSerial(11, 12);

// Create dwiBus object with SoftwareSerial, control pin 13, and device address 0x01
SerialBus bus(softSerial, 13, 0x01);

void setup() {
    softSerial.begin(9600);  // Start SoftwareSerial communication
    Serial.begin(9600);  // Debug output
}

void loop() {
    // Send a packet to device with address 0x02
    bus.sendPacket(0x02, "HELLO", 5);

    // Check for received packets
    dwiBus receivedPacket;
    if (bus.receivePacket(receivedPacket)) {
        Serial.print("Packet received: ");
        Serial.println(receivedPacket.data);
    }

    delay(1000);  // Wait for 1 second
}
```

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE Version 3 (GPL v3). See the LICENSE.txt file for details.

## Author

**Ali Çimen**  
Email: [cimenwd@gmail.com](mailto:cimenwd@gmail.com)  
GitHub: [https://github.com/dralicimen](https://github.com/dralicimen)

---

## **Directory Structure**

```
dwiBus/
│
├── src/
│   ├── dwiBus.h
│   ├── dwiBus.cpp
│
├── examples/
│   └── dwiBusTest/
│       └── dwiBusTest.ino
│
├── library.json
├── library.properties
├── LICENSE.txt
└── README.md
```

## **Conclusion**

These files and structure complete the **dwiBus library** for use on Arduino and ESP32 platforms. Licensed under **GNU GPL v3**, with relevant links provided. The library is now ready to be used with **Arduino IDE** and **PlatformIO** environments.
