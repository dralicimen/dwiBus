
# **dwiBus Library**

The `dwiBus` library provides a structured and reliable **UART communication protocol** for multi-device networks. Designed for **Arduino** and **ESP32** platforms, it simplifies data transmission between up to 255 devices with **CRC error checking**, **packet management**, and **broadcast messaging** capabilities. 

---

## **Features**
- **Supports up to 255 devices** with individual addressing
- **Broadcast messaging** using address `0xFF`
- **Automatic packet splitting and reassembly** for large data transmissions
- **CRC-8 error control** to detect data corruption
- **Hardware and Software Serial** support
- **ACK support** for reliable communication
- **Dynamic memory management** using the `Response` class

---

## **Installation**

1. **Download or Clone**:  
   [https://github.com/dralicimen/dwiBus](https://github.com/dralicimen/dwiBus)

2. **Add Library to Arduino IDE**:
   - **Sketch > Include Library > Add .ZIP Library**
   - Select the downloaded ZIP file.

3. **Install Dependencies**:
   - Ensure **SoftwareSerial** is available for Arduino platforms.

---

## **Wiring Diagram: TX and RX with Resistors or Diodes**

To connect multiple devices over UART using **half-duplex communication**, you need to share the TX and RX lines. Use **1K Ohm resistors** or **diodes** to prevent data collisions. Below is a simple ASCII diagram:

```
Device 1            Device 2            Device 3
+---------+         +---------+         +---------+
|    TX ---+----+---+    TX ---+----+---+    TX   |
|    RX <--|----+---|--> RX <--|----+---|--> RX   |
|         [1K]      [1K]      [1K]               |
|              Common Bus                      |
|------------------ TX/RX BUS ------------------|
```

### **Using Diodes (Optional)**
- **Cathode (|<)** connects to the **TX pin** of each device.
- **Anode (>|)** connects to the shared RX line.

---

## **Usage Guide**

### **Initialization**

```cpp
#include <dwiBus.h>

// Initialize dwiBus with RX = 11, TX = 12, Control Pin = 13, Address = 0x02
dwiBus bus(11, 12, 13, 0x02);
```

---

### **Register a Callback Function**

```cpp
void onPacketReceived(const Response& response) {
    Serial.print("Received from 0x");
    Serial.print(response.sender, HEX);
    Serial.print(": ");
    Serial.write(response.completeData, response.totalLength);
    Serial.println();
}
```

---

## **Complete Example: Chat Program**

This example demonstrates communication between two devices using `dwiBus`.

```cpp
#include <dwiBus.h>

// Initialize the dwiBus object
dwiBus bus(11, 12, 13, 0x02);  
Response* responsePtr = nullptr;

// Callback to handle incoming messages
void onPacketReceived(const Response& response) {
    if (responsePtr) delete responsePtr;  
    responsePtr = new Response(response);  

    Serial.print("Received from 0x");
    Serial.print(response.sender, HEX);
    Serial.print(": ");
    Serial.write(response.completeData, response.totalLength);
    Serial.println();

    // Send ACK to sender
    bus.sendPacket(response.sender, "ACK from 0x02");
}

void setup() {
    Serial.begin(9600);
    bus.begin(9600);
    bus.onPacketReceived(onPacketReceived);

    Serial.println("Client 2 (0x02) is ready.");
}

void loop() {
    if (bus.sendPacket(0x01, "Hello from 0x02")) {
        Serial.println("Message sent to 0x01.");
    } else {
        Serial.println("Bus busy.");
    }

    bus.handleReceive();  
    delay(5000);  
}
```

---

## **Response Class Overview**

The `Response` class dynamically stores incoming data.

```cpp
class Response {
public:
    char* completeData;         // Pointer to the complete data
    uint16_t totalLength;        // Total length of the data
    uint8_t sender;              // Sender's address
    unsigned long receiveTime;   // Timestamp of reception

    Response(uint16_t length);   // Constructor to allocate memory
    ~Response();                 // Destructor to free memory
};
```

---

## **Error Handling**

- **CRC Mismatch**: If CRC fails, the packet is discarded with an error message.
- **Bus Timeout**: If the bus is busy, sending will fail with a timeout message.
- **Broadcast Handling**: Devices must ignore ACK responses to broadcast messages.

---

## **License**

This project is licensed under the **GNU GPL v3 License**. For more information, see the `LICENSE` file.

---

## **Author**

**Ali Çimen**  
GitHub: [dralicimen](https://github.com/dralicimen)  
Email: cimenwd@gmail.com  

---

## **Changelog**

- **v1.0.0**: Initial release with CRC error control and broadcast functionality.

---

## **Contributing**

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
