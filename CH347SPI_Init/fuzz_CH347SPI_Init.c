#include <windows.h>
#include <stdio.h>
#include "CH347DLL_EN.H"

enum fieldType {U_CHAR, U_SHORT, U_LONG};

struct spiCfgFuzzer {
	char *name;
	unsigned int min;
	unsigned int max;
	union {
		unsigned char *uChar;
		unsigned short *uShort;
		unsigned long *uLong;
	} field;
	enum fieldType fieldT;
};

int main(int argc, char** argv) {
	ULONG i = 0;
	BOOL ret = FALSE;
	mSpiCfgS spiCfg = {0};
	struct spiCfgFuzzer spiCfgFuzzTests[] = {
		{.name = "iMode", .min = 0, .max = 3, .field.uChar = &spiCfg.iMode, .fieldT = U_CHAR},
		{.name = "iClock", .min = 0, .max = 7, .field.uChar = &spiCfg.iClock, .fieldT = U_CHAR},
		{.name = "iByteOrder", .min = 0, .max = 1, .field.uChar = &spiCfg.iByteOrder, .fieldT = U_CHAR},
		{.name = "iSpiWriteReadInterval", .min = 0, .max = 20, .field.uShort = &spiCfg.iSpiWriteReadInterval, .fieldT = U_SHORT},
		{.name = "iSpiWriteReadInterval", .min = 65530, .max = 65535, .field.uShort = &spiCfg.iSpiWriteReadInterval, .fieldT = U_SHORT},
		{.name = "iSpiOutDefaultData", .min = 0, .max = 4, .field.uChar = &spiCfg.iSpiOutDefaultData, .fieldT = U_CHAR},
		{.name = "iSpiOutDefaultData", .min = 0xFC, .max = 0xFF, .field.uChar = &spiCfg.iSpiOutDefaultData, .fieldT = U_CHAR},
		{.name = "CS1Polarity", .min = 0, .max = 1, .field.uChar = &spiCfg.CS1Polarity, .fieldT = U_CHAR},
		{.name = "CS2Polarity", .min = 0, .max = 1, .field.uChar = &spiCfg.CS2Polarity, .fieldT = U_CHAR},
		{.name = "iActiveDelay", .min = 0, .max = 20, .field.uShort = &spiCfg.iActiveDelay, .fieldT = U_SHORT},
		{.name = "iActiveDelay", .min = 65530, .max = 65535, .field.uShort = &spiCfg.iActiveDelay, .fieldT = U_SHORT},
		{.name = "iDelayDeactive", .min = 0, .max = 20, .field.uLong = &spiCfg.iDelayDeactive, .fieldT = U_LONG},
		{.name = "iDelayDeactive", .min = 65530, .max = 65535, .field.uLong = &spiCfg.iDelayDeactive, .fieldT = U_LONG},
	};

	i = CH347OpenDevice(0);
	if (i == INVALID_HANDLE_VALUE) {
		printf("Error! CH347OpenDevice\n");
	}

	CH347SetTimeout(i, 500, 500);

	for (int j = 0; j < sizeof(spiCfgFuzzTests)/sizeof(struct spiCfgFuzzer); j++) {
		struct spiCfgFuzzer s = spiCfgFuzzTests[j];
		printf("%s %d-%d\n", s.name, s.min, s.max);
		for (int val = s.min; val < s.max+1; val++) {
			printf("%d\n", val);
			switch (s.fieldT) {
			case U_CHAR:
				*(s.field.uChar) = (unsigned char)val;
				break;
			case U_SHORT:
				*(s.field.uShort) = (unsigned short)val;
				break;
			case U_LONG:
				*(s.field.uLong) = (unsigned long)val;
				break;
			}
			ret = CH347SPI_Init(0, &spiCfg);
			if (!ret) {
				printf("Error! CH347SPI_Init\n");
			}
		}
		switch (s.fieldT) {
		case U_CHAR:
			*(s.field.uChar) = 0;
			break;
		case U_SHORT:
			*(s.field.uShort) = 0;
			break;
		case U_LONG:
			*(s.field.uLong) = 0;
			break;
		}
	}
	return 0;
}
