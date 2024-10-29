#include <dwiBus.h>

// dwiBus object: RX = 11, TX = 12, control pin = 13, device address = 0x02
dwiBus bus(11, 12, 13, 0x02);  

// Response nesnesi işaretçisi
Response* responsePtr = nullptr;

// Callback function to handle incoming packets
void onPacketReceived(const Response& response) {
    if (responsePtr) delete responsePtr;  // Eski response'ı temizle
    responsePtr = new Response(response);  // Yeni response'ı kaydet

    Serial.print("Received from 0x");
    Serial.print(response.sender, HEX);
    Serial.print(": ");
    Serial.write(response.completeData, response.totalLength);
    Serial.println();

    // ACK gönder
    bus.sendPacket(response.sender, "ACK from 0x02");
}

void setup() {
    Serial.begin(9600);  // Seri haberleşme başlatılır
    bus.begin(9600);  // dwiBus başlatılır
    bus.onPacketReceived(onPacketReceived);  // Callback fonksiyonu kaydedilir

    Serial.println("Client 2 (0x02) is ready.");
}

void loop() {
    // Cihaza mesaj gönder
    if (bus.sendPacket(0x01, "Hello from 0x02")) {
        Serial.println("Message sent to 0x01.");
    } else {
        Serial.println("Failed to send message to 0x01.");
    }

    // Gelen paketleri kontrol et
    bus.handleReceive();

    delay(5000);  // 5 saniye bekle
}
