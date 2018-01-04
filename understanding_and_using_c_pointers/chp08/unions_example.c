#include<stdio.h>
#include<stdlib.h>

typedef union _conversion{
  float fNum;
  unsigned int uiNum;
} Conversion;

int isPositive1(float number){
  Conversion conversion = { .fNum = number};
  return (conversion.uiNum & 0x80000000) == 0;
}

typedef union _conversion2 {
  float *fNum;
  unsigned int *uiNum;
} Conversion2;

int isPositive2(float number) {
  Conversion2 conversion;
  conversion.fNum = &number;
  return (*conversion.uiNum & 0x80000000)==0;
}

int isPositive3(float number) {
  unsigned int *ptrValue = (unsigned int *)&number;
  return (*ptrValue & 0x80000000) == 0;
}

int main() {
  float number[2] =  {30.90f,-30.90f};
  //float negnumber = -30.90;
  int i = 0;
  for(i = 0;i<2;i++){
    printf("Result of %0.3f for isPositive1 is: %d\n",number[i],isPositive1(number[i]));
    printf("\n");
    printf("Result of %0.3f for isPositive2 is: %d\n",number[i],isPositive2(number[i]));
    printf("\n");
    printf("Result of %0.3f for isPositive3 is: %d\n",number[i],isPositive3(number[i]));
    printf("\n");
  }
  return 0;
}

