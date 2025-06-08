#include "ps2.h"
#include "main.h"


extern uint8_t ps2_buffer;
extern uint8_t ps2_buffer_ready;
extern uint8_t parity;

void PS2_Read(void) {
  static uint8_t bit_count = 0;
  static uint8_t data = 0;

  GPIO_PinState read_bit = HAL_GPIO_ReadPin(PS2DATA_GPIO_Port, PS2DATA_Pin);

  if (bit_count == 0) {
    if (read_bit != GPIO_PIN_RESET) {
      //reset
      return;
    }
    if (ps2_buffer_ready !=0) {
      //reset
    }
  } else if (bit_count <= 8) {
    if (read_bit == GPIO_PIN_SET) {
      data |= (1 << bit_count);
    }
  } else if (bit_count == 9) {
    parity = (read_bit == GPIO_PIN_SET ? 1 : 0);
  } else if (bit_count == 10) {
    if (read_bit == GPIO_PIN_SET) {
      ps2_buffer = data;
      ps2_buffer_ready = 1;
    } else {
      //reset
    }
    bit_count = 0;
    data = 0;
    return;
  }
  bit_count++;
}
