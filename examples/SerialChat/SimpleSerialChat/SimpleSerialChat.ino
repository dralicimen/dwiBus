#include <dwiBus.h>

// RX = 11, TX = 12, Kontrol Pini = 13, Cihaz Adresi = 0x01
dwiBus bus(11, 12, 13, 0x01);  

uint8_t chatPartnerAddress = 0x02;  // Diğer cihazın adresi

// Gelen mesajlar için callback fonksiyonu
void onPacketReceived(const dwiPacket& packet) {
    Serial.print("Message from 0x");
    Serial.print(packet.sender, HEX);
    Serial.print(": ");
    Serial.println(packet.data);
}

void setup() {
    Serial.begin(9600);  // Debugging
    bus.begin(9600);  // dwiBus başlat
    bus.onPacketReceived(onPacketReceived);  // Callback fonksiyonunu kaydet



    Serial.println("Nano (0x01) hazır. Mesajınızı girin:");
}

void loop() {
    bus.handleReceive();  // Gelen mesajları kontrol et

    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');
        message.trim();  // Mesajı temizle

        if (bus.sendPacket(chatPartnerAddress, message.c_str(), message.length())) {

            Serial.println("Mesaj gönderildi.");
        } else {
            Serial.println("Hat meşgul, mesaj gönderilemedi.");
        }
    }
}
