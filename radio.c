#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "radio.h"

// Question to tutor: is it good practice (or just okay) to define the following,
// even if the specs don't ask for it? For example if we wanted to change the error numbers later?
#define MAGIC_NUMBER (872)
#define NO_FILE_SPECIFIED 237
#define FILE_DOES_NOT_EXIST 564
#define INVALID_FILE_POINTER 543
#define MAGIC_ERROR 444
#define NO_HEADER_DATA 42

// Another question: will there be any marks awarded to programming style, e.g. commenting?

///////////////////////////
///////////////////////////

// 1.
void display_file(int argc, char** argv) {
	FILE *file;
	
	if (argc < 2 || argv[1] == NULL) { // Check if there isn't an argument for the filepath
		printf("No file specified: %d\n", NO_FILE_SPECIFIED); // Print if no argument
		return;
	}
	
	file = fopen(argv[1], "r"); // Open file
	
	if (file) { // If the file exists
		printf("FILE PATH: %s\n", argv[1]); // Print out the file path
		fclose(file); // Close the file before returning
		
		return;
	} else { // Otherwise, if file does not exist
		printf("File does not exist: %d\n", FILE_DOES_NOT_EXIST); // Print that the file doesn't exist
		//fclose(file); // fclose breaks this, not sure why
		
		return;
	}
	
	return;
}

// 2.
header* read_header(FILE* f) {
	if (f == NULL) { // Check if file pointer is null
		printf("Invalid File Pointer: %d\n", INVALID_FILE_POINTER); // If so, print invalid file pointer error
		return NULL;
	}
	
	unsigned int * magic = (unsigned int *)malloc(sizeof(unsigned int)); // Allocate space for magic number
	fread(magic, sizeof(unsigned int), 1, f);
	
	if (*magic != MAGIC_NUMBER) { // If file does not have correct magic number
		printf("Incorrect Magic: %d\n", MAGIC_ERROR); // Print error message
		free(magic); // Free memory allocated to magic
		magic = NULL; // Set magic to null
	
		return NULL;
	}
	
	char * date = (char *)malloc(sizeof(char) * 4);
	unsigned long * size = (unsigned long *)malloc(sizeof(unsigned long));
	fread(date, sizeof(char), 4, f);
	fread(size, sizeof(unsigned long), 1, f);
	
	//test
	//printf("%s\n", date);
	//printf("%lu\n", *size);
	
	header * h = (header *)malloc(sizeof(header));
	//h->magic = *magic;
	h->date = (unsigned int) *date;
	h->size = *size;
	
	free(magic); // Free memory allocated to magic
	free(date); // Free memory allocated to date
	free(size); // Free memory allocated to size
	date = NULL; // Set date to null
	size = NULL; // Set size to null
	magic = NULL; // Set magic to null
	
	return h;
}

// 3.
void display_header(header* h_data) {
	if (h_data == NULL) { // If h_data null, display error message and return
		printf("No header data: %d\n", NO_HEADER_DATA);
		return;
	}
	
	printf("DATE: %u\n", h_data->date); // Display date data
	printf("DATA SIZE: %lu\n", h_data->size); // Display size data
	
	return;
}

// 4.
content_block* read_data(FILE* f, header* h_data) {
	if (f == NULL) {
		printf("Invalid File Pointer: 777\n");
		return NULL;
	}
	
	content_block * data = (content_block *)malloc(128); // Allocate space for magic number
	fread(data, 128, 1, f);
	
	return data;
}

// 5.
void display_data(content_block* data, header* h_data, int index) {
	// Show error message if:
	// 	data is null
	// 	index will be out of bounds
	//  h_data is null
	if (data == NULL || index > 92 || h_data == NULL) {
		printf("Cannot display data: 522\n");
		return;
	}
	
	printf("SEGMENT: %d\n", index);
	printf("VIDEO: %u %u %u %u\n", data->video[index], data->video[index + 1], data->video[index + 2], data->video[index + 3]);
	printf("AUDIO: %u %u %u %u\n", data->audio[index], data->audio[index + 1], data->audio[index + 2], data->audio[index + 3]);
	
	return;
}

// 6.
void free_data(content_block* data) {
	free(data);
	data = NULL;
	return;
}

//////////////////////////////
//////////////////////////////

int alt_main(int argc, char** argv) {

	if(argc > 1) {
		char* command = argv[1];
		if(!strcmp(command, "DISPLAY_FILE")) {
			char* new_argv[2];
			new_argv[0] = argv[0];
			new_argv[1] = argv[2];
			display_file(argc-1, new_argv);
		} else if(!strcmp(command, "READ_HEADER")) {
			FILE* f = fopen(argv[2], "rb");
			header* hdata = read_header(f);
			free(hdata);
			fclose(f);
		} else if(!strcmp(command, "DISPLAY_HEADER")) {
			FILE* f = fopen(argv[2], "rb");
			header* hdata = read_header(f);
			display_header(hdata);
			free(hdata);
			fclose(f);
		} else if(!strcmp(command, "READ_DATA")) {
			FILE* f = fopen(argv[2], "rb");
			header* hdata = read_header(f);
			display_header(hdata);
			free(hdata);
			fclose(f);
		} else if(!strcmp(command, "DISPLAY_DATA")) {
			header hdata = { .magic = 998, .date=124543213, .size = 100 };
			content_block blocks[100];
			for(int i = 0; i < hdata.size; i++) {
				blocks[i].video[0] = i;
				blocks[i].audio[0] = i+3;
				blocks[i].video[1] = i;
				blocks[i].audio[1] = i+8;
				blocks[i].video[2] = i;
				blocks[i].audio[2] = i+2;
				blocks[i].video[3] = i;
				blocks[i].audio[3] = i+1;
			}
			display_data(blocks, &hdata, 0);
		} else if(!strcmp(command, "FREE_DATA")) {
			content_block* d = malloc(sizeof(content_block));
			free_data(d);
		}
	}

	return 0;
}
