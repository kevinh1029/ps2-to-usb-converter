#include "ps2.h"
#include "main.h"


extern uint8_t ps2_read_buffer;
extern uint8_t ps2_read_buffer_done;
extern uint8_t parity;
extern uint8_t ps2_write;
extern uint8_t ps2_write_buffer;
extern uint8_t ps2_write_buffer_done;

void PS2_Read(void) {
  static uint8_t bit_count = 0;
  static uint8_t data = 0;

  GPIO_PinState read_bit = HAL_GPIO_ReadPin(PS2DATA_GPIO_Port, PS2DATA_Pin);

  if (bit_count == 0) {
    if (read_bit != GPIO_PIN_RESET) {
      //reset
      return;
    }
    if (ps2_read_buffer_done !=0) {
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
      ps2_read_buffer = data;
      ps2_read_buffer_done = 1;//need to look at this more carefully
    } else {
      //reset
    }
    bit_count = 0;
    data = 0;
    return;
  }
  bit_count++;
}

void PS2_Write(void) {
  static uint8_t bit_count = 0;
  static uint8_t temp_parity;

  if (bit_count < 8) 
  {
    HAL_GPIO_WritePin(PS2DATA_GPIO_Port, PS2DATA_Pin, (ps2_write_buffer & (1 << (bit_count - 1))) ? 1 : 0);
  }
  else if (bit_count == 9) {
    temp_parity = ps2_write_buffer;
    temp_parity ^= (temp_parity >> 4);
    temp_parity ^= (temp_parity >> 2);
    temp_parity ^= (temp_parity >> 1);
    HAL_GPIO_WritePin(PS2DATA_GPIO_Port, PS2DATA_Pin, !(temp_parity&1));
  }
  else if (bit_count == 10){
    HAL_GPIO_WritePin(PS2DATA_GPIO_Port, PS2DATA_Pin, 1);
    PS2DATA_GPIO_Port->MODER &= GPIO_MODER_MODER10_Msk;
  }
  else {
    if(!HAL_GPIO_ReadPin(PS2DATA_GPIO_Port, PS2DATA_Pin)) {
      ps2_write = 0;
      ps2_write_buffer_done = 1;
    }
    else{
      //reset
    }
    bit_count = 0;

    return;
  }
  bit_count++;
}
