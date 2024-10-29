#include <dwiBus.h>

// Initialize dwiBus with RX = 11, TX = 12, Control Pin = 13, Device Address = 0x02
dwiBus bus(11, 12, 13, 0x01);  

// Response nesnesi işaretçisi
Response* responsePtr = nullptr;

uint8_t chatPartnerAddress = 0x02;  // Sohbet partnerinin adresi

// Callback: Gelen veri tamamlandığında çalışır
void onResponseReceived(const Response& response) {
    // Önceki yanıt belleğini temizle
    if (responsePtr) delete responsePtr;  

    // Gelen yanıt için yeni dinamik bellek ayır ve kopyala
    responsePtr = new Response(response);
}

void setup() {
    Serial.begin(9600);  // Serial haberleşmeyi başlat
    bus.begin(9600);  // dwiBus haberleşmesini başlat
    bus.onPacketReceived(onResponseReceived);  // Callback'i kaydet

    Serial.println("Device 0x02 is ready. Enter your message:");
}

void loop() {
    bus.handleReceive();  // Gelen mesajları kontrol et

    // Eğer yeni bir yanıt geldiyse, ekrana yazdır ve belleği temizle
    if (responsePtr && responsePtr->totalLength > 0) {
        Serial.print("Received from 0x");
        Serial.print(responsePtr->sender, HEX);
        Serial.print(": ");
        Serial.write(responsePtr->completeData, responsePtr->totalLength);
        Serial.println();

        // Bellek temizleme işlemi
        delete responsePtr;  
        responsePtr = nullptr;
    }

    // Kullanıcıdan mesaj geldi mi kontrol et
    if (Serial.available() > 0) {
        // Kullanıcıdan gelen mesajı oku
        String message = Serial.readStringUntil('\n');
        message.trim();  // Mesajın başı-sonundaki boşlukları temizle

        // Mesajı karşı cihaza gönder
        if (bus.sendPacket(chatPartnerAddress, message.c_str())) {
            Serial.println("Message sent.");
        } else {
            Serial.println("Bus busy, message not sent.");
        }
    }
}
