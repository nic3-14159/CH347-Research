/* nic3-14159's terrible, unsafe CH347 REPL */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CH347DLL_EN.H"

#define LINE_LENGTH 1024

static void set_spiCfg(mSpiCfgS *cfg, char *field, unsigned long value) {
	if (strcmp(field, "iMode") == 0) {
		cfg->iMode = (unsigned char) value;
	} else if (strcmp(field, "iClock") == 0) {
		cfg->iClock = (unsigned char) value;
	} else if (strcmp(field, "iByteOrder") == 0) {
		cfg->iByteOrder = (unsigned char) value;
	} else if (strcmp(field, "iSpiWriteReadInterval") == 0) {
		cfg->iSpiWriteReadInterval = (unsigned short) value;
	} else if (strcmp(field, "iSpiOutDefaultData") == 0) {
		cfg->iSpiOutDefaultData = (unsigned char) value;
	} else if (strcmp(field, "iChipSelect") == 0) {
		cfg->iChipSelect = (unsigned long) value;
	} else if (strcmp(field, "CS1Polarity") == 0) {
		cfg->CS1Polarity = (unsigned char) value;
	} else if (strcmp(field, "CS2Polarity") == 0) {
		cfg->CS2Polarity = (unsigned char) value;
	} else if (strcmp(field, "iIsAutoDeativeCS") == 0) {
		cfg->iIsAutoDeativeCS = (unsigned short) value;
	} else if (strcmp(field, "iActiveDelay") == 0) {
		cfg->iActiveDelay = (unsigned short) value;
	} else if (strcmp(field, "iDelayDeactive") == 0) {
		cfg->iDelayDeactive = (unsigned long) value;
	} else {
		printf("Bad value\n");
	}
}

static void print_buffer(unsigned char *buffer, int len) {
	for (int j = 0; j < len; j++) {
		if (j % 16 == 0) {
			printf("%06X: ", j);
		}
		printf("%02hhX ", buffer[j]);
		if (j % 16 == 15) {
			printf("\n");
		}
	}
	printf("\n");

}
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
	unsigned char *data_buf = NULL;
	unsigned char *data_out = NULL;
	int data_size = 0;
	unsigned long cs = 0;
	unsigned long length1 = 0;
	unsigned long length2 = 0;
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
			unsigned long cs = strtol(args[1], NULL, 16);
			sscanf(args[2], "%li", &length1);
			sscanf(args[3], "%li", &length2);
			memcpy(data_buf, data_out, data_size);
			CH347SPI_Read(0, cs, length1, &length2, data_buf);
			print_buffer(data_buf, length2);
		} else if (strcmp(input_buffer, "w") == 0) {
			unsigned long cs = strtol(args[1], NULL, 16);
			sscanf(args[2], "%li", &length1);
			sscanf(args[3], "%li", &length2);
			memcpy(data_buf, data_out, data_size);
			CH347SPI_Write(0, cs, length1, length2, data_buf);
			print_buffer(data_buf, length2);
		} else if (strcmp(input_buffer, "wr") == 0) {
			unsigned long cs = strtol(args[1], NULL, 16);
			sscanf(args[2], "%li", &length1);
			memcpy(data_buf, data_out, data_size);
			CH347SPI_WriteRead(0, cs, length1, data_buf);
			print_buffer(data_buf, length1);
		} else if (strcmp(input_buffer, "streamSPI4") == 0) {
			unsigned long cs = strtol(args[1], NULL, 16);
			sscanf(args[2], "%li", &length1);
			memcpy(data_buf, data_out, data_size);
			CH347StreamSPI4(0, cs, length1, data_buf);
			print_buffer(data_buf, length1);
		} else if (strcmp(input_buffer, "spicfg") == 0) {
			unsigned long val = strtol(args[2], NULL, 16);
			set_spiCfg(&spiCfg, args[1], val);
			continue;
		} else if (strcmp(input_buffer, "init") == 0) {
			ret = CH347SPI_Init(0, &spiCfg);
			printf(!ret ? " Error! CH347SPI_Init\n" : "");
		} else if (strcmp(input_buffer, "buffer") == 0) {
			data_size = strtol(args[1], NULL, 10);
			data_buf = realloc(data_buf, data_size);
			data_out = realloc(data_out, data_size);
			memset(data_buf, 0xff, data_size);
			memset(data_out, 0xff, data_size);
			for (int j = 0; j < data_size; j++) {
				if (args[j+2] == NULL)
					break;
				data_out[j] = strtol(args[j+2], NULL, 16);
			}
		} else {
			printf("Unknown command: %s\n", input_buffer);
		}
	}
	free(data_buf);
	return 0;
}
