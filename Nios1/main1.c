/*
 * main1.c
 *
 *  Created on: 2014-03-11
 *      Author: Ehsan
 */

//#define KEYS_BASE 0x00803460
#include <stdio.h>
#include <io.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "system.h"
#include "wavdata.h"

alt_up_sd_card_dev *sdcard_dev = NULL;

#define switches (volatile int *) SWITCHES_BASE
#define leds (int *) LEDS_BASE
int main() {
	printf("nios1: started\n");

	//Have a look at this: http://www.codeproject.com/Articles/12449/Allocating-memory-to-a-Pointer-to-Pointer-variable
	selected_files_calloc = (char **) calloc(4, sizeof(char*));
	(selected_files_calloc[0]) = (char *) calloc(16, sizeof(char));
	(selected_files_calloc[1]) = (char *) calloc(16, sizeof(char));
	(selected_files_calloc[2]) = (char *) calloc(16, sizeof(char));
	(selected_files_calloc[3]) = (char *) calloc(16, sizeof(char));

	selected_files2_calloc = (char **) calloc(4, sizeof(char*));
	(selected_files2_calloc[0]) = (char *) calloc(16, sizeof(char));
	(selected_files2_calloc[1]) = (char *) calloc(16, sizeof(char));
	(selected_files2_calloc[2]) = (char *) calloc(16, sizeof(char));
	(selected_files2_calloc[3]) = (char *) calloc(16, sizeof(char));

	LOOP_GRID_calloc = (unsigned int **) calloc(4, sizeof(unsigned int*));
	(LOOP_GRID_calloc[0]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID_calloc[1]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID_calloc[2]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID_calloc[3]) = (unsigned int *) calloc(16, sizeof(unsigned int));

	LOOP_GRID2_calloc = (unsigned int **) calloc(4, sizeof(unsigned int*));
	(LOOP_GRID2_calloc[0]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID2_calloc[1]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID2_calloc[2]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	(LOOP_GRID2_calloc[3]) = (unsigned int *) calloc(16, sizeof(unsigned int));
	// Wait for key 1 to be pressed
	printf("NIOS1: Waiting for KEY 2 to be pressed\n");
	while ((IORD(BUTTONS_BASE, 0) & 0x04) == 0x04)
		;
	printf("NIOS1: KEY 2 was pressed\n");

	//Giving the pointer of LOOP_GRID , fifo_data and filenames
	int error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE,
			FIFO_12_IN_CSR_BASE, (int) fifo_data);
	printf("Address of the fifo_data: %p\n", fifo_data);

	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE, FIFO_12_IN_CSR_BASE,
			(int) LOOP_GRID_calloc);
	printf("Address of the LOOP_GRID: %p\n", LOOP_GRID_calloc);

	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE, FIFO_12_IN_CSR_BASE,
			(int) LOOP_GRID2_calloc);
	printf("Address of the LOOP_GRID: %p\n", LOOP_GRID2_calloc);

	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE, FIFO_12_IN_CSR_BASE,
			(int) file_names);
	printf("Address of the file_names: %p\n", file_names);

	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE, FIFO_12_IN_CSR_BASE,
			(int) selected_files_calloc);
	printf("Address of the selected_files for device 1: %p\n",
			selected_files_calloc);

	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE, FIFO_12_IN_CSR_BASE,
			(int) selected_files2_calloc);
	printf("Address of the selected_files for device 2: %p\n",
			selected_files2_calloc);
	/*	error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE + 4,
	 FIFO_12_IN_CSR_BASE + 4, (int) &numFiles);
	 printf("Address of the numFiles: %d\n", (int) &numFiles);*/

	//Initialising the SD Card IP Core HAL device driver.
	sdcard_dev = alt_up_sd_card_open_dev("/dev/SD_card");

	int sdCard_check = sdcard_setup(sdcard_dev);
	if (sdCard_check == -1)
		printf("Could not read from the SDcard.\n");
	else if (sdCard_check == -2)
		printf("The SDcard is not present!\n");
	else if (sdCard_check == -3)
		printf(
				"The SDcard is not formatted to be FAT16 and could not be read.\n");
	else
		printf("SDcard is opened successfully.\n");

	//reads the file names
	numFiles = doFiles(sdcard_dev);
	printf("Read %d number of files.\n", numFiles);

	//Initialising the LOOP_GRID values. probably useless but just in case
	int row = 0;
	int col = 0;
	for (row = 0; row < NUM_OF_MUSIC; row++) {
		for (col = 0; col < 16; col++) {
			LOOP_GRID[row][col] = 0;
		}
	}
	int print_count = 0;

	for (print_count = 0; print_count < 8; print_count++) {
		printf("Name of the %d file is %16s.\n", print_count,
				file_names[print_count]);
	}

	//Wait for key 1 to be pressed
	printf("NIOS1: Waiting for KEY 1 to be pressed\n");
	while ((IORD(BUTTONS_BASE, 0) & 0x02) == 0x02)
		;
	printf("NIOS1: KEY 1 was pressed\n");

	printf("File Names are copied to appropriate file structs.\n");
	/*char * strncpy ( char * destination, const char * source, size_t num );
	 Copies the first num characters of source to destination. If the end of the source C string
	 */

	printf("The selected files from device 1 are: \n%s \t %s \t %s \t %s \n",
			selected_files_calloc[0], selected_files_calloc[1],
			selected_files_calloc[2], selected_files_calloc[3]);
	strncpy(sample0_wav_file.file_name, selected_files_calloc[0], 9);
	strncpy(sample1_wav_file.file_name, selected_files_calloc[1], 9);
	strncpy(sample2_wav_file.file_name, selected_files_calloc[2], 9);
	strncpy(sample3_wav_file.file_name, selected_files_calloc[3], 9);

	printf("The selected files from device 2 are: \n%s \t %s \t %s \t %s \n",
			selected_files2_calloc[0], selected_files2_calloc[1],
			selected_files2_calloc[2], selected_files2_calloc[3]);
	strncpy(sample0_wav2_file.file_name, selected_files2_calloc[0], 16);
	strncpy(sample1_wav2_file.file_name, selected_files2_calloc[1], 16);
	strncpy(sample2_wav2_file.file_name, selected_files2_calloc[2], 16);
	strncpy(sample3_wav2_file.file_name, selected_files2_calloc[3], 16);

	//Opens up the files and reads the data into data section of each struct.
	readWavData(&sample0_wav_file);
	readWavData(&sample1_wav_file);
	readWavData(&sample2_wav_file);
	readWavData(&sample3_wav_file);
	readWavData(&sample0_wav2_file);
	readWavData(&sample1_wav2_file);
	readWavData(&sample2_wav2_file);
	readWavData(&sample3_wav2_file);
	/////////////////////////////////////////////////////////////////////////


	/*	int test;
	 error = altera_avalon_fifo_write_fifo(FIFO_12_IN_BASE + 2,
	 FIFO_12_IN_CSR_BASE + 2, (int) &test);
	 printf("error: %d\n", error);
	 printf("Address of the fifo_data: %d\n", (int) &test);
	 int asd = 0;
	 while (test != 1370) {
	 asd++;
	 if (asd%10000 == 0) printf ("test=%d\n",test);
	 }*/
	int i;
	while (1) {

		for (i = 0; i < sample_size; i++) {
			fifo_data[i]
					= (((LOOP_GRID_calloc[0][0] * sample0_wav_file.data[i])
							+ (LOOP_GRID_calloc[1][0]
									* sample1_wav_file.data[i])
							+ (LOOP_GRID_calloc[2][0]
									* sample2_wav_file.data[i])
							+ (LOOP_GRID_calloc[3][0]
									* sample3_wav_file.data[i]))
							+ ((LOOP_GRID2_calloc[0][0]
									* sample0_wav2_file.data[i])
									+ (LOOP_GRID2_calloc[1][0]
											* sample1_wav2_file.data[i])
									+ (LOOP_GRID2_calloc[2][0]
											* sample2_wav2_file.data[i])
									+ (LOOP_GRID2_calloc[3][0]
											* sample3_wav2_file.data[i])));
		}

		for (i = 0; i < sample_size; i++) {
			fifo_data[sample_size + i] = (((LOOP_GRID_calloc[0][1]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][1]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][1]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][1]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][1]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][1]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][1]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][1]
					* sample3_wav2_file.data[i])));
		}

		for (i = 0; i < sample_size; i++) {
			fifo_data[2 * sample_size + i] = (((LOOP_GRID_calloc[0][2]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][2]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][2]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][2]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][2]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][2]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][2]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][2]
					* sample3_wav2_file.data[i])));
		}

		for (i = 0; i < sample_size; i++) {
			fifo_data[3 * sample_size + i] = (((LOOP_GRID_calloc[0][3]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][3]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][3]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][3]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][3]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][3]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][3]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][3]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[4 * sample_size + i] = (((LOOP_GRID_calloc[0][4]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][4]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][4]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][4]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][4]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][4]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][4]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][4]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[5 * sample_size + i] = (((LOOP_GRID_calloc[0][5]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][5]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][5]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][5]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][5]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][5]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][5]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][5]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[6 * sample_size + i] = (((LOOP_GRID_calloc[0][6]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][6]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][6]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][6]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][6]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][6]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][6]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][6]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[7 * sample_size + i] = (((LOOP_GRID_calloc[0][7]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][7]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][7]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][7]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][7]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][7]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][7]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][7]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[8 * sample_size + i] = (((LOOP_GRID_calloc[0][8]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][8]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][8]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][8]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][8]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][8]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][8]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][8]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[9 * sample_size + i] = (((LOOP_GRID_calloc[0][9]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][9]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][9]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][9]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][9]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][9]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][9]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][9]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[10 * sample_size + i] = (((LOOP_GRID_calloc[0][10]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][10]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][10]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][10]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][10]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][10]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][10]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][10]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[11 * sample_size + i] = (((LOOP_GRID_calloc[0][11]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][11]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][11]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][11]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][11]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][11]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][11]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][11]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[12 * sample_size + i] = (((LOOP_GRID_calloc[0][12]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][12]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][12]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][12]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][12]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][12]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][12]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][12]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[13 * sample_size + i] = (((LOOP_GRID_calloc[0][13]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][13]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][13]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][13]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][13]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][13]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][13]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][13]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[14 * sample_size + i] = (((LOOP_GRID_calloc[0][14]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][14]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][14]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][14]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][14]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][14]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][14]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][14]
					* sample3_wav2_file.data[i])));
		}
		for (i = 0; i < sample_size; i++) {
			fifo_data[15 * sample_size + i] = (((LOOP_GRID_calloc[0][15]
					* sample0_wav_file.data[i]) + (LOOP_GRID_calloc[1][15]
					* sample1_wav_file.data[i]) + (LOOP_GRID_calloc[2][15]
					* sample2_wav_file.data[i]) + (LOOP_GRID_calloc[3][15]
					* sample3_wav_file.data[i])) + ((LOOP_GRID2_calloc[0][15]
					* sample0_wav2_file.data[i]) + (LOOP_GRID2_calloc[1][15]
					* sample1_wav2_file.data[i]) + (LOOP_GRID2_calloc[2][15]
					* sample2_wav2_file.data[i]) + (LOOP_GRID2_calloc[3][15]
					* sample3_wav2_file.data[i])));
		}

		IOWR (LEDS_BASE,0,(IORD(switches,0)));

	}

	free(*selected_files_calloc);
	free(*(selected_files_calloc + 1));
	free(*(selected_files_calloc + 2));
	free(*(selected_files_calloc + 3));
	free(selected_files_calloc);

	free(*selected_files2_calloc);
	free(*(selected_files2_calloc + 1));
	free(*(selected_files2_calloc + 2));
	free(*(selected_files2_calloc + 3));
	free(selected_files2_calloc);

	free(*LOOP_GRID_calloc);
	free(*(LOOP_GRID_calloc + 1));
	free(*(LOOP_GRID_calloc + 2));
	free(*(LOOP_GRID_calloc + 3));
	free(LOOP_GRID_calloc);

	return 0;
}

