#include "dwiBus.h"

// Kurucu: SoftwareSerial nesnesi ve cihaz bilgilerini ayarlar
dwiBus::dwiBus(int rxPin, int txPin, int busPin, uint16_t deviceAddress)
    : serial(rxPin, txPin), packetCallback(nullptr) {
    this->busPin = busPin;
    this->address = deviceAddress;
    pinMode(busPin, OUTPUT);
    digitalWrite(busPin, LOW);  // Hat başlangıçta müsait
}

// Haberleşmeyi başlatır
void dwiBus::begin(long baudRate) {
    serial.begin(baudRate);  // SoftwareSerial başlat
}

// Kullanıcı callback fonksiyonunu kaydeder
void dwiBus::onPacketReceived(void (*callback)(const dwiPacket&)) {
    packetCallback = callback;
}

// CRC hesaplama fonksiyonu
uint8_t dwiBus::calculateCRC(const uint8_t* data, size_t length) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];  // XOR işlemi
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;  // Polinom: x^8 + x^2 + x + 1 (0x07)
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}


// CRC kontrolü
bool dwiBus::checkCRC(const dwiPacket& packet) {
    uint8_t calculatedCRC = calculateCRC((uint8_t*)&packet, sizeof(packet) - 3);  // Son 3 bayt hariç
    return calculatedCRC == packet.crc;
}


// Hat müsait olana kadar bekler
bool dwiBus::waitForBus(unsigned long timeout) {
    unsigned long startTime = millis();
    while (digitalRead(busPin) == HIGH) {
        if (millis() - startTime >= timeout) {
            Serial.println("Timeout: Hat meşgul.");
            return false;
        }
    }
    digitalWrite(busPin, HIGH);  // Hat meşgul yap
    return true;
}

// Paket gönderme fonksiyonu
bool dwiBus::sendPacket(uint16_t receiver, const char* data, uint16_t length, unsigned long timeout) {
    if (length > 50) {
        Serial.println("Hata: Veri uzunluğu 50 byte'ı geçemez.");
        return false;
    }

    if (!waitForBus(timeout)) {
        return false;
    }

    dwiPacket packet;
    packet.receiver = receiver;
    packet.sender = address;
    packet.length = length;
    memset(packet.data, 0, sizeof(packet.data));
    memcpy(packet.data, data, length);
    packet.crc = calculateCRC((uint8_t*)&packet, sizeof(packet) - 3);  // CRC hesapla

    serial.write((uint8_t*)&packet, sizeof(packet));
    serial.flush();
    digitalWrite(busPin, LOW);
    return true;
}


// Gelen paketleri kontrol eder ve callback'i çağırır
void dwiBus::handleReceive() {
    if (serial.available() >= sizeof(dwiPacket)) {
        dwiPacket packet;
        serial.readBytes((uint8_t*)&packet, sizeof(dwiPacket));

        if (packet.length > 50) {
            Serial.println("Hata: Geçersiz veri uzunluğu.");
            return;
        }

        if (packet.receiver == address && checkCRC(packet)) {
            if (packetCallback != nullptr) {
                packetCallback(packet);
            }
        } else {
            Serial.println("Hata: CRC hatası veya adres uyuşmazlığı.");
        }
    }
}

