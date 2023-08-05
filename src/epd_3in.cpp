#include "Arduino.h"
#include "SPI.h"
#include "epd_3in.h"

EPD::EPD (uint8_t cs_pin, uint8_t dc_pin, uint8_t rst_pin, uint8_t busy_pin) {
    _cs = cs_pin;
    _dc = dc_pin;
    _rst = rst_pin;
    _busy = busy_pin;

}

void EPD::reset() {
    digitalWrite(_rst, HIGH);
    delay(20);
    digitalWrite(_rst, LOW);
    delay(2);
    digitalWrite(_rst, HIGH);
    delay(20);
}

void EPD::send_command(uint8_t cmd) {
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(cmd);
    SPI.endTransaction();

    digitalWrite(_cs, HIGH);
}

void EPD::send_data(uint8_t data) {
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(data);
    SPI.endTransaction();

    digitalWrite(_cs, HIGH);
}

void EPD::wait_for_idle() {
    while (digitalRead(_busy) == LOW) delay(5);
}

void EPD::wait_for_busy() {
    while (digitalRead(_busy) == HIGH) delay(5);
}

void EPD::refresh() {
    send_command(0x12); // DISPLAY_REFRESH
    send_data(0x01);
    wait_for_idle();

    send_command(0x02); //POWER_OFF
    send_data(0x00);
    wait_for_idle();
}

void EPD::init() {
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_rst, OUTPUT);
    pinMode(_busy, INPUT);

    SPI.begin(); // ESP32-S3 defaults: MOSI 11, MISO 13, SCK 12

    reset();

    send_command(0x66);
    send_data(0x49);
    send_data(0x55);
    send_data(0x13);
    send_data(0x5D);
    send_data(0x05);
    send_data(0x10);

    send_command(0xB0);
    send_data(0x00); // 1 boost

    send_command(0x01);
    send_data(0x0F);
    send_data(0x00);

    send_command(0x00);
    send_data(0x4F);
    send_data(0x6B);

    send_command(0x06);
    send_data(0xD7);
    send_data(0xDE);
    send_data(0x12);

    send_command(0x61);
    send_data(0x00);
    send_data(0xA8);
    send_data(0x01);
    send_data(0x90);

    send_command(0x50);
    send_data(0x37);

    send_command(0x60);
    send_data(0x0C);
    send_data(0x05);

    send_command(0xE3);
    send_data(0xFF);

    send_command(0x84);
    send_data(0x00);
}

void EPD::display(const uint8_t *buf) {
    send_command(0x04);
    wait_for_idle();

    send_command(0x10);
    for (uint32_t i = 0; i < _buf_size; i++) {
        send_data(buf[i]);
    }
    refresh();
}

void EPD::display(uint8_t *buf) {
    send_command(0x04);
    wait_for_idle();

    send_command(0x10);
    for (uint32_t i = 0; i < _buf_size; i++) {
        send_data(buf[i]);
    }
    refresh();
}

void EPD::clear() {
    clear(WHITE);
}

void EPD::clear(Colour colour) {
    send_command(0x04);
    wait_for_idle();

    send_command(0x10);
    for (int row = 0; row < _h; row++) {
        for (int col = 0; col < _w; col++) {
            send_data(colour << 6 | colour << 4 | colour << 2 | colour);
        }
    }

    refresh();
}

void EPD::test_pattern() {
    send_command(0x04);
    wait_for_idle();

    send_command(0x10);

    for (int row = 0; row < _h; row++) {
        for (int col = 0; col < _w; col++) {
            send_data(BLACK << 6 | RED << 4 | YELLOW << 2 | WHITE);
        }
    }

    refresh();
}