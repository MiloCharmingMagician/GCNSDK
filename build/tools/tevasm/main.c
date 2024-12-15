#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50
#define MAX_OPCODES 61

const char* opcodes[MAX_OPCODES] = {
	"nop",
	"stage1",
	"stage2",
	"stage3",
	"stage4",
	"stage5",
	"stage6",
	"stage7",
	"stage8",
	"stage9",
	"stage10",
	"stage11",
	"stage12",
	"stage13",
	"stage14",
	"stage15",
	"stage16",
    "add",
    "sub",
    "comp_r8_gt",
    "comp_r8_eq",
    "comp_gr16_gt",
    "comp_gr16_eq",
    "comp_bgr24_gt",
    "comp_bgr24_eq",
    "comp_rgb8_gt",
    "comp_rgb8_eq",
    "tb_zero",
    "tb_addhalf",
    "tb_subhalf",
    "cs_scale_1",
    "cs_scale_2",
    "cs_scale_4",
    "cs_divide_2",
    "cc_c0",
    "cc_c1",
    "cc_c2",
    "cc_cprev",
    "cc_a0",
    "cc_a1",
    "cc_a2",
    "cc_aprev",
    "cc_texc",
    "cc_texrrr",
    "cc_texggg",
    "cc_texbbb",
    "cc_texa",
    "cc_rasc",
    "cc_rasa",
    "cc_one",
    "cc_half",
    "cc_konst",
    "cc_zero",
    "ca_a0",
    "ca_a1",
    "ca_a2",
    "ca_aprev",
    "ca_texa",
    "ca_rasa",
    "ca_konst",
    "ca_zero"
};

char wordList[MAX_WORDS][MAX_WORD_LENGTH];
int wordCount = 0;

int findOpcode(const char *str);
void loadShader(const char* filename);
void compileShader(const char* filename);

int main(int argc, char* argv)
{
	loadShader("tev.txt");
    compileShader("tev.bin");

	printf("\n");
    system("pause");
    return 0;
}

int findOpcode(const char *word)
{
	int i = 0;

    for (i = 0; i < MAX_OPCODES; i++) {
        if (strcmp(word, opcodes[i]) == 0) {
			printf("word: %s\n", word);
			printf("opcode: %s\n", opcodes[i]);
            return i; // Return the index if the word matches
        }
    }
    return -1; // Return -1 if not found
}

void loadShader(const char* filename)
{
	FILE *fp;
	char line[256];
    char *token;

    fp = fopen(filename, "r");
	if(fp == NULL) {
        printf("error reading file\n");
    }
    else
	{
	// Read the entire line from the file
    if (fgets(line, sizeof(line), fp) != NULL) {
        // Tokenize the line using comma as a delimiter
        token = strtok(line, ",");
        while (token != NULL && wordCount < MAX_WORDS) {
            // Trim newline characters, if any
            token[strcspn(token, "\n")] = 0;

            // Copy the token into the global list
            strncpy(wordList[wordCount], token, MAX_WORD_LENGTH - 1);
            wordList[wordCount][MAX_WORD_LENGTH - 1] = '\0'; // Ensure null termination
            wordCount++;

            // Get the next token
            token = strtok(NULL, ",");
        }
    } else {
        printf("File is empty or an error occurred while reading.\n");
    }

	   fclose(fp);
	}
}

void compileShader(const char* filename)
{
	FILE *fp;
    int i = 0;
    char opcodeByte;
	int opcodeIndex;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error creating file");
        return;
    }

    for (i = 0; i < wordCount; i++) {
        opcodeIndex = findOpcode(wordList[i]);

        if (opcodeIndex >= 0) {
            opcodeByte = (char)opcodeIndex; // Convert opcode index to 1-byte value
            fwrite(&opcodeByte, sizeof(char), 1, fp);
        } else {
            opcodeByte = (char)-1; // Use -1 for unknown opcodes (0xFF in unsigned byte)
           fwrite(&opcodeByte, sizeof(char), 1, fp);
       }
    }
}
