#include <stdio.h>

struct spiCfgFuzzer {
	char *name;
	unsigned int min;
	unsigned int max;
};

int main(int argc, char** argv) {
	struct spiCfgFuzzer spiCfgFuzzTests[] = {
		{.name = "iMode", .min = 0, .max = 3},
		{.name = "iClock", .min = 0, .max = 7},
		{.name = "iByteOrder", .min = 0, .max = 7},
		{.name = "iByteOrder", .min = 253, .max = 255},
		{.name = "iSpiWriteReadInterval", .min = 0, .max = 20},
		{.name = "iSpiWriteReadInterval", .min = 65530, .max = 65535},
		{.name = "iSpiOutDefaultData", .min = 0, .max = 4},
		{.name = "iSpiOutDefaultData", .min = 0xFC, .max = 0xFF},
		{.name = "iChipSelect", .min = 0, .max = 0xFF},
		{.name = "iChipSelect", .min = 0xFFFFFFFDUL, .max = 0xFFFFFFFFUL},
		{.name = "CS1Polarity", .min = 0, .max = 4},
		{.name = "CS2Polarity", .min = 0, .max = 4},
		{.name = "iIsAutoDeativeCS", .min = 0, .max = 20},
		{.name = "iIsAutoDeativeCS", .min = 65530, .max = 65535},
		{.name = "iActiveDelay", .min = 0, .max = 20},
		{.name = "iActiveDelay", .min = 65530, .max = 65535},
		{.name = "iDelayDeactive", .min = 0, .max = 20},
		{.name = "iDelayDeactive", .min = 0xFFFFFFFDUL, .max = 0xFFFFFFFFUL},
	};
	char buffer[256] = {0};

	for (size_t j = 0; j < sizeof(spiCfgFuzzTests)/sizeof(struct spiCfgFuzzer); j++) {
		struct spiCfgFuzzer s = spiCfgFuzzTests[j];
		printf("\n%s %Xh-%Xh\n", s.name, s.min, s.max);
		for (unsigned long long val = s.min; val < (unsigned long long)s.max+1; val++) {
			for (unsigned line = 0; line < 4; line++) {
				fgets(buffer, sizeof(buffer), stdin);
				/* This code just adds annotations */
				
				if (line == 0) {
					printf("% 8Xh: %s", val, buffer);
				} else {
					printf("           %s", buffer);
				}
				
				
				/* This code only prints the long data transfers */ 
				/*
				if (line == 1) {
					printf("% 8Xh: %s", val, buffer);
				} else if (line == 2) {
					printf("           %s", buffer);
				}
				*/

				/* Print only the bulk out transfers */
				/*if (line == 2) {
					printf("% 8Xh: %s", val, buffer);
				}*/
			}
		}
	}
	return 0;
}
