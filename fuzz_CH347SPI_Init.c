#include <windows.h>
#include <stdio.h>
#include "CH347DLL_EN.H"

int main(int argc, char** argv) {
	ULONG i = 0;
	BOOL ret = FALSE;
	mSpiCfgS spiCfg = {0};

	i = CH347OpenDevice(0);
	if (i == INVALID_HANDLE_VALUE) {
		printf("Error! CH347OpenDevice\n");
	}

	CH347SetTimeout(i, 500, 500);

	return 0;
}
