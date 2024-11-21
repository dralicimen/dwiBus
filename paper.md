---
title: 'Multi-Device UART Communication with the dwiBus Library'
tags:
  - UART
  - IoT
  - Embedded Systems
  - Arduino
  - ESP32
authors:
  - name: Ali Çimen
    orcid: 0000-0002-7515-2733
    affiliation: 1
    corresponding: true
affiliations:
  - name: Yönetim Bilişim Sistemleri Bölümü, İstanbul Gelişim Üniversitesi, İstanbul, Türkiye
    index: 1
date: 10 November 2024
bibliography: references.bib
---

# Summary

The **dwiBus** library provides a modern, reliable communication protocol for multi-device UART communication on shared buses, catering specifically to Arduino and ESP32 platforms. Designed to overcome the limitations of traditional protocols like Modbus RTU, it integrates CRC-based error checking, scalability for up to 255 devices, and a flexible API for seamless integration in IoT and embedded systems.

# Statement of Need

As industrial automation systems become more IoT-enabled, effective low-level communication protocols are essential. Classical systems like Modbus RTU are widely adopted but exhibit constraints that hinder their suitability for modern IoT applications. The **dwiBus** library addresses these challenges, offering enhanced reliability, scalability, and flexibility.

## Challenges with Traditional Protocols

1. **Incompatibility with Modern Wireless Systems**  
   Modbus RTU cannot directly integrate with technologies like Zigbee or Wi-Fi, limiting its use in distributed IoT systems [@gandhi2015modbus].

2. **Susceptibility to Electromagnetic Interference (EMI)**  
   Modbus RTU lacks robust error detection and correction, making it unreliable in noisy environments [@urrea2016error].

3. **Scalability Issues**  
   Modbus RTU supports up to 247 devices in theory, but performance degrades significantly beyond 5-10 devices per segment [@guarese2012modbus].

## The Role of dwiBus

The **dwiBus** library is a modern alternative designed to mitigate these issues, with features tailored for IoT and industrial automation.

### Key Features

- **Enhanced Data Integrity:** CRC-8 error checking protects against EMI and ensures reliable communication [@urrea2019modbus].  
- **Scalability:** Supports up to 255 devices with unique addressing.  
- **Broadcast Messaging:** Enables efficient communication across distributed networks.  
- **Flexibility:** Easily integrates with Arduino and ESP32 platforms, supporting both HardwareSerial and SoftwareSerial interfaces.

# Usage

The **dwiBus** library provides a structured and reliable UART communication protocol for multi-device networks. Designed for Arduino and ESP32 platforms, it simplifies data transmission between up to 255 devices with CRC error checking, packet management, and broadcast messaging capabilities.

## Setup and Initialization

To use the dwiBus library, the user must include the library in their project and initialize it with the appropriate communication interface. Each device in the network is assigned a unique address, which serves as its identifier.

```cpp
#include <dwiBus.h>

// Create a dwiBus instance
dwiBus bus;

// Define a unique address for this device
const uint8_t DEVICE_ADDRESS = 1;

void setup() {
    Serial.begin(9600);               // Start serial communication
    bus.begin(&Serial, DEVICE_ADDRESS); // Initialize dwiBus with HardwareSerial and address
    Serial.println("Device initialized!");
}
```

## Sending and Receiving Messages

The dwiBus library provides straightforward functions for sending and receiving messages between devices. A message can be sent to a specific device using its unique address or broadcasted to all devices in the network.

```cpp
bus.write(receiverAddress, "Hello Device");
```

**Broadcasting Messages**
```cpp
bus.broadcast("System Check");
```

**Receiving Messages**
```cpp
if (bus.available()) {
    uint8_t sender;
    String message = bus.read(&sender);
    Serial.print("Message from Device ");
    Serial.print(sender);
    Serial.print(": ");
    Serial.println(message);
}
```

## Integration with HardwareSerial and SoftwareSerial

The library is compatible with both HardwareSerial and SoftwareSerial, allowing developers to select the interface that best suits their application. This flexibility ensures compatibility with a wide range of hardware platforms, including Arduino and ESP32.

```cpp
#include <SoftwareSerial.h>
#include <dwiBus.h>

// Initialize SoftwareSerial
SoftwareSerial softSerial(10, 11); // RX, TX
dwiBus bus;

void setup() {
    softSerial.begin(9600);
    bus.begin(&softSerial, 2); // Initialize dwiBus with SoftwareSerial and address
}
```

# Advanced Usage: Modular Integration

The library’s modular API allows developers to incorporate it into complex systems with minimal changes to their existing codebase. The clear separation of initialization, messaging, and error-checking logic makes the library easy to integrate into new and legacy systems alike.

# References
