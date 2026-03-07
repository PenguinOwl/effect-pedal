/*
 * screen.h
 *
 *  Created on: Feb 21, 2026
 *      Author: ethee
 */

#ifndef INC_SCREEN_H_
#define INC_SCREEN_H_

#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include <stdint.h>

void Screen_Init(void);
void Screen_Update(void);
void Screen_Add_Sample(void);
void Compute_FFT(void);

#endif /* INC_SCREEN_H_ */
