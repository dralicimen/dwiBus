#ifndef DWIBUS_H
#define DWIBUS_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// Paket yapısı: dwiPacket
struct dwiPacket {
    uint8_t sync[2] = {0xAA, 0x55};
    uint8_t receiver;
    uint8_t sender;
    uint8_t packetNum;
    uint8_t totalPackets;
    uint8_t length;
    char data[50];  // Maksimum 50 byte
    uint8_t crc;
    uint8_t end[2] = {0x0D, 0x0A};
};

// Haberleşme sınıfı: dwiBus
class dwiBus {
public:
    dwiBus(int rxPin, int txPin, int busPin, uint8_t deviceAddress);

    void begin(long baudRate);
    void onPacketReceived(void (*callback)(const dwiPacket&));

    bool sendPacket(uint8_t receiver, const char* data, uint8_t length, unsigned long timeout = 1000);
    bool sendLargePacket(uint8_t receiver, const char* data, uint16_t totalLength, unsigned long timeout = 1000);

    void handleReceive();

private:
    int busPin;
    uint8_t address;
    SoftwareSerial serial;
    void (*packetCallback)(const dwiPacket&);

    bool waitForBus(unsigned long timeout);
    uint8_t calculateCRC(const uint8_t* data, size_t length);
    bool checkCRC(const dwiPacket& packet);
};

#endif
