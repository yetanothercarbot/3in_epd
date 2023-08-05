/*
    EPD_3in: A library for driving Waveshare's 4-colour 3 inch e-paper display.
*/
#ifndef EPD_3IN_H
#define EPD_3IN_H

#include "Arduino.h"

class EPD {
    public:
        EPD(uint8_t cs_pin, uint8_t dc_pin, uint8_t rst_pin, uint8_t busy_pin);
        enum Colour {BLACK = 0b00, WHITE = 0b01, YELLOW = 0b10, RED = 0b11};
        void reset();
        void send_command(uint8_t cmd);
        void send_data(uint8_t data);
        void wait_for_idle(); // Wait for idle
        void wait_for_busy(); // Wait for busy
        void refresh();
        void init();
        void display(const uint8_t *buf);
        void display(uint8_t *buf);
        void clear();
        void clear(Colour colour);
        void test_pattern();
        void sleep();
        
    private:
        uint8_t _cs, _dc, _rst, _busy;
        uint16_t _w = 168, _h = 400, _buf_size = 16800;
        
};

#endif