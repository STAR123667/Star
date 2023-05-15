#include "test_c.h"
#include "stdlib.h"
#include "time.h"
void main()
{
 int random;
 char i;
 srand(time(NULL));

 for(i=0;i<10;++i)
 {
  random = 68+rand()%33;
  printf("%d\n",random);
 }
 i &= 0x00;
}

