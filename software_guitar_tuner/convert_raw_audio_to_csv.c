#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int main(){
 
  int ret; 
  short val;
  short *ptr;  
  size_t fsize, size_begin, size_end;
  FILE *fp, *csvfile;
  fp = fopen("output2.raw","rb");
  printf("File pointer %p\n",fp);
  fseek(fp,0,SEEK_END);
  size_end = ftell(fp);
  fseek(fp,0,SEEK_SET);
  size_begin = ftell(fp);
  fsize = size_end - size_begin;
  printf("size_end: %d\n",(int)size_end);
  printf("size_begin: %d\n",(int)size_begin);
  printf("File size: %d\n",(int)fsize);
  char *audiofile = malloc(fsize+1);
  fseek(fp,0,SEEK_SET);
  if((ret=fread(audiofile,fsize,1,fp)<0)){
    fprintf(stderr,"unable to read in file %d\n",ret);
  }
  fclose(fp);
  int idx=0;

  csvfile = fopen("output2csv.csv","w");
  ptr=(short *)audiofile;
  for(idx=0;idx<fsize;idx+=2){
    fprintf(csvfile,"%hi,",*ptr++);
  }
  fclose(csvfile);
}
