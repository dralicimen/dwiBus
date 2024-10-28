#include <dwiBus.h>

// Define RX and TX pins for SoftwareSerial, and control pin for the bus
#define RX_PIN 11
#define TX_PIN 12
#define BUS_PIN 13

dwiBus* bus;  // Pointer to the dwiBus object
uint16_t deviceAddress;  // This device's address

// Callback function to handle incoming messages
void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Message from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);
}

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    Serial.println("Enter your device address (1-255): ");

    // Wait until the user enters a valid address
    while (Serial.available() == 0) {
        // Do nothing, just wait for input
    }
    deviceAddress = Serial.parseInt();  // Read the device address as an integer

    // Validate the entered address
    if (deviceAddress < 1 || deviceAddress > 255) {
        Serial.println("Invalid address! Using default address 1.");
        deviceAddress = 1;  // Default to address 1 if the input is invalid
    }

    // Create the dwiBus object dynamically with the entered address
    bus = new dwiBus(RX_PIN, TX_PIN, BUS_PIN, deviceAddress);
    bus->begin(9600);  // Initialize dwiBus communication
    bus->onPacketReceived(onPacketReceived);  // Register the callback function

    Serial.print("Device address set to: 0x");
    Serial.println(deviceAddress, HEX);
    Serial.println("You can now start chatting.");
}

void loop() {
    // Ask the user to enter the recipient's address
    Serial.println("Enter the recipient's address (1-255): ");
    while (Serial.available() == 0) {
        // Wait for input
    }
    uint16_t recipientAddress = Serial.parseInt();  // Read the recipient's address

    // Ask the user to enter the message
    Serial.println("Enter your message: ");
    while (Serial.available() == 0) {
        // Wait for input
    }
    String message = Serial.readStringUntil('\n');  // Read the message
    message.trim();  // Remove any trailing newlines or spaces

    // Send the message to the recipient
    if (bus->sendPacket(recipientAddress, message.c_str(), message.length())) {
        Serial.println("Message sent.");
    } else {
        Serial.println("Failed to send message: Bus is busy.");
    }

    // Check for incoming packets
    bus->handleReceive();
}
