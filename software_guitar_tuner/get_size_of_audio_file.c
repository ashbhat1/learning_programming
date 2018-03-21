#include<stdio.h>
#include<stdlib.h>

int main(){
	size_t size,size_begin,size_end;
	FILE *fp;
	fp = fopen("output2.wav","rb");
	printf("File pointer %p\n",fp);
	fseek(fp,0,SEEK_END);
	size_end = ftell(fp);
	//size = ftell(fp); // THIS SEEM TO WORKS
	fseek(fp,0,SEEK_SET);
	size_begin   = ftell(fp);
	size=size_end-size_begin;
	printf("size_end: %d\n",size_end);
	printf("size_begin: %d\n", size_begin);
	printf("File size: %d\n",size);
	fclose(fp);
}
