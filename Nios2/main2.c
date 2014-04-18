#include <stdio.h>
#include <assert.h>
#include <system.h>
#include <string.h>
#include <stdlib.h>
#include "altera_up_avalon_rs232.h"
#include "altera_avalon_fifo_util.h"

#include "myaudio.h"

#include "myvga.h"
#define switches (volatile char *) 0x00002000
#define leds ( char *) 0x00002010
#define verbose 1

alt_up_audio_dev *audio_dev;
//alt_up_char_buffer_dev *char_buffer;


int main(void)
{
	printf("NOISII core 2 started\n");
	x_pos = (int *) malloc(sizeof(int));
	*x_pos = 1;
	// Wait for key 2 to be pressed
	printf("NIOS1: Waiting for KEY 2 to be pressed\n");
	while ((IORD(BUTTONS_BASE, 0) & 0x04) == 0x04)
		;
	printf("NIOS1: KEY 2 was pressed\n");
	int mem_ptr;
	//get the address of where fifi_data is saved

	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	fifo_data_ptr = (unsigned int *) mem_ptr;
	printf("The received Address of fifo_data is: %p\n",fifo_data_ptr);

	//get the address of where LOOP_GRID is saved
	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	unsigned int **LOOP_GRID_ptr = (unsigned int **) mem_ptr;
	printf("The received Address of LOOP_GRID_ptr is: %p\n",LOOP_GRID_ptr);

	//get the address of where LOOP_GRID2 is saved
	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	unsigned int **LOOP_GRID2_ptr = (unsigned int **) mem_ptr;
	printf("The received Address of LOOP_GRID2_ptr is: %p\n",LOOP_GRID2_ptr);

	//get the address of where file_name is saved
	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	char ** file_names_ptr = (char **) mem_ptr;
	printf("The received Address of file_name is: %p\n",file_names_ptr);

	//get the address of where selected_files is saved
	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	char ** selected_file_ptr = (char **) mem_ptr;
	printf("The received Address of selected_file_ptr for device 1 is: %p\n",selected_file_ptr);

	//get the address of where selected_files is saved
	while (1)
	{
		mem_ptr = altera_avalon_fifo_read_fifo(FIFO_12_OUT_BASE,FIFO_12_OUT_CSR_BASE);
		if (mem_ptr) break;
	}
	char ** selected_file2_ptr = (char **) mem_ptr;
	printf("The received Address of selected_file2_ptr for device 2 is: %p\n",selected_file2_ptr);


	int numFile = 7;
	printf("All pointers are initialised.\n");
	//printing the names of the files in SD card
	int print_count = 0;

	printf("NIOS1: Waiting for KEY 3 to be pressed\n");
	while ((IORD(BUTTONS_BASE, 0) & 0x08) == 0x08)
		;
	printf("NIOS1: KEY 3 was pressed\n");

	for (print_count = 0; print_count < (4 * 8) ; print_count += 4)
	{
		printf("Address of &file_names_ptr[%d] is %d: Name of the %d file_names_ptr is %s .\n",print_count,&file_names_ptr[print_count],print_count,&file_names_ptr[print_count]);
	}

	//Setting up the Audio_video core
	av_config_setup();

	//Initialising the audio IP Core HAL device driver.
	audio_dev = alt_up_audio_open_dev("/dev/audio");
	if (audio_dev == NULL) printf("Error: could not open audio device \n");
	else printf("Opened audio device \n");
	//alt_up_audio_reset_audio_core(audio_dev); // This is not necessary most of the time


	printf("Audio Interrupts is initialised.\n");
	init_audio_interrupt();

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;

	// Set the first buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr1);

	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	// Wait for the swap to complete
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;

	// Clear both buffers (all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer,0);

	//Draw loop grid once to get positions
	draw_loop_grid(pixel_buffer);

	//This box is used by keyboard.
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x_0,y_0,x_1,y_1,0xFFFF,0);

	// middleman
	printf("Communication\n");

	int i = 0;
	int j = 0;
	int k = 0;
	unsigned char data;
	unsigned char data2;
	unsigned char parity;
	unsigned char parity2;
	unsigned char message1[6];
	unsigned char message2[6];
	unsigned char message3[6];
	unsigned char message4[6];
	unsigned char message5[6];
	unsigned char message6[6];
	unsigned char message7[6];
	unsigned char message8[6];


	strncpy(message1,&file_names_ptr[0],6);
	strncpy(message2,&file_names_ptr[4],6);
	strncpy(message3,&file_names_ptr[8],6);
	strncpy(message4,&file_names_ptr[12],6);
	strncpy(message5,&file_names_ptr[16],6);
	strncpy(message6,&file_names_ptr[20],6);
	strncpy(message7,&file_names_ptr[24],6);
	strncpy(message8,&file_names_ptr[28],6);
	printf ("message1=%s",message1);
	printf ("message2=%s",message2);
	printf ("message3=%s",message3);
	printf ("message4=%s",message4);
	printf ("message5=%s",message5);
	printf ("message6=%s",message6);
	printf ("message7=%s",message7);
	printf ("message8=%s",message8);

	int column = 0;
	int row = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp0 = 0;
	int result = 0;
	char android_msg[12];
	char file_msg[80];
	char file_msg2[80];
	char android_ID[4];
	char file1[13];
	char file2[13];
	char file3[13];
	char file4[13];
	char file5[13];
	char file6[13];
	char file7[13];
	char file8[13];
	int device_count = 0;

	printf("UART Initialization\n");

	alt_up_rs232_dev* uart_2 = alt_up_rs232_open_dev(RS232_0_NAME);
	if (!uart_2)
	{
		printf("Error opening rs232\n");
		return 0;
	}
	printf("Clearing read buffer to start\n");

	while (alt_up_rs232_get_used_space_in_read_FIFO(uart_2))
	{ //returns the number of data words remaining in the read FIFO
		alt_up_rs232_read_data(uart_2,&data,&parity); //read data from rs232 core - store data in data and parity in parity
	}

	// start sending file names to all devices
	printf("Sending the message to the Middleman\n");
	//Start with the number of bytes in our message
	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message1));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message1) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message1[j]) != 0) //writes a character to rs232 core
		{
			printf("Message1 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message2));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message2) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message2[j]) != 0) //writes a character to rs232 core
		{
			printf("Message2 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message3));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message3) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message3[j]) != 0) //writes a character to rs232 core
		{
			printf("Message3 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message4));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message4) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message4[j]) != 0) //writes a character to rs232 core
		{
			printf("Message4 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message5));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message5) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message5[j]) != 0) //writes a character to rs232 core
		{
			printf("Message5 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message6));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message6) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message6[j]) != 0) //writes a character to rs232 core
		{
			printf("Message6 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message7));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message7) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message7[j]) != 0) //writes a character to rs232 core
		{
			printf("Message7 was not sent successfully.\n");
		}
	}

	alt_up_rs232_write_data(uart_2,0xFF); // send the message to all devices
	alt_up_rs232_write_data(uart_2,(unsigned char) strlen(message8));
	//Now send the actual message to the Middleman
	for (j = 0; j < strlen(message8) ; j++)
	{
		if (alt_up_rs232_write_data(uart_2,message8[j]) != 0) //writes a character to rs232 core
		{
			printf("Message8 was not sent successfully.\n");
		}
	}

	//=====================================================================================================================

	// Now receive file names and device ID from the Middleman
	printf("Waiting for file names\n");
	printf("Waiting for data to come back from the Middleman\n"); // now wait for devices to send buttontags
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart_2) == 0)
		//do nothing while waiting for characters to arrive
		;

	// First byte is the number of characters in our message
	alt_up_rs232_read_data(uart_2,&data2,&parity2); //first data to arrive is number of chars
	int num_to_receive_2 = (int) data2;
	printf("About to receive %d characters:\n",num_to_receive_2);

	//for (i = 0; i < num_to_receive; i++) {
	while (k < 65)
	{

		while (alt_up_rs232_get_used_space_in_read_FIFO(uart_2) == 0)
			// do nothing while waiting for rest of data
			;

		alt_up_rs232_read_data(uart_2,&data2,&parity2); //keep reading received data and print
		printf("%c  ",data2);

		if (device_count == 0)
		{
			file_msg[k] = data2;
			printf("file message %d is %d\n",k,file_msg[k]);
		}

		if (device_count == 1)
		{
			file_msg2[k] = data2;
			printf("file message2 %d is %d\n",k,file_msg2[k]);
		}

		if (k == 64) //
		{

			printf("reached 72\n");
			if ((file_msg[2] == 65) && (file_msg[3] == 65) && (device_count == 0))
			{
				printf("First file is: ");
				file1[0] = file_msg[7];
				printf("%c",file1[0]); //first 4 are device ID
				file1[1] = file_msg[8];
				printf("%c",file1[1]);
				file1[2] = file_msg[9];
				printf("%c",file1[2]);
				file1[3] = file_msg[10];
				printf("%c",file1[3]);//name of file begins
				file1[4] = file_msg[11];
				printf("%c",file1[4]);
				file1[5] = file_msg[12];
				printf("%c",file1[5]);
				file1[6] = file_msg[13];
				printf("%c",file1[6]);
				file1[7] = file_msg[14];
				printf("%c",file1[7]);
				file1[8] = file_msg[15];
				printf("%c",file1[8]);
				file1[9] = file_msg[16];
				printf("%c",file1[9]);
				file1[10] = file_msg[17];
				printf("%c",file1[10]);
				file1[11] = file_msg[18];
				printf("%c",file1[11]);
				file1[12] = file_msg[19];
				printf("%c\n",file1[12]);

				printf("Second file is: ");
				file2[0] = file_msg[21];
				printf("%c",file2[0]); //first 4 are device ID
				file2[1] = file_msg[22];
				printf("%c",file2[1]);
				file2[2] = file_msg[23];
				printf("%c",file2[2]);
				file2[3] = file_msg[24];
				printf("%c",file2[3]);//name of file begins
				file2[4] = file_msg[25];
				printf("%c",file2[4]);
				file2[5] = file_msg[26];
				printf("%c",file2[5]);
				file2[6] = file_msg[27];
				printf("%c",file2[6]);
				file2[7] = file_msg[28];
				printf("%c",file2[7]);
				file2[8] = file_msg[29];
				printf("%c",file2[8]);
				file2[9] = file_msg[30];
				printf("%c",file2[9]);
				file2[10] = file_msg[31];
				printf("%c",file2[10]);
				file2[11] = file_msg[32];
				printf("%c",file2[11]);
				file2[12] = file_msg[33];
				printf("%c\n",file2[12]);

				printf("Third file is: ");
				file3[0] = file_msg[35];
				printf("%c",file3[0]); //first 4 are device ID
				file3[1] = file_msg[36];
				printf("%c",file3[1]);
				file3[2] = file_msg[37];
				printf("%c",file3[2]);
				file3[3] = file_msg[38];
				printf("%c",file3[3]);//name of file begins
				file3[4] = file_msg[39];
				printf("%c",file3[4]);
				file3[5] = file_msg[40];
				printf("%c",file3[5]);
				file3[6] = file_msg[41];
				printf("%c",file3[6]);
				file3[7] = file_msg[42];
				printf("%c",file3[7]);
				file3[8] = file_msg[43];
				printf("%c",file3[8]);
				file3[9] = file_msg[44];
				printf("%c",file3[9]);
				file3[10] = file_msg[45];
				printf("%c",file3[10]);
				file3[11] = file_msg[46];
				printf("%c",file3[11]);
				file3[12] = file_msg[47];
				printf("%c\n",file3[12]);

				printf("Fourth file is: ");
				file4[0] = file_msg[49];
				printf("%c",file4[0]); //first 4 are device ID
				file4[1] = file_msg[50];
				printf("%c",file4[1]);
				file4[2] = file_msg[51];
				printf("%c",file4[2]);
				file4[3] = file_msg[52];
				printf("%c",file4[3]);//name of file begins
				file4[4] = file_msg[53];
				printf("%c",file4[4]);
				file4[5] = file_msg[54];
				printf("%c",file4[5]);
				file4[6] = file_msg[55];
				printf("%c",file4[6]);
				file4[7] = file_msg[56];
				printf("%c",file4[7]);
				file4[8] = file_msg[57];
				printf("%c",file4[8]);
				file4[9] = file_msg[58];
				printf("%c",file4[9]);
				file4[10] = file_msg[59];
				printf("%c",file4[10]);
				file4[11] = file_msg[60];
				printf("%c",file4[11]);
				file4[12] = file_msg[61];
				printf("%c\n",file4[12]);

				if ((file_msg[63] == 66) && (file_msg[64] == 66))
				{
					device_count = 1;
					k = -2; //reset loop
					printf("Received all file names from first device\n");
				}

			}

			if ((file_msg2[2] == 65) && (file_msg2[3] == 65) && (device_count == 1))
			{
				//printf("Reading from device 2 and k is %d\n", k);

				printf("Fifth file is: ");
				file5[0] = file_msg2[7];
				printf("%c",file5[0]); //first 4 are device ID
				file5[1] = file_msg2[8];
				printf("%c",file5[1]);
				file5[2] = file_msg2[9];
				printf("%c",file5[2]);
				file5[3] = file_msg2[10];
				printf("%c",file5[3]);//name of file begins
				file5[4] = file_msg2[11];
				printf("%c",file5[4]);
				file5[5] = file_msg2[12];
				printf("%c",file5[5]);
				file5[6] = file_msg2[13];
				printf("%c",file5[6]);
				file5[7] = file_msg2[14];
				printf("%c",file5[7]);
				file5[8] = file_msg2[15];
				printf("%c",file5[8]);
				file5[9] = file_msg2[16];
				printf("%c",file5[9]);
				file5[10] = file_msg2[17];
				printf("%c",file5[10]);
				file5[11] = file_msg2[18];
				printf("%c",file5[11]);
				file5[12] = file_msg2[19];
				printf("%c\n",file5[12]);

				printf("Sixth file is: ");
				file6[0] = file_msg2[21];
				printf("%c",file6[0]); //first 4 are device ID
				file6[1] = file_msg2[22];
				printf("%c",file6[1]);
				file6[2] = file_msg2[23];
				printf("%c",file6[2]);
				file6[3] = file_msg2[24];
				printf("%c",file6[3]);//name of file begins
				file6[4] = file_msg2[25];
				printf("%c",file6[4]);
				file6[5] = file_msg2[26];
				printf("%c",file6[5]);
				file6[6] = file_msg2[27];
				printf("%c",file6[6]);
				file6[7] = file_msg2[28];
				printf("%c",file6[7]);
				file6[8] = file_msg2[29];
				printf("%c",file6[8]);
				file6[9] = file_msg2[30];
				printf("%c",file6[9]);
				file6[10] = file_msg2[31];
				printf("%c",file6[10]);
				file6[11] = file_msg2[32];
				printf("%c",file6[11]);
				file6[12] = file_msg2[33];
				printf("%c\n",file6[12]);

				printf("Seventh file is: ");
				file7[0] = file_msg2[35];
				printf("%c",file7[0]); //first 4 are device ID
				file7[1] = file_msg2[36];
				printf("%c",file7[1]);
				file7[2] = file_msg2[37];
				printf("%c",file7[2]);
				file7[3] = file_msg2[38];
				printf("%c",file7[3]);//name of file begins
				file7[4] = file_msg2[39];
				printf("%c",file7[4]);
				file7[5] = file_msg2[40];
				printf("%c",file7[5]);
				file7[6] = file_msg2[41];
				printf("%c",file7[6]);
				file7[7] = file_msg2[42];
				printf("%c",file7[7]);
				file7[8] = file_msg2[43];
				printf("%c",file7[8]);
				file7[9] = file_msg2[44];
				printf("%c",file7[9]);
				file7[10] = file_msg2[45];
				printf("%c",file7[10]);
				file7[11] = file_msg2[46];
				printf("%c",file7[11]);
				file7[12] = file_msg2[47];
				printf("%c\n",file7[12]);

				printf("Eighth file is: ");
				file8[0] = file_msg2[49];
				printf("%c",file8[0]); //first 4 are device ID
				file8[1] = file_msg2[50];
				printf("%c",file8[1]);
				file8[2] = file_msg2[51];
				printf("%c",file8[2]);
				file8[3] = file_msg2[52];
				printf("%c",file8[3]);//name of file begins
				file8[4] = file_msg2[53];
				printf("%c",file8[4]);
				file8[5] = file_msg2[54];
				printf("%c",file8[5]);
				file8[6] = file_msg2[55];
				printf("%c",file8[6]);
				file8[7] = file_msg2[56];
				printf("%c",file8[7]);
				file8[8] = file_msg2[57];
				printf("%c",file8[8]);
				file8[9] = file_msg2[58];
				printf("%c",file8[9]);
				file8[10] = file_msg2[59];
				printf("%c",file8[10]);
				file8[11] = file_msg2[60];
				printf("%c",file8[11]);
				file8[12] = file_msg2[61];
				printf("%c\n",file8[12]);

				if ((file_msg[63] == 66) && (file_msg[64] == 66))
				{
					printf("Received all file names from second device\n");
				}
			}

			//i = -2; //reset loop

		}

		k++;

	}

	char deviceId[4];
	strncpy(&deviceId[0],&file1[0],4);
	printf("File names dedicated to DevideId = %c%c%c%c.\n",deviceId[0],deviceId[1],deviceId[2],deviceId[3]);
	printf("&file1[4]= %s\n",&file1[4]);
	printf("&file2[4]= %s\n",&file2[4]);
	printf("&file3[4]= %s\n",&file3[4]);
	printf("&file4[4]= %s\n",&file4[4]);
	strncpy(selected_file_ptr[0],&file1[4],9);
	strncpy(selected_file_ptr[1],&file2[4],9);
	strncpy(selected_file_ptr[2],&file3[4],9);
	strncpy(selected_file_ptr[3],&file4[4],9);
	printf("selected_file_ptr[0] = %s\n",selected_file_ptr[0]);
	printf("selected_file_ptr[1] = %s\n",selected_file_ptr[1]);
	printf("selected_file_ptr[2] = %s\n",selected_file_ptr[2]);
	printf("selected_file_ptr[3] = %s\n",selected_file_ptr[3]);

	char device2Id[4];
	strncpy(&device2Id[0],&file5[0],4);
	printf("File names dedicated to Devide2Id = %c%c%c%c.\n",device2Id[0],device2Id[1],device2Id[2],device2Id[3]);
	printf("&file5[4]= %s\n",&file5[4]);
	printf("&file6[4]= %s\n",&file6[4]);
	printf("&file7[4]= %s\n",&file7[4]);
	printf("&file8[4]= %s\n",&file8[4]);
	strncpy(selected_file2_ptr[0],&file5[4],9);
	strncpy(selected_file2_ptr[1],&file6[4],9);
	strncpy(selected_file2_ptr[2],&file7[4],9);
	strncpy(selected_file2_ptr[3],&file8[4],9);
	printf("selected_file2_ptr[0] = %s\n",selected_file2_ptr[0]);
	printf("selected_file2_ptr[1] = %s\n",selected_file2_ptr[1]);
	printf("selected_file2_ptr[2] = %s\n",selected_file2_ptr[2]);
	printf("selected_file2_ptr[3] = %s\n",selected_file2_ptr[3]);
	//=====================================================================================================================

	// Now receive the message from the Middleman
	printf("Waiting for data to come back from the Middleman\n"); // now wait for devices to send buttontags
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart_2) == 0)
		//do nothing while waiting for characters to arrive
		;

	// First byte is the number of characters in our message
	alt_up_rs232_read_data(uart_2,&data,&parity); //first data to arrive is number of chars
	int num_to_receive = (int) data;
	printf("About to receive %d characters:\n",num_to_receive);

	//for (i = 0; i < num_to_receive; i++) {
	while (i < 12)
	{

		while (alt_up_rs232_get_used_space_in_read_FIFO(uart_2) == 0)
		{
			draw_scroll_bar(x_pos); //VGA
		}

		alt_up_rs232_read_data(uart_2,&data,&parity); //keep reading received data and print
		printf("%c  ",data);

		android_msg[i] = data;
		printf("android message %d is %d\n",i,android_msg[i]);

		if (i == 10) //
		{

			if ((android_msg[2] == 49) && (android_msg[3] == 49))
			{
				printf("Android Device ID:");
				android_ID[0] = android_msg[4];
				printf("%c",android_ID[0]);
				android_ID[1] = android_msg[5];
				printf("%c",android_ID[1]);
				android_ID[2] = android_msg[6];
				printf("%c",android_ID[2]);
				android_ID[3] = android_msg[7];
				printf("%c \n",android_ID[3]);

				row = android_msg[8] - 48;
				temp1 = android_msg[9] - 48;
				temp2 = android_msg[10] - 48;
				column = (temp1 * 10) + temp2;
				printf("row is %d \n",row);
				printf("column is %d \n",column);
				//printf("device ID is %d\n", android_msg[-1]); //should we just go with serial number?

				if ((android_ID[0] == deviceId[0]) && (android_ID[1] == deviceId[1]) && (android_ID[2] == deviceId[2])
				        && (android_ID[3] == deviceId[3]))
				{
					if (LOOP_GRID_ptr[row][column] == 0)
					{
						LOOP_GRID_ptr[row][column] = 1;
						fill_top_loop_grid_green(row,column);
					}
					else
					{
						LOOP_GRID_ptr[row][column] = 0;
						fill_top_loop_grid_black(row,column);
					}
				}
				else
				{
					if (LOOP_GRID2_ptr[row][column] == 0)
					{
						LOOP_GRID2_ptr[row][column] = 1;
						fill_bottom_loop_grid_orange(row,column);
					}
					else
					{
						LOOP_GRID2_ptr[row][column] = 0;
						fill_bottom_loop_grid_black(row,column);
					}
				}

			}

			i = -2; //reset loop
		}

		i++;
	}

	free(x_pos);
	return 0;
}

