#include "stdio.h"
#define uint unsigned int
#define uchar unsigned char
uint adc =0x0ffe;

uchar high_bit=0x0f;
uchar low_bit=0xfe;
uint  swap_value=0;

void main()
{
	//*  ȡ�ߵͰ�λ  *//
//	low_bit = adc & 0xff;  //ȡ��λ
//	high_bit = adc >> 8;  //ȡ��λ

//             high_bit = (adc >> 8) & 0XFF; //��ȡ�߰�λ
//             low_bit = adc & 0XFF;      //��ȡ�Ͱ�λ

//printf("��λ = %X \n", low_bit);
//printf("��λ = %X \n", high_bit);

//--------------------------------------------------//
	//*  �ߵͰ�λ�ϲ�һ��  *//
	swap_value = high_bit;
	swap_value <<= 8;
	swap_value |= low_bit;

	printf("ֵΪ��%X \n", swap_value);

while(1){}
}