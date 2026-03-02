/*
* screen.c
*
*  Created on: Feb 19, 2026
*      Author: ethee
*/
#include "screen.h"
#include "lvgl/lvgl.h"
#include <math.h>
#include <stdio.h>
#include "Adafruit_Zero_FFT_Library/Adafruit_ZeroFFT.h"
#include "Adafruit_ZeroFFT.h"
#include "adc_shared.h"
#define SAMPLE_RATE 48000.0f
#define EQ_WIDTH 320
#define EQ_HEIGHT 240
#define FREQ_MIN 20.0f		// 20Hz
#define FREQ_MAX 20000.0f	// 20kHz
#define FFT_SIZE 1024
#define NUM_BANDS 32
#define BINS_PER_BAND 16
static float band_values[NUM_BANDS];
static volatile uint16_t fft_index = 0;
static volatile uint8_t fft_ready = 0;
static lv_obj_t * eq_bg;
static lv_obj_t * bands[NUM_BANDS];
static int16_t realbuf[FFT_SIZE];
#define ADC_CHANNEL_COUNT 4
#define NUM_BINS (FFT_SIZE / 2)
float magnitude[NUM_BINS];


static int freq_to_x(float freq, int width) // math to turn frequency to plotted x-value
{
	float log_min = log10f(FREQ_MIN);
	float log_max = log10f(FREQ_MAX);
	float log_f = log10f(freq);
	float norm = (log_f - log_min) / (log_max - log_min);
	return (int)(norm * EQ_WIDTH);
}
static int db_to_y(float db, int height) // math to turn dB into plotted y-value
{
   float max_db = -12.0f;
   float min_db = 12.0f;
   float norm = (db - min_db) / (max_db - min_db);
   return (int)(norm * EQ_HEIGHT);
}
static void eq_draw_event_cb(lv_event_t * e) // called to initialize graph lines
{
   lv_obj_t * obj = lv_event_get_target(e);
   lv_layer_t * layer = lv_event_get_layer(e);
   int width  = lv_obj_get_width(obj);
   int height = lv_obj_get_height(obj);
   lv_draw_line_dsc_t line;
   lv_draw_line_dsc_init(&line);
   line.color = lv_color_hex(0x303030);
   line.width = 1;
   float freqs[] = {20,50,100,200,500,1000,2000,5000,10000}; // frequency lines present
   for(int i = 0; i < 9; i++) // cycles through and plots each frequency line
   {
       int x = freq_to_x(freqs[i], width); // calls x-value function from earlier
       line.p1.x = x;
       line.p1.y = 0;
       line.p2.x = x;
       line.p2.y = height;
       lv_draw_line(layer, &line);
   }
   for(int i = 0; i <= 8; i++) // cycles through and plot dB lines
   {
       int y = (height * i) / 8;
       line.p1.x = 0;
       line.p1.y = y;
       line.p2.x = width;
       line.p2.y = y;
       lv_draw_line(layer, &line);
   }
}
void Screen_Init(void) // this function is called in main
{
	eq_bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(eq_bg, 320, 240);
	lv_obj_set_pos(eq_bg, 0, 0);
	lv_obj_set_style_bg_color(eq_bg, lv_color_hex(0x04041C), 0);
	lv_obj_set_style_border_width(eq_bg, 0, 0);
	lv_obj_add_event_cb(eq_bg, eq_draw_event_cb, LV_EVENT_DRAW_MAIN, NULL);
	// ideally removes scroll wheel for off-screen content
	lv_obj_clear_flag(eq_bg, LV_OBJ_FLAG_SCROLLABLE);
	int width  = lv_obj_get_width(eq_bg);
	int height = lv_obj_get_height(eq_bg);
	float db_values[] = {6, 0, -6}; // dB labels that get plotted
	for(int i = 0; i < 3; i++) // plotting dB labels
	{
	    lv_obj_t * label = lv_label_create(eq_bg);
	    char buf[8];
	    sprintf(buf, "%d", (int)db_values[i]);
	    lv_label_set_text(label, buf);
	    lv_obj_set_style_text_color(label, lv_color_hex(0x808080), LV_PART_MAIN);
	    int y = db_to_y(db_values[i], height);
	    lv_obj_set_pos(label, 0, y - 8);
	}
	float freqs[] = {50, 100, 1000, 2000, 10000}; // frequency labels that get plotted
	for(int i = 0; i < 5; i++)
	{
	    lv_obj_t * label = lv_label_create(eq_bg);
	    char buf[10];
	    if(freqs[i] >= 1000) // turns thousand values into "--k"
	        sprintf(buf, "%dk", (int)(freqs[i] / 1000));
	    else
	        sprintf(buf, "%d", (int)freqs[i]);
	    lv_label_set_text(label, buf);
	    lv_obj_set_style_text_color(label, lv_color_hex(0x808080), LV_PART_MAIN);
	    int x = freq_to_x(freqs[i], width);
	    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 0, +10);
	    lv_obj_set_x(label, x - lv_obj_get_width(label) / 3);
	}
	for (int i = 0; i < NUM_BANDS; i++)
	{
		bands[i] = lv_obj_create(eq_bg);
		lv_obj_set_size(bands[i], 5, 150);
		lv_obj_set_style_bg_color(bands[i], lv_color_hex(0xFF0000), 0);
		lv_obj_align(bands[i], LV_ALIGN_BOTTOM_LEFT, 0, +15);
		band_values[i] = 0.0f;
	}
}
void Screen_Add_Sample(void)
{
	for (int i = 0; i < FFT_SIZE; i++)
	{
		int16_t centered = (int16_t)adc_buf[0] - 32768;
		realbuf[fft_index++] = centered;

	}
	if (fft_index >= FFT_SIZE)
	{
		fft_index = 0;
		fft_ready = 1;
	}
}
void Compute_FFT(void)
{
	if (fft_ready)
	{
		fft_ready = 0;
		ZeroFFT(realbuf, FFT_SIZE);
		for (int k = 0; k < NUM_BINS; k++)
		{
			float real = realbuf[2*k];
			float imag = realbuf[2*k + 1];
			magnitude[k] = sqrtf(real * real + imag * imag);
		}

		for (int band = 0; band < NUM_BANDS; band++)
			{
				float sum = 0;
				for (int i = 0; i < BINS_PER_BAND; i++)
				{
					int bin = band * BINS_PER_BAND + i;
					sum += magnitude[bin];
				}
				float avg = sum / BINS_PER_BAND;
				band_values[band] = 20.0f * log10f(avg + 1e-6f);
			}
	}
}

// this is where the constantly updating code will go
void Screen_Update(void)
{
	int height = lv_obj_get_height(eq_bg);
	    for (int i = 0; i < NUM_BANDS; i++)
	    {
	        float db = band_values[i];

	        int bar_height = db * 4;
	        if (bar_height < 10) bar_height = 10;
	        if (bar_height > 200) bar_height = 200;
	        lv_obj_set_height(bands[i], bar_height);
	        lv_obj_set_x(bands[i], i * 8);
	    }
}

