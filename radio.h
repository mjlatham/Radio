#ifndef BROADCAST_H
#define BROADCAST_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
	uint32_t magic;
	uint32_t date;
	uint64_t size;
} header;

typedef struct {
	uint8_t video[96];
	uint8_t audio[32];	
} content_block;

// 1.
void display_file(int argc, char** argv);

// 2.
header* read_header(FILE* f);

// 3.
void display_header(header* h_data);

// 4.
content_block* read_data(FILE* f, header* hdata);

// 5.
void display_data(content_block* data, header* h_data, int index);

// 6.
void free_data(content_block* data);

int alt_main(int argc, char** argv);
#endif
