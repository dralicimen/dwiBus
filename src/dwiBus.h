#ifndef DWIBUS_H
#define DWIBUS_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// Paket yapısı: dwiPacket
struct dwiPacket {
    uint8_t sync[2] = {0xAA, 0x55};  // Senkronizasyon baytları
    uint8_t receiver;
    uint8_t sender;
    uint8_t packetNum;
    uint8_t totalPackets;
    uint8_t length;
    char data[50];  // Maksimum 50 byte veri
    uint8_t crc;
    uint8_t end[2] = {0x0D, 0x0A};
};

// Yanıt yapısı: Response
class Response {
public:
    char* completeData;  // Gelen tam veri
    uint16_t totalLength;  // Gelen verinin toplam uzunluğu
    uint8_t sender;  // Veriyi gönderen cihazın adresi
    unsigned long receiveTime;  // Gelen mesajın alındığı zaman

    // Yapıcı: Dinamik bellek tahsisi yapar
    Response(uint16_t length) {
        completeData = new char[length];
        totalLength = length;
    }

    // Kopya yapıcı: Belleği kopyalar
    Response(const Response& other) {
        totalLength = other.totalLength;
        sender = other.sender;
        receiveTime = other.receiveTime;
        completeData = new char[totalLength];
        memcpy(completeData, other.completeData, totalLength);
    }

    // Yıkıcı: Belleği serbest bırakır
    ~Response() {
        delete[] completeData;
    }
};

// Haberleşme sınıfı: dwiBus
class dwiBus {
public:
    dwiBus(int rxPin, int txPin, int busPin, uint8_t deviceAddress);

    void begin(long baudRate);
    void onPacketReceived(void (*callback)(const Response&));

    bool sendPacket(uint8_t receiver, const char* data, unsigned long timeout = 1000);
    void handleReceive();

private:
    int busPin;
    uint8_t address;
    SoftwareSerial serial;
    void (*packetCallback)(const Response&);

    bool waitForBus(unsigned long timeout);
    uint8_t calculateCRC(const uint8_t* data, size_t length);
    bool checkCRC(const dwiPacket& packet);
};

#endif
