/*
 * wavdata.h
 *
 *  Created on: 2014-03-17
 *      Author: Ehsan
 */

#ifndef WAVDATA_H_
#define WAVDATA_H_

#include <stdio.h>
#include <io.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_character_lcd.h"
#include "altera_avalon_fifo_util.h"
#define sample_size 11000
#define SDverbose 1
#define NUM_OF_MUSIC 8
struct file_attr {
		int data[sample_size];
		int array_size;
		float ratio;
		int filehandle;
		char file_name[16];
		int size_of_file;
		int size_of_data;
		int samples_per_second;
		int max_value;
		int id;
		int index_of_max_value;
};

unsigned int LOOP_GRID[NUM_OF_MUSIC][16];
unsigned int ** LOOP_GRID_calloc ;
unsigned int ** LOOP_GRID2_calloc ;
int fifo_data[16 * sample_size];
char file_names[8][16];
//char * file_names[16];
//char  file_names[16*16];
//char ** file_names_calloc;
//char selected_files[4][12];
char ** selected_files_calloc;

char ** selected_files2_calloc;
int numFiles;
struct file_attr sample0_wav_file; // this holds the attributes of the background file
struct file_attr sample1_wav_file; // this holds the attributes of the sample file
struct file_attr sample2_wav_file; // this holds the attributes of the sample file
struct file_attr sample3_wav_file; // this holds the attributes of the sample file

struct file_attr sample0_wav2_file; // this holds the attributes of the background file
struct file_attr sample1_wav2_file; // this holds the attributes of the sample file
struct file_attr sample2_wav2_file; // this holds the attributes of the sample file
struct file_attr sample3_wav2_file; // this holds the attributes of the sample file

int open_wavfile(struct file_attr * sample_attr);
int readWavData(struct file_attr * sample_attr);
int lcd_setup(alt_up_character_lcd_dev * char_lcd_dev);
int sdcard_setup(alt_up_sd_card_dev *sdcard_dev);
int open_wavfile(struct file_attr * sample_attr);
int getNumOfFiles();
int saveFileNames(int num_of_files,char ** file_names);
int printNames(int num_of_files,char ** file_names);
int doFiles(alt_up_sd_card_dev *sdcard_dev);
#endif
