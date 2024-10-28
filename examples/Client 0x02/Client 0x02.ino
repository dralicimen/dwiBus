#include <dwiBus.h>

// dwiBus object: RX = 11, TX = 12, control pin = 13, device address = 0x02
dwiBus bus(11, 12, 13, 0x02);

// Callback function to handle incoming packets
void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Received from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);

    // Send an acknowledgment back to the sender
    bus.sendPacket(packet.sender, "ACK from 0x02", 14);
}

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    bus.begin(9600);  // Initialize dwiBus communication
    bus.onPacketReceived(onPacketReceived);  // Register the callback function

    Serial.println("Client 2 (0x02) is ready.");
}

void loop() {
    // Send a message to the device with address 0x01
    if (bus.sendPacket(0x01, "Hello from 0x02", 15)) {
        Serial.println("Message sent to 0x01.");
    } else {
        Serial.println("Failed to send message to 0x01.");
    }

    // Check for incoming packets
    bus.handleReceive();

    delay(5000);  // Wait for 5 seconds before sending again
}
