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
  - name: İstanbul Gelişim Üniversitesi, İktisadi İdari ve Sosyal Bilimler Fakültesi, Yönetim Bilişim Sistemleri Bölümü
    index: 1
date: 10 November 2024
bibliography: references.bib
---

### Summary

DwiBus: multi-device UART on shared bus, for Arduinos and ESP32s. It implements CRC-based error checking, which will ensure you always get the data as expected for SoftwareSerial and HardwareSerial interfaces. DwiBus makes multi-device interactions easier by enabling users to control shared communication lines effectively (Masoodi et al., 2025).

The library exposes a flexible API to define behaviors specific to each device, provide custom addresses and process packets seamlessly. This versatility enables it to be used for IoT projects, embedded systems, or anywhere you need reliable UART communication between devices. The dwiBus modular design allows you to extend it and with even greater flexibility, easily add it to existing projects; it is scalable and simple.

### Statement of Need

As industrial automation systems become more IoT-enabled, effective low-level communication protocols emerge as a key requirement for controlling and managing multi-device interactions. Modbus RTU and similar classical systems are widely adopted communication protocols in the industry, but they have some fundamental constraints that prevent them from being used as a part of modern IoT systems. The emergence of this challenges leads to the innovate solutions such as the dwiBus library that fulfill scaling, reliability and flexibility required by modern industrial and IoT applications.

### Challenges with Traditional Protocols

For example, Modbus RTU can not be directly embedded to the modern wireless communication technologies such as Zigbee or Wi-Fi. The protocol fails to be dynamic required by the distributed IoT systems which cause inefficiency and also affect the use of wireless technology (Gandhi & Adhyaru, 2015). The limitation is all the more detrimental as it restricts its use in such environments, where wireless communication is becoming too important nowadays (Gandhi & Adhyaru, 2015) within smart grids.

It brings us to another problem in Modbus RTU as well, it actually offers vulnerability over EMI (Electromagnetic Interference) in noisy industrial environments. It lacks solid error detection and correction method, And causes natural data drop and corruption, which harm the reliability of communication. Urrea et al. However, this protocol is not viable for industrial applications because Modbus RTU fails to preserve the correctness of data in the presence of high EMI conditions (Urrea et al., 2016).

This master-slave architecture of Modbus RTU brings in scalability limitations, restricting the number of devices that could be networked. Guarese et al. However, as mentioned in [Guarese et al., 2012], Modbus RTU supports a maximum of 247 devices in theory, but the performance continues to deteriorate when the number of nodes overpasses a limit per segment (no more than 5-10 devices), making it impractical for large scale IoT applications.

### The Role of dwiBus

To address these weaknesses, we developed the dwiBus library as a more modern alternative. The protocol is intended to mitigate the key issues with legacy protocols and adds capabilities around data, including reliability, scalability and seamless integration features.

#### Enhanced Data Integrity

The library supports CRC-8 error checking to detect and correct data corruption, which protects against unwanted noise in a high EMI environment. Urrea et al. Urrea et al (2019) also showed that implementation of sophisticated error checking mechanisms can enhance the reliability of communication within industrial networks.

#### Scalability for IoT Systems

In contrast, dwiBus empowers almost limitless networks utilized in modern IoT systems by offering full unique addressing — it can support up to 255 devices as opposed to the sizeable tract that modbus RTU can. Such scalability guarantees reliable performance irrespective of the size of the network, which makes it particularly ideal for larger deployments.

#### Broadcast Messaging

Broadcast messaging brings something special — it makes communication across many devices easier and enables much more efficient interaction patterns. This is useful particularly in distributed networks where devices require updates or synchronisation very often.

#### Flexibility and Integration

Support dwiBus integrates easily with any of your existing systems — designed for Arduino, ESP32 platforms. The modular API allows developers to seamlessly integrate the library with diverse applications, from industrial automation to IoT.

### Significance

The dwiBus library is the breakthrough for communication protocols in IoT and industrial systems. This customizes a fixed network approach to the needs of contemporary applications while overcoming the limitations of Modbus RTU, giving a scalable, reliable and repeatable way to solve a problem. The solid error-checking mechanisms combined with the scalability and flexibility of dwiBus make it fit for contemporary industrial communication challenges.

The dwiBus library pushes the boundries of what a UART-based communication protocol can be with features such as; CRC-8 error checking, broadcast messaging, and inbound modular integration. Not only does it end decades-old legacy systems problems, but this too will open next-gen paths with future-forward industrial automation and IoT networks.

### Usage

The dwiBus library provides a structured and reliable UART communication protocol for multi-device networks. Designed for Arduino and ESP32 platforms, it simplifies data transmission between up to 255 devices with CRC error checking, packet management, and broadcast messaging capabilities.

#### Setup and Initialization

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

The initialization process is simple and allows the library to adapt to various communication interfaces.

#### Sending and Receiving Messages

The dwiBus library provides straightforward functions for sending and receiving messages between devices. A message can be sent to a specific device using its unique address, or broadcasted to all devices in the network.

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

This flexible messaging framework ensures reliable communication and supports complex interaction patterns in multi-device systems.

#### Integration with HardwareSerial and SoftwareSerial

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

#### Advanced Usage: Modular Integration

The library’s modular API allows developers to incorporate it into complex systems with minimal changes to their existing codebase. The clear separation of initialization, messaging, and error-checking logic makes the library easy to integrate into new and legacy systems alike.
