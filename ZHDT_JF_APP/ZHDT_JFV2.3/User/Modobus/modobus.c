#include "modobus.h"

void ModbusCrc16(unsigned char Frame[], unsigned char Length) {
	unsigned int crc16 = 0xffff;
	unsigned char ByteIndex, n;
	for (ByteIndex = 0; ByteIndex < Length - 2; ByteIndex++)
	{
		crc16 ^= Frame[ByteIndex];
		for (n = 0; n < 8; n++)
		{
			if (crc16 & 1) 
			{
				crc16 >>= 1;
				crc16 ^= 0xA001;
			} 
			else 
			{
			crc16 >>= 1;
			}
		}	
	}
	Frame[Length - 2] = crc16;
	Frame[Length - 1] = crc16 >> 8;
}
