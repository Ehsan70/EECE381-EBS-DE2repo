/*
 * myaudio.c
 *
 *  Created on: 2014-03-13
 *      Author: Ehsan
 */

#include "myvga.h"
#include "myaudio.h"

int* x_pos;

void av_config_setup()
{
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_video");
	if (!av_config) printf("error opening\n");
	while (!alt_up_av_config_read_ready(av_config))
	{
	}
}

int verbose_nios2 = 1;

void audio_setup(alt_up_audio_dev * audio_dev)
{
	//Initialising the audio IP Core HAL device driver.
	audio_dev = alt_up_audio_open_dev("/dev/audio");
	if (audio_dev == NULL)
	{
		if (verbose_nios2) printf("Error: could not open audio device \n");
	}
	else
	{
		if (verbose_nios2) printf("Opened audio device \n");
	}
}

void handle_audio_interrupts(void* context,alt_u32 id)
{
	int spaceR;
	int spaceL;
	int min = 0;
	static int num_played_samples = 0;
	spaceR = alt_up_audio_write_fifo_space(audio_dev,ALT_UP_AUDIO_RIGHT);
	spaceL = alt_up_audio_write_fifo_space(audio_dev,ALT_UP_AUDIO_LEFT);

	min = (spaceR < spaceL) ? spaceR : spaceL;

	alt_up_audio_write_fifo(audio_dev,&fifo_data_ptr[num_played_samples],min,ALT_UP_AUDIO_RIGHT);
	alt_up_audio_write_fifo(audio_dev,&fifo_data_ptr[num_played_samples],min,ALT_UP_AUDIO_LEFT);
	counter++;
	num_played_samples += min;

	// Restarts the loop with the beginning of fifo_data when the scrlbar is at pos 1
	//sample_size is 11000 so 16 * sample_size is 176000.
	// Remember to change the 1000 to sample_size * 16
	if (num_played_samples >= (16 * sample_size))
	{
		num_played_samples = 0;
	}

	if (num_played_samples / (sample_size) == 0 && (!(*x_pos == 1)))
	{
		*x_pos = 1;

	}
	if (num_played_samples / (sample_size) == 1 && (!(*x_pos == 21)))
	{
		*x_pos = 21;

	}
	if (num_played_samples / (sample_size) == 2 && (!(*x_pos == 41)))
	{
		*x_pos = 41;

	}

	if (num_played_samples / (sample_size) == 3 && (!(*x_pos == 61)))
	{
		*x_pos = 61;

	}
	if (num_played_samples / (sample_size) == 4 && (!(*x_pos == 81)))
	{
		*x_pos = 81;

	}
	if (num_played_samples / (sample_size) == 5 && (!(*x_pos == 101)))
	{
		*x_pos = 101;

	}
	if (num_played_samples / (sample_size) == 6 && (!(*x_pos == 121)))
	{
		*x_pos = 121;

	}
	if (num_played_samples / (sample_size) == 7 && (!(*x_pos == 141)))
	{
		*x_pos = 141;

	}
	if (num_played_samples / (sample_size) == 8 && (!(*x_pos == 161)))
	{
		*x_pos = 161;

	}
	if (num_played_samples / (sample_size) == 9 && (!(*x_pos == 181)))
	{
		*x_pos = 181;

	}
	if (num_played_samples / (sample_size) == 10 && (!(*x_pos == 201)))
	{
		*x_pos = 201;

	}
	if (num_played_samples / (sample_size) == 11 && (!(*x_pos == 221)))
	{
		*x_pos = 221;

	}
	if (num_played_samples / (sample_size) == 12 && (!(*x_pos == 241)))
	{
		*x_pos = 241;

	}
	if (num_played_samples / (sample_size) == 13 && (!(*x_pos == 261)))
	{
		*x_pos = 261;

	}
	if (num_played_samples / (sample_size) == 14 && (!(*x_pos == 281)))
	{
		*x_pos = 281;

	}
	if (num_played_samples / (sample_size) == 15 && (!(*x_pos == 301)))
	{
		*x_pos = 301;

	}

}

void handle_audio_interrupts2(void* context,alt_u32 id)
{

	//alt_up_audio_disable_write_interrupt(audio_dev);
	int spaceR;
	int spaceL;
	int min = 0;
	static int num_played_samples = 0;
	spaceR = alt_up_audio_write_fifo_space(audio_dev,ALT_UP_AUDIO_RIGHT);
	spaceL = alt_up_audio_write_fifo_space(audio_dev,ALT_UP_AUDIO_LEFT);

	min = (spaceR < spaceL) ? spaceR : spaceL;

	alt_up_audio_write_fifo(audio_dev,fifo_data_ptr,min,ALT_UP_AUDIO_RIGHT);
	alt_up_audio_write_fifo(audio_dev,fifo_data_ptr,min,ALT_UP_AUDIO_LEFT);

	fifo_data_ptr += min;

	num_played_samples += min;
	if (num_played_samples >= 25000)
	{
		fifo_data_ptr = test3;
		num_played_samples = 0;
	}
}
void andrew(void * context,alt_u32 id)
{
	int buf[128];
	int i;
	for (i = 0; i < 128 ; i++)
	{
		if ((i >> 1) % 2 == 0)
		{
			buf[i] = 0x0fff;
		}
		else
		{
			buf[i] = 0x0;
		}
	}
	alt_up_audio_write_fifo(audio_dev,buf,128,ALT_UP_AUDIO_RIGHT);
	alt_up_audio_write_fifo(audio_dev,buf,128,ALT_UP_AUDIO_LEFT);
}
void init_audio_interrupt()
{

	//Register the interrupt handler.
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
	alt_ic_isr_register(0, AUDIO_IRQ, handle_audio_interrupts,NULL, NULL);
	alt_ic_irq_enable(0, AUDIO_IRQ);
#else

	alt_irq_register(AUDIO_IRQ,NULL,handle_audio_interrupts);
	alt_up_audio_enable_write_interrupt(audio_dev);
#endif
}
int getAddress()
{
	int mem_ptr;
	//get the address of where fifi_data is saved

	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;

	}

}
