/* nic3-14159's terrible, unsafe CH347 REPL */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "CH347DLL_EN.H"

#define LINE_LENGTH 1024
int main(int argc, char** argv) {
	ULONG i = 0;
	BOOL ret = FALSE;
	mSpiCfgS spiCfg = {0};
	spiCfg.iChipSelect = 0x80;
	i = CH347OpenDevice(0);
	if (i == INVALID_HANDLE_VALUE) {
		printf("Error! CH347OpenDevice\n");
	}

	CH347SetTimeout(i, 500, 500);
	ret = CH347SPI_Init(0, &spiCfg);
	printf(!ret ? " Error! CH347SPI_Init\n" : "\n");

	char input_buffer[LINE_LENGTH];
	char *args[32] = {NULL};
	while (1) {
		printf("CH347: ");
		fflush(stdout);
		while(fgets(input_buffer, LINE_LENGTH, stdin) == NULL);
		char *token;
		int idx = 0;
		token = strtok(input_buffer, " \n");
		while (token != NULL) {
			args[idx] = token;
			idx++;
			token = strtok(NULL, " \n");
		}
		args[idx] = NULL;
		if (strcmp(input_buffer, "exit") == 0) {
			break;
		} else if (strcmp(input_buffer, "ccs") == 0) {
			if (args[1] == NULL) {
				printf("Usage: ccs <cs>\n");
				printf("\tcs: 0 = unset CS, 1 = set CS\n");
				continue;
			}
			CH347SPI_ChangeCS(0, atoi(args[1]));
		} else if (strcmp(input_buffer, "scs") == 0) {
			if (idx < 3) {
				printf("Usage: scs <mask2><mask1> <cs>\n");
				printf("\tmaskx: 0 = change CS, 1 = ignore CS\n");
				printf("\tcs: 0 = unset CS, 1 = set CS\n");
				continue;
			}
			unsigned short mask = strtol(args[1], NULL, 16);
			unsigned short cs = strtol(args[2], NULL, 16);
			mask = ((mask & 0x10) << 4) | (mask & 1);
			cs = ((cs & 0x10) << 4) | (cs & 1);
			CH347SPI_SetChipSelect(0, mask, cs, 0, 0, 0);
			
		} else if (strcmp(input_buffer, "r") == 0) {
			continue;	
		} else if (strcmp(input_buffer, "w") == 0) {
			continue;	
		} else if (strcmp(input_buffer, "wr") == 0) {
			continue;	
		} else if (strcmp(input_buffer, "set") == 0) {
			continue;	
		} else {
			printf("Unknown command: %s\n", input_buffer);
		}
	}
	return 0;
}
