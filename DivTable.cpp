
#include "DivTable.h"
#include <stdint.h>

unsigned char TVPDivTable[256*256];

void InitDivTable(void)
{
	int a,b;

	for(b=0; b<256; b++)
	{
		TVPDivTable[(0<<8)+b] = 0;
		for(a=1; a<256; a++)
		{
			uint32_t tmp = (uint32_t)(b*255/a);
			if(tmp > 255) tmp = 255;
			TVPDivTable[(a<<8)+b] = (uint8_t)(tmp);
		}
	}
}
