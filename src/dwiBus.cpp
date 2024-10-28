#include "dwiBus.h"

// Kurucu
dwiBus::dwiBus(int rxPin, int txPin, int busPin, uint8_t deviceAddress)
    : serial(rxPin, txPin), packetCallback(nullptr) {
    this->busPin = busPin;
    this->address = deviceAddress;
    pinMode(busPin, OUTPUT);
    digitalWrite(busPin, LOW);
}

// Haberleşmeyi başlatır
void dwiBus::begin(long baudRate) {
    serial.begin(baudRate);
}

// Callback fonksiyonunu kaydeder
void dwiBus::onPacketReceived(void (*callback)(const dwiPacket&)) {
    packetCallback = callback;
}


// CRC kontrolü
uint8_t dwiBus::calculateCRC(const uint8_t* data, size_t length) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : crc << 1;
        }
    }
    return crc;
}
bool dwiBus::checkCRC(const dwiPacket& packet) {
    size_t lengthToCheck = sizeof(packet) - sizeof(packet.crc) - sizeof(packet.end);
    uint8_t calculatedCRC = calculateCRC((uint8_t*)&packet, lengthToCheck);
    return calculatedCRC == packet.crc;
}



// Hat bekleme
bool dwiBus::waitForBus(unsigned long timeout) {
    unsigned long startTime = millis();
    while (digitalRead(busPin) == HIGH) {
        if (millis() - startTime >= timeout) {
            Serial.println("Timeout: Hat meşgul.");
            return false;
        }
    }
    digitalWrite(busPin, HIGH);
    return true;
}

// Küçük paket gönderme
bool dwiBus::sendPacket(uint8_t receiver, const char* data, uint8_t length, unsigned long timeout) {
    return sendLargePacket(receiver, data, length, timeout);
}

// Büyük paket gönderme
bool dwiBus::sendLargePacket(uint8_t receiver, const char* data, uint16_t totalLength, unsigned long timeout) {
    uint8_t totalPackets = (totalLength + 49) / 50;
    uint8_t packetNum = 0;

    for (uint16_t offset = 0; offset < totalLength; offset += 50) {
        uint8_t length = (totalLength - offset > 50) ? 50 : totalLength - offset;

        dwiPacket packet;  // Yapıyı oluştur
        memset(&packet, 0, sizeof(packet));  // Yapıyı sıfırla

        packet.receiver = receiver;
        packet.sender = address;
        packet.packetNum = packetNum++;
        packet.totalPackets = totalPackets;
        packet.length = length;

        memcpy(packet.data, data + offset, length);

        packet.crc = calculateCRC((uint8_t*)&packet, sizeof(packet) - sizeof(packet.crc) - sizeof(packet.end));

        if (!waitForBus(timeout)) return false;

        serial.write((uint8_t*)&packet, sizeof(packet));
        serial.flush();
        digitalWrite(busPin, LOW);
    }
    return true;
}


// Gelen paketleri alır
void dwiBus::handleReceive() {
    static char buffer[1024];  // Büyük veri için geçici buffer
    static uint8_t receivedPackets = 0;
    static uint8_t expectedPackets = 0;
    static uint16_t receivedLength = 0;

    if (serial.available() >= sizeof(dwiPacket)) {
        dwiPacket packet;
        serial.readBytes((uint8_t*)&packet, sizeof(dwiPacket));

        // CRC ve adres kontrolü
        if (checkCRC(packet) && packet.receiver == address) {
            // Gelen veriyi buffer'a ekle
            memcpy(buffer + (packet.packetNum * 50), packet.data, packet.length);
            receivedLength += packet.length;
            receivedPackets++;

            // İlk paket geldiğinde toplam paket sayısını ayarla
            if (packet.packetNum == 0) expectedPackets = packet.totalPackets;

            // Tüm paketler geldiyse
            if (receivedPackets == expectedPackets) {
                Serial.print("Tam veri alındı: ");
                Serial.write(buffer, receivedLength);  // Tam veriyi yazdır

                // Tüm değişkenleri sıfırla
                receivedLength = 0;
                receivedPackets = 0;
                expectedPackets = 0;
            }
        } else {
            Serial.println("Hata: CRC hatası veya adres uyuşmazlığı.");
        }
    }
}

