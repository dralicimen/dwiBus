#ifndef DWIBUS_H
#define DWIBUS_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// Paket yapısı: dwiPacket
struct dwiPacket {
    uint8_t sync[2] = {0xAA, 0x55};  // Senkronizasyon baytları
    uint16_t receiver;  // Varış Adresi
    uint16_t sender;    // Gönderen Adresi
    uint16_t length;    // Veri Uzunluğu
    char data[50];      // Veri (Maks. 50 byte)
    uint8_t crc;        // CRC-8
    uint8_t end[2] = {0x0D, 0x0A};  // Bitiş baytları
};

// Haberleşme sınıfı: dwiBus
class dwiBus {
public:
    dwiBus(int rxPin, int txPin, int busPin, uint16_t deviceAddress);  // Kurucu

    void begin(long baudRate);  // Haberleşmeyi başlat
    void onPacketReceived(void (*callback)(const dwiPacket&));  // Callback kaydet

    bool sendPacket(uint16_t receiver, const char* data, uint16_t length, unsigned long timeout = 1000);  // Paket gönder
    void handleReceive();  // Gelen paketleri kontrol et

private:
    int busPin;  // Hat durumu için pin
    uint16_t address;  // Cihaz adresi
    SoftwareSerial serial;  // SoftwareSerial nesnesi
    void (*packetCallback)(const dwiPacket&);  // Kullanıcı callback'i

    bool waitForBus(unsigned long timeout);  // Hat müsait olana kadar bekle
    uint8_t calculateCRC(const uint8_t* data, size_t length);
    bool checkCRC(const dwiPacket& packet);
};

#endif
