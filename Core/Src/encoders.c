/*
 * encoders.c
 *
 *  Created on: Feb 4, 2026
 *      Author: edith
 */

#include "encoders.h"

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

// Default start state
#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

encoder_t encoders[ENCODER_COUNT];

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

void init_encoders() {
	encoder_t encoder1;
	encoder1.gpio_pin_button  = ENC1_B_Pin;
	encoder1.gpio_port_button = ENC1_B_GPIO_Port;
	encoder1.gpio_pin_left    = ENC1_L_Pin;
	encoder1.gpio_port_left   = ENC1_L_GPIO_Port;
	encoder1.gpio_pin_right   = ENC1_R_Pin;
	encoder1.gpio_port_right  = ENC1_R_GPIO_Port;
	encoder_t encoder2;
	encoder2.gpio_pin_button  = ENC2_B_Pin;
	encoder2.gpio_port_button = ENC2_B_GPIO_Port;
	encoder2.gpio_pin_left    = ENC2_L_Pin;
	encoder2.gpio_port_left   = ENC2_L_GPIO_Port;
	encoder2.gpio_pin_right   = ENC2_R_Pin;
	encoder2.gpio_port_right  = ENC2_R_GPIO_Port;
	encoder_t encoder3;
	encoder3.gpio_pin_button  = ENC3_B_Pin;
	encoder3.gpio_port_button = ENC3_B_GPIO_Port;
	encoder3.gpio_pin_left    = ENC3_L_Pin;
	encoder3.gpio_port_left   = ENC3_L_GPIO_Port;
	encoder3.gpio_pin_right   = ENC3_R_Pin;
	encoder3.gpio_port_right  = ENC3_R_GPIO_Port;
	encoder_t encoder4;
	encoder4.gpio_pin_button  = ENC4_B_Pin;
	encoder4.gpio_port_button = ENC4_B_GPIO_Port;
	encoder4.gpio_pin_left    = ENC4_L_Pin;
	encoder4.gpio_port_left   = ENC4_L_GPIO_Port;
	encoder4.gpio_pin_right   = ENC4_R_Pin;
	encoder4.gpio_port_right  = ENC4_R_GPIO_Port;
	encoders[0] = encoder1;
	encoders[1] = encoder2;
	encoders[2] = encoder3;
	encoders[3] = encoder4;
	for (int i = 0; i < ENCODER_COUNT; i++) {
			encoder_t *encoder = encoders + i;
			encoder->spin_buf = 0;
			encoder->pressed = 0;
			encoder->_last_press = 0;
			encoder->_state = R_START;
	}
}

void update_encoders() {
	uint32_t current_time = HAL_GetTick();
	for (int i = 0; i < ENCODER_COUNT; i++) {
		encoder_t *encoder = encoders + i;
		uint8_t pin0 = HAL_GPIO_ReadPin(encoder->gpio_port_right, encoder->gpio_pin_right) == GPIO_PIN_SET;
		uint8_t pin1 = HAL_GPIO_ReadPin(encoder->gpio_port_left, encoder->gpio_pin_left) == GPIO_PIN_SET;
		uint8_t new_pressed = HAL_GPIO_ReadPin(encoder->gpio_port_button, encoder->gpio_pin_button) == GPIO_PIN_RESET;

		if (new_pressed) {
			if (encoder->pressed)
				encoder->pressed_time += current_time - encoder->_last_press;
			encoder->_last_press = current_time;
			encoder->pressed = 1;
		} else if (encoder->pressed) {
			if (encoder->_last_press < current_time - DEBOUNCE_DELAY) {
				encoder->pressed = 0;
				encoder->pressed_time = 0;
			}
		}

		uint8_t pinstate = (pin1 << 1) | pin0;
		  // Determine new state from the pins and state table.
		encoder->_state = ttable[encoder->_state & 0xf][pinstate];
		uint8_t event = encoder->_state & 0x30;
		if (event == DIR_CW)
			encoder->spin_buf += 1;
		else if (event == DIR_CCW)
			encoder->spin_buf -= 1;
	}
}

encoder_t *get_encoders() {
	return encoders;
}
