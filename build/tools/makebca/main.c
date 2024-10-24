/*
*
* Written by Garrett.S
* Date: 2024/17/09
* (C) Cyndanera 2023-2024 Llc.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>

#define AUTHCODE_SECTOR_START 4096 // end offset of pfi + dmi sectors
#define AUTHCODE_SECTOR_END 0x0 // size has not been set yet of bitstream of responce data

struct PsnRegion
{
    unsigned long start;
    unsigned long end;
};

struct DiscBCA
{
	unsigned char optInfo[52];
    unsigned char manufacturer[2];
    unsigned char recorderDevice[2];
    unsigned char APMRecorderDevice;
    unsigned char discDate[2];
    unsigned char discTime[2];
    unsigned char discNumber[3];
    unsigned char key[8];
    unsigned char id[4];
	PsnRegion psn[6];
	unsigned char pad[64];
};

static DiscBCA bca;
static DiscBCA * wp = &bca;

void initBca();
void setAuthcode();
void encryptAuthcode();
void writeBca(char* filename);

int main(int argc, char** argv)
{
	char* flags;
    char* outfilename;

#ifndef _DEBUG
	if (argc < 1) {
		// print usage here and return, since that's what you need.
		std::cout << "makebca [FLAGS] [OUTPUT]" << std::endl;
	}
	else {
		flags = argv[1];
		outfilename = argv[2];
	}
#else
	flags = "BCA_NO_DRM";
	outfilename = "bca.bin";
#endif

	std::cout << "NROM BCA Maker" << std::endl;
	std::cout << "(C) Cyndanera 2023-2024 Llc." << std::endl;

    if(flags = "BCA_DRM")
	{
        wp->optInfo[0] = 1;
	}

	initBca();
	setAuthcode();
	encryptAuthcode();
	writeBca(outfilename);

	return 0;
}

void initBca()
{
	srand(time(NULL));

	for (int i = 0; i < 52; i++)
	{
	    wp->optInfo[i] = 0;
	}

	wp->manufacturer[0] = 0;
    wp->manufacturer[1] = 0;

	wp->recorderDevice[0] = 0;
	wp->recorderDevice[1] = 0;

	wp->APMRecorderDevice = 0;

	wp->discDate[0] = 0;
	wp->discDate[1] = 0;

	wp->discTime[0] = 0;
	wp->discTime[1] = 0;

	wp->discNumber[0] = 0;
	wp->discNumber[1] = 0;

	for (int i = 0; i < 8; i++)
	{
	    wp->key[i] = rand() % 90;
	}

	for (int i = 0; i < 4; i++)
	{
	    wp->id[i] = 0;
	}

	for (int i = 0; i < 6; i++)
	{
	    wp->psn[i].start = 0;
	    wp->psn[i].end = 0;
	}

	for (int i = 0; i < 64; i++)
	{
	    wp->pad[i] = 0;
	}
}

void setAuthcode()
{
	// PSN1
	wp->psn[0].start = AUTHCODE_SECTOR_START;
	wp->psn[0].end = AUTHCODE_SECTOR_END;

	// PSN2
	wp->psn[1].start = AUTHCODE_SECTOR_START;
	wp->psn[1].end = AUTHCODE_SECTOR_END;

	// PSN3
	wp->psn[2].start = AUTHCODE_SECTOR_START;
	wp->psn[2].end = AUTHCODE_SECTOR_END;

	// PSN4
	wp->psn[3].start = AUTHCODE_SECTOR_START;
	wp->psn[3].end = AUTHCODE_SECTOR_END;

	// PSN5
	wp->psn[4].start = AUTHCODE_SECTOR_START;
	wp->psn[4].end = AUTHCODE_SECTOR_END;

	// PSN6
	wp->psn[5].start = AUTHCODE_SECTOR_START;
	wp->psn[5].end = AUTHCODE_SECTOR_END;
}

void encryptAuthcode()
{
	// encrypt authcode
	for (int i = 0; i < 4; i++)
    {
		// PSN1
        wp->psn[0].start = wp->psn[0].start ^ wp->key[i % 8];
		wp->psn[0].end = wp->psn[0].end ^ wp->key[i % 8];

		// PSN2
        wp->psn[1].start = wp->psn[1].start ^ wp->key[i % 8];
		wp->psn[1].end = wp->psn[1].end ^ wp->key[i % 8];

		// PSN3
        wp->psn[2].start = wp->psn[2].start ^ wp->key[i % 8];
		wp->psn[2].end = wp->psn[2].end ^ wp->key[i % 8];

		// PSN4
        wp->psn[3].start = wp->psn[3].start ^ wp->key[i % 8];
		wp->psn[3].end = wp->psn[3].end ^ wp->key[i % 8];

		// PSN5
        wp->psn[4].start = wp->psn[4].start ^ wp->key[i % 8];
		wp->psn[4].end = wp->psn[4].end ^ wp->key[i % 8];

		// PSN6
        wp->psn[5].start = wp->psn[5].start ^ wp->key[i % 8];
		wp->psn[5].end = wp->psn[5].end ^ wp->key[i % 8];
    }
}

void writeBca(char* filename)
{
	std::fstream file;

	// write bca file
	file.open(filename, std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		for (int i = 0; i < 51; i++)
	    {
		    file.write(reinterpret_cast<const char *>(&wp->optInfo[i]), sizeof(wp->optInfo[i]));
	    }
        
		file.write(reinterpret_cast<const char *>(&wp->manufacturer[0]), sizeof(wp->manufacturer[0]));
		file.write(reinterpret_cast<const char *>(&wp->manufacturer[1]), sizeof(wp->manufacturer[1]));

		file.write(reinterpret_cast<const char *>(&wp->recorderDevice[0]), sizeof(wp->recorderDevice[0]));
		file.write(reinterpret_cast<const char *>(&wp->recorderDevice[1]), sizeof(wp->recorderDevice[1]));

		file.write(reinterpret_cast<const char *>(&wp->APMRecorderDevice), sizeof(wp->APMRecorderDevice));

		file.write(reinterpret_cast<const char *>(&wp->discDate[0]), sizeof(wp->discDate[0]));
		file.write(reinterpret_cast<const char *>(&wp->discDate[1]), sizeof(wp->discDate[1]));
		file.write(reinterpret_cast<const char *>(&wp->discDate[2]), sizeof(wp->discDate[2]));

		file.write(reinterpret_cast<const char *>(&wp->discTime[0]), sizeof(wp->discTime[0]));
		file.write(reinterpret_cast<const char *>(&wp->discTime[1]), sizeof(wp->discTime[1]));
		file.write(reinterpret_cast<const char *>(&wp->discTime[2]), sizeof(wp->discTime[2]));

		file.write(reinterpret_cast<const char *>(&wp->discNumber[0]), sizeof(wp->discNumber[0]));
		file.write(reinterpret_cast<const char *>(&wp->discNumber[1]), sizeof(wp->discNumber[1]));
		file.write(reinterpret_cast<const char *>(&wp->discNumber[2]), sizeof(wp->discNumber[2]));

	    for (int i = 0; i < 8; i++)
	    {
	    	file.write(reinterpret_cast<const char *>(&wp->key[i]), sizeof(wp->key[i]));
	    }

	    for (int i = 0; i < 4; i++)
	    {
	        file.write(reinterpret_cast<const char *>(&wp->id[i]), sizeof(wp->id[i]));
	    }

	    for (int i = 0; i < 6; i++)
	    {
		    file.write(reinterpret_cast<const char *>(&wp->psn[i].start), sizeof(wp->psn[i].start));
	    	file.write(reinterpret_cast<const char *>(&wp->psn[i].end), sizeof(wp->psn[i].end));
	    }

		// padding
		for (int i = 0; i < 64; i++)
	    {
	        file.write(reinterpret_cast<const char *>(&wp->pad[i]), sizeof(wp->pad[i]));
		}

		// close the file
		file.close();
	}
}
