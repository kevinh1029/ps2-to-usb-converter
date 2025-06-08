#ifndef PS2USB_MAP_H
#define PS2USB_MAP_H


#include <stdint.h>

extern const uint8_t ps2_to_usb_map[256];

uint8_t ps2_to_usb(uint8_t ps2_code);

#endif
