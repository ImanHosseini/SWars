#pragma once
#include <stdlib.h>
typedef struct DrawFrame {
	char* dat;
	DrawFrame() { dat = NULL; };

	DrawFrame(char* dat) :dat(dat) {};
} DrawFrame;


