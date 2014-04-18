/*
 * myaudio.h
 *
 *  Created on: 2014-03-11
 *      Author: Ehsan
 */

#ifndef MYAUDIO_H_
#define MYAUDIO_H_

#include "sys/alt_irq.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include <stdio.h>
#include <io.h>

#define sample_size 11000
//Note: this used in myaudio.h so it has to be before the include.
#define NUM_OF_MUSIC 4

unsigned int test2[1000];
unsigned int test3[100000];
unsigned int * fifo_data_ptr;
//unsigned int ** LOOP_GRID_ptr;

volatile int counter ;
 alt_up_av_config_dev * av_dev;
 alt_up_audio_dev *audio_dev;


//unsigned int test [100];
void av_config_setup();
void audio_setup(alt_up_audio_dev * audio_dev) ;
void handle_audio_interrupts(void* context, alt_u32 id) ;
void init_audio_interrupt();
int getAddress();
#endif /* MYAUDIO_H_ */
