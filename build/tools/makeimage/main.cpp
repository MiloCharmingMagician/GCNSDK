#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#define ENTRY_ADDRESS 1476534401
#define SIZE 2551840768
#define REBOOT_SIZE 2160263424

struct Header
{
	char date[16];
    unsigned long entryAddress;
	unsigned long size;
	unsigned long rebootSize;
};

static Header ldr;
static Header * wp = &ldr;

char* readFileData(const char* filename);
unsigned long getFileSize(const char* filename);

int main(int argc, char** argv) {
	std::fstream file;
	char* ldrFilename;
    char* output;

	std::cout << "Makeimage Version 1.0.0" << "\n";
    std::cout << "Copyright (C) Cyndanera Llc. 2023-2024. All rights reserved" << "\n\n";

#ifndef _DEBUG
	if (argc < 2) {
		// print usage here and return, since that's what you need.
		std::cout << "makeimage [LDRFILE] [OUTPUT]" << std::endl;
	}
	else {
		ldrFilename = argv[1];
        output = argv[2];
	}
#else
    ldrFilename = "apploader.bin";
    output = "apploader.img";
#endif

    unsigned long ldrSize = getFileSize(ldrFilename);
	char* ldrFile = readFileData(ldrFilename);

	time_t t;
    tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

	// get date
    int t_year = tm_info->tm_year + 1900;
	int t_month = tm_info->tm_mon - 1;
	int t_day = tm_info->tm_mday;

	char c_year[5] = {0x0, 0x0, 0x0, 0x0, 0x0};
	char c_month[3] = {0x0, 0x0, 0x0};
	char c_day[3] = {0x0, 0x0, 0x0};

	// init apploader header
    for (int i = 0; i < 16; i++)
	{
		wp->date[i] = 0;
	}

	sprintf(c_year, "%04d", t_year);
	sprintf(c_month, "%02d", t_month);
	sprintf(c_day, "%02d", t_day);

	wp->date[0] = c_year[0];
	wp->date[1] = c_year[1];
	wp->date[2] = c_year[2];
	wp->date[3] = c_year[3];
	wp->date[4] = '/';
	wp->date[5] = c_month[0];
	wp->date[6] = c_month[1];
	wp->date[7] = '/';
	wp->date[8] = c_day[0];
	wp->date[9] = c_day[1];

	wp->entryAddress = ENTRY_ADDRESS;
	wp->size = SIZE;
	wp->rebootSize = REBOOT_SIZE;

	// write file
	file.open(output, std::ios::out | std::ios::binary);
	if (file.is_open()) {
        // Header
		file.write(wp->date, 16);
		file.write(reinterpret_cast<const char*>(&wp->entryAddress), sizeof(wp->entryAddress));
		file.write(reinterpret_cast<const char*>(&wp->size), sizeof(wp->size));
		file.write(reinterpret_cast<const char*>(&wp->rebootSize), sizeof(wp->rebootSize));
		// Code
        file.write(ldrFile, ldrSize);
		file.close();
	}

#ifdef _DEBUG
    system("pause");
#endif

    return 0;
}

char* readFileData(const char* filename)
{
	std::fstream file;
    char* data;
    unsigned long size = getFileSize(filename);
    
    file.open(filename, std::ios::in | std::ios::binary);
    if (file.is_open()) {
		data = new char[size];
		file.read(data, size);
		file.close();
		return data;
	}
	else 
	{
	    return 0;
	}
}

unsigned long getFileSize(const char* filename) {
	unsigned long size = 0;
	std::fstream file;

	file.open(filename, std::ios::in | std::ios::binary);

	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		size = file.tellg();
	}
	else {
		std::cout << "Error - " << filename << "\n";
	}

	return size;
}
