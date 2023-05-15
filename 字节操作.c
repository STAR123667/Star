#include "stdio.h"
#define uint unsigned int
#define uchar unsigned char
uint adc =0x0ffe;

uchar high_bit=0x0f;
uchar low_bit=0xfe;
uint  swap_value=0;

void main()
{
	//*  取高低八位  *//
//	low_bit = adc & 0xff;  //取低位
//	high_bit = adc >> 8;  //取高位

//             high_bit = (adc >> 8) & 0XFF; //先取高八位
//             low_bit = adc & 0XFF;      //再取低八位

//printf("低位 = %X \n", low_bit);
//printf("高位 = %X \n", high_bit);

//--------------------------------------------------//
	//*  高低八位合并一起  *//
	swap_value = high_bit;
	swap_value <<= 8;
	swap_value |= low_bit;

	printf("值为：%X \n", swap_value);

while(1){}
}