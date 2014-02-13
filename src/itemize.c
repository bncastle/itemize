//
//Purpose:
//This program takes given input files and puts them together in a single "Itemized" file
//that is searchable based on the input filename. 
//The file can be used in two ways:
//1) It can then be "COFF'd" using objector, and linked into a C program. 
//2) It can be opened as a binary file in a C program
//The C program can then find the data for a given file by looking through the 
//items in the data blob
//
// Copyright 2014 Pixelbytestudios LLC
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "itemize_lib.h"

#define BUFFER_SIZE 2048

#define NUM_SWITCHES  		2
#define SW_USE_EXT    		0
#define SW_FORCE_OVERWRITE 	1

long file_size(FILE *fp)
{
	long current = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp,current, SEEK_SET);
	return size;
}

int file_exists(const char *fname)
{
			
	FILE *f = fopen(fname,"rb");
	if(f != NULL)
	{
		fclose(f);
		return 1;
	}
	return 0;
}

//Checks if the given char pointer contains a switch
//
//returns: switch char, 0 if it was an empty switch, -1 if no switch found
//
char is_switch(char *txt)
{
	int i = 0;

	//ignore any spaces
	while(isspace(txt[i]) && txt[i] != '\0') i++;

	if(txt[i] != '\0' && txt[i] == '-')
	{
		//is it an empty switch ?
		if(txt[i + 1] == '\0') return 0;
		else return txt[i + 1];
	}

	return -1;
}

int main(int argc, char **argv)
{
	unsigned char buffer[BUFFER_SIZE];
	int i,c, size, bytes_read;
	int file_arg_index = 1;
	ItemizerHeader hdr;
	FILE *fin, *fout;
	char *name;
	char sw, outfilename[80];
	unsigned char switches[NUM_SWITCHES];

	memset(switches, 0, NUM_SWITCHES);
	memset(&hdr, 0, sizeof(ItemizerHeader));

	if(argc > 2)
	{
		while((sw = is_switch(argv[file_arg_index])) > -1) 
		{
			//Check switches
			switch(sw)
			{
				case 'e':
					switches[SW_USE_EXT] = 1;
				break;
				case 'f':
					switches[SW_FORCE_OVERWRITE] = 1;
				break;
				default:
					printf("ERROR: Unknown switch: -%c\n", sw);
					return EXIT_FAILURE;
				break;
			}
			file_arg_index++;
		}
	}

	//we need a possible output filename and an input file for a valid cmd line
	if(argc - file_arg_index < 2)
	{
		fprintf(stdout, "\n== Itemize (Copyright 2014 Pixelbyte Studios) ==");
		fprintf(stdout, "\nusage: itemize.exe [options] output-file-without-ext [input files]\n");
		fprintf(stdout, "\noptions:\n");
		fprintf(stdout, "-e : Include the file extension when naming items.\n");
		fprintf(stdout, "     Ex: test.bin would be test_bin in the itemized file.\n");
		fprintf(stdout, "-f : Force overwrite of the output file.\n");
		fprintf(stdout, "     By default if the output file exists, execution will stop.\n");
		fprintf(stdout,"===================================================\n");
		return EXIT_SUCCESS;
	}

	//Check and make sure the input files exist
	for (i = file_arg_index + 1; i < argc; ++i)
	{
		fin = fopen(argv[i], "rb");
		if(fin == NULL)
		{
			fprintf(stderr, "ERROR: Can't open %s ", argv[i]);
			return EXIT_FAILURE;
		}
	}

	//Grab the output filename and add our extension
	strcpy(outfilename, argv[file_arg_index]);
	//c = 0;
	//while(outfilename[c] != '.' && outfilename[c] != '\0') c++;
	//outfilename[c] = '\0';
	strcat(outfilename, ITEMIZED_EXT);

	//If the force overwrite (-f) switch is not specified, check and
	//see if the output file already exists
	if(switches[SW_FORCE_OVERWRITE] == 0)
	{
		if(file_exists(outfilename))
		{
			fprintf(stderr, "ERROR: Output file %s exists!\n", outfilename );
			fprintf(stderr, "Use -f to force overwrite of the output file.");
			return EXIT_FAILURE;
		}
	}

	//wherever file_arg_index points is our output filename
	fout = fopen(outfilename, "wb");

	//Write the main header
	hdr = CURRENT_HDR;

	//Update the # of entries in the itemizer blob
	hdr.num_entries = argc - (file_arg_index + 1);
	fwrite(&hdr, 1, sizeof(ItemizerHeader), fout);

	//Now grab the file data and slap it into out output file
	printf("Adding %d items:\n", hdr.num_entries);
	for (i = file_arg_index + 1; i < argc; ++i)
	{
		fin = fopen(argv[i], "rb");
		if(fin == NULL)
		{
			fprintf(stderr, "ERROR: Unable to open %s ", argv[i]);
			fclose(fout);
			return EXIT_FAILURE;
		}

		//modify the filename to make our item name
		c = 0;
		while(argv[i][c] != '.' && argv[i][c] != '\0') c++;
		if(switches[SW_USE_EXT] && argv[i][c] == '.') argv[i][c] = '_';
		else argv[i][c] = '\0';

		name = argv[i];

		size = file_size(fin);
		printf("%s [%d]\n", name, size);

		//Write it 
		fwrite(&size, sizeof(int), 1, fout);
		fwrite(name, 1, strlen(name), fout);
		fputc('\0',fout); // Add NULL termination
		while((bytes_read = fread(buffer, 1, BUFFER_SIZE, fin)) > 0)
			fwrite(buffer, 1, bytes_read, fout);

		fclose(fin);
	}

	fclose(fout);

	return EXIT_SUCCESS;
}