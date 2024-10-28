#include <dwiBus.h>

// dwiBus object: RX = 11, TX = 12, control pin = 13, device address = 0x01 (change for other devices)
dwiBus bus(11, 12, 13, 0x01);  // Set your device's unique address here

// Store the destination device's address (e.g., chat with device 0x02)
uint16_t chatPartnerAddress = 0x02;  // Change as needed

// Callback function to handle incoming chat messages
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
    // Check for incoming messages
    bus.handleReceive();

    // Send a message if there is input from the serial monitor
    if (Serial.available() > 0) {
        // Read the input from the serial monitor
        String message = Serial.readStringUntil('\n');
        message.trim();  // Remove any trailing newlines or spaces

        // Send the message to the chat partner
        if (bus.sendPacket(chatPartnerAddress, message.c_str(), message.length())) {
            Serial.println("Message sent.");
        } else {
            Serial.println("Failed to send message: Bus is busy.");
        }
    }
}
