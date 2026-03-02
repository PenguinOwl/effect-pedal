/*
 * encoders.h
 *
 *  Created on: Feb 4, 2026
 *      Author: edith
 */

#ifndef INC_ENCODERS_H_
#define INC_ENCODERS_H_

#include "main.h"

#define ENCODER_COUNT 4
#define DEBOUNCE_DELAY 20

typedef struct encoder_t {
	uint16_t gpio_pin_left;
	GPIO_TypeDef *gpio_port_left;
	uint16_t gpio_pin_right;
	GPIO_TypeDef *gpio_port_right;
	uint16_t gpio_pin_button;
	GPIO_TypeDef *gpio_port_button;

	int16_t spin_buf;
	uint8_t pressed;

	uint8_t _state;
	uint32_t _last_press;
} encoder_t;

void init_encoders();
encoder_t *get_encoders();
void update_encoders();

#endif /* INC_ENCODERS_H_ */
