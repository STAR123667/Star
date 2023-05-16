#include "main.h"
enum keyNum
{
     key1,    /*默认为0*/
	 key2,    /*递增为1*/
	 key3=4,  /*赋值为4*/
	 key4,    /*递增为5*/
	 key5,    /*递增为6*/
}Key; 
// enum keyNum
// {
     // key1=0x01,    /*赋值为1*/
	 // key2=0x02,    /*赋值为2*/
	 // key3=0x03,    /*赋值为3*/
	 // key4=0x04,    /*赋值为4*/
	 // key5=0x05,    /*赋值为5*/
// }Key;    
struct TestNum
{
     int   Num1;
	 char  Num2;
	 float Num3; 
}Key2;
uint8_t DataNum[key5]; /*定义数组,元素为枚举的最大元素值6*/
int main(void)
{
   DataNum[0] = 0x01;	/*数组元素赋值*/
   DataNum[1] = 0x02;	
   DataNum[2] = 0x03;	
   DataNum[3] = 0x04;	
   DataNum[4] = 0x05;	
   DataNum[5] = 0x06;	
  
   Key2.Num1 = 0x10;    /*结构体成员赋值*/
   Key2.Num2 = 'a';
   Key2.Num3 = 1.024;
	
   while(1)
   {   
   }
}

