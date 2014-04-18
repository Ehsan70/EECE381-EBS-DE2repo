#include "wavdata.h"
#define SDCARDverbose 1

int readWavData(struct file_attr * sample_attr)
{
	(*sample_attr).filehandle = alt_up_sd_card_fopen((*sample_attr).file_name,false);
	if ((*sample_attr).filehandle == -1)
	{
		printf("The file could not be opened.\n");
		return -1;
	}
	else if ((*sample_attr).filehandle == -2)
	{
		printf("The file is already open.\n");
		return -2;
	}
	else
	{
		printf("File is opened successfully. waveFile=%d.\n",(*sample_attr).filehandle);
	}
	int n = 0;
	int total_num_samples = 0;
	short sample1;
	short sample2;
	short sample;
	(*sample_attr).max_value = 0;
	while (total_num_samples <= sample_size)
	{
		sample1 = alt_up_sd_card_read((*sample_attr).filehandle);
		sample2 = alt_up_sd_card_read((*sample_attr).filehandle);

		//		if ((sample1 == -1  || sample2 == -1 ) && n>23) {
		//			printf("sample1 = %d\n", sample1);
		//			printf("sample2 = %d\n", sample2);
		//			break;
		//		}
		sample = (short) ((unsigned char) (sample2 & 0x00FF) << 8) | (unsigned char) (sample & 0xFF);
		(*sample_attr).data[n] = (int) sample;

		//the first 44 bytes are header info.
		if ((*sample_attr).data[n] > (*sample_attr).max_value && n > 23)
		{
			(*sample_attr).index_of_max_value = n;
			(*sample_attr).max_value = (*sample_attr).data[n];
		}
		n++;
		total_num_samples++;

		if (total_num_samples % 1000 == 0 && SDCARDverbose)
		{
			printf("%s samples:%d\n",(*sample_attr).file_name,total_num_samples);
		}
		//		if (total_num_samples >= sample_size)
		//			break;

	}

	// Now we how much data is in array
	(*sample_attr).array_size = n;
	//Close file 1 that audio was taken from

	if (alt_up_sd_card_fclose((*sample_attr).filehandle)) printf("The file %s closed successfully.\n",(*sample_attr).file_name);
	else printf("File is was not closed successfully.\n");

	printf("Adjusting the sample data of %s.\n",(*sample_attr).file_name);
	printf("Maximum value of original data for sample0 is %d at %d\n",(*sample_attr).max_value,(*sample_attr).index_of_max_value);
	printf("Number of data for sample %s: %d\n",(*sample_attr).file_name,(*sample_attr).array_size);

	//This figures out the ration each data has to be multiplies to
	if ((*sample_attr).max_value > (32767 / NUM_OF_MUSIC)) (*sample_attr).ratio = (float) (32767 / NUM_OF_MUSIC)
	        / (float) (*sample_attr).max_value;
	else (*sample_attr).ratio = (float) (*sample_attr).max_value / (float) (32767 / NUM_OF_MUSIC);

	printf("The ration for sample %s is : %f\n",(*sample_attr).file_name,(*sample_attr).ratio);
	for (n = 0; n < (*sample_attr).array_size ; n++)
	{
		(*sample_attr).data[n] = (float) (*sample_attr).data[n] * (*sample_attr).ratio;
		if (n % 1000 == 0 && SDverbose) printf("(*sample_attr).data[%d]=%d\n",n,(*sample_attr).data[n]);
	}
	printf("New max value sample %s is %d.\n\n",(*sample_attr).file_name,(*sample_attr).data[(*sample_attr).index_of_max_value]);
	return 1;
}

int lcd_setup(alt_up_character_lcd_dev * char_lcd_dev)
{
	char_lcd_dev = alt_up_character_lcd_open_dev("/dev/character_lcd");
	if (char_lcd_dev == NULL)
	{
		printf("Error: could not open character LCD device\n");
		return -1;
	}
	else
	{
		printf("Opened character LCD device\n");
	}
	/* Initialize the character display */
	alt_up_character_lcd_init(char_lcd_dev);
	return 1;
}

int sdcard_setup(alt_up_sd_card_dev *sdcard_dev)
{
	//Initialising the SD Card IP Core HAL device driver.
	if (sdcard_dev == NULL)
	{
		printf("Could not read from the SDcard.\n");
		return -1;
	}
	else
	{
		if (!alt_up_sd_card_is_Present())
		{
			printf("The SDcard is not present!\n");
			return -2;
		}

		else
		{
			if (!alt_up_sd_card_is_FAT16())
			{
				printf("The SDcard is not formatted to be FAT16 and could not be read.\n");
				return -3;
			}
		}

	}
	return 1;
}

int open_wavfile(struct file_attr * sample_attr)
{

	(*sample_attr).filehandle = alt_up_sd_card_fopen((*sample_attr).file_name,false);
	if ((*sample_attr).filehandle == -1)
	{
		printf("The file could not be opened.\n");
		return -1;
	}
	else if ((*sample_attr).filehandle == -2)
	{
		printf("The file is already open.\n");
		return -2;
	}
	else
	{
		printf("File is opened successfully. waveFile=%d.\n",(*sample_attr).filehandle);
		return 1;
	}
	return 0;
}
int getNumOfFiles()
{
	//getting the names of the files from sd card.
	int num_of_files = 0;
	char buffer_name[10];
	int handler = 0;
	handler = alt_up_sd_card_find_first(" ",buffer_name);
	while ((handler = alt_up_sd_card_find_next(buffer_name)) == !(-1))
	{
		num_of_files++;
	}
	return num_of_files;
}

int saveFileNames(int num_of_files,char ** file_names)
{
	int file_counterr = 0;
	int handler;
	handler = alt_up_sd_card_find_first(" ",file_names[file_counterr]);
	file_counterr++;
	while ((handler = alt_up_sd_card_find_next(file_names[file_counterr])) == !(-1))
	{
		file_counterr++;
	}
	return file_counterr;
}
int printNames(int num_of_files,char ** file_names)
{
	int print_count = 0;
	for (print_count = 0; print_count < num_of_files ; print_count++)
	{
		printf("Name of the %d file is %s.\n",print_count,(file_names[print_count]));
	}
	return print_count;
}
int doFiles(alt_up_sd_card_dev *sdcard_dev)
{
	int num_of_files = 0;
	int handler = 0;
	int count_char = 0;
	handler = alt_up_sd_card_find_first(" ",&(file_names[num_of_files][0]));
	num_of_files++;
	count_char += 12;
	while ((handler = alt_up_sd_card_find_next(&(file_names[num_of_files][0])) == !(-1)))
	{
		num_of_files++;
		count_char += 12;
	}
	printf("There are %d number of files in the SD card.\nCount_char = %d .\n",num_of_files,count_char);
	count_char=0;
	int print_count = 0;
	for (print_count = 0; print_count < num_of_files ; print_count++)
	{
		printf("Name of the %d file is %s.\n",print_count,&(file_names[print_count][0]));
		count_char += 12;
	}
	return num_of_files - 1;
}
