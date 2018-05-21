#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int getData(char *csvfpath,int **data); //https://stackoverflow.com/questions/34844003/changing-array-inside-function-in-c important as to why you need to pass the address the ptr, so it modifies whats inside the pointer


int main(){
  int *data;
  int arrlen = getData("output2csv.csv",&data);  
  printf("arrlen: %d\n",arrlen);
  int idx=0;
  //printing out contents of array
  for(idx=0;idx<arrlen;idx++){
    printf("%d,",data[idx]);
  }
  
}




int getData(char *csvfpath, int **data){
  int idx;
  int ret;
  size_t fsize, size_begin, size_end;
  short *ptr;
  FILE *fp;
  fp=fopen(csvfpath,"r");
  printf("File pointer %p\n",fp);
  fseek(fp,0,SEEK_END);
  size_end = ftell(fp);
  fseek(fp,0,SEEK_SET);
  size_begin = ftell(fp);
  fsize = size_end - size_begin;
  printf("size_end: %d\n",(int)size_end);
  printf("size_begin: %d\n",(int)size_begin);
  printf("File size: %d\n",(int)fsize);
  char *audio_char_data = malloc(fsize+1);
  char *arr_begin = audio_char_data;
  printf("audio_char_data: %p\n",audio_char_data);
  printf("audio_begin    : %p\n",arr_begin);
  if((ret=fread(audio_char_data,fsize,1,fp)<0)){
    fprintf(stderr,"unable to read in file %d\n",ret);
  }
  //fclose(fp);
  //ptr=audiodata;
  //get length of array
  int arrlen=0;
  for(idx=0;idx<fsize-1;idx++){
    if(*(audio_char_data+1)==','){
       arrlen++;
     }
    *audio_char_data++;
    //printf("%c",*audio_char_data++);//uncomment *audio_char_data++ if you want to print out characters
  }
  printf("\nNumber of data points: %d\n",arrlen);
  fclose(fp);
  //creating data array size of the data set
  int *audiodata = malloc(sizeof(int)*arrlen); //multiply by the num of int bytes so that the right amount of bytes can be saved!
  int *audiodataptr = audiodata;
  char temp[10];
  int idx2=0;//keep track of the length of a string (the string of characters that are supposed to be numbers)
  int int_idx=0;
  audio_char_data=arr_begin;
  //making sure that the 2 ptrs point to the same data set
  printf("audio_char_data: %p\n",audio_char_data);
  printf("audio_begin    : %p\n",arr_begin);

  for(idx=0;idx<fsize;idx++){
    //printf("%c",*audio_char_data);
    
    //if you don't find a , then keep adding values to the string
    //convert the list of char numbers to an actual numbers
    if(*audio_char_data !=','){
      temp[idx2]=*audio_char_data;//keep increasing string value
      idx2++;
      }
    else if(*audio_char_data == ','){
      temp[idx2]='\0';//make sure the string is properly converted to a string, so need a null character
      *audiodata=atoi(temp);//convert string to number
      idx2=0;
      int_idx++; //this is used to keep track of the number of integer in csv, should = arrlen
      //printf("audiodata: %d\n",*audiodata);
      audiodata++;
      }
    audio_char_data++;
  }
  printf("int_idx: %d\n",int_idx);
  audiodata=audiodataptr;
  /*for(idx=0;idx<arrlen;idx++){
    printf("%d,",audiodata[idx]);
  }*/

  *data=audiodataptr;
  return arrlen;
  
}



