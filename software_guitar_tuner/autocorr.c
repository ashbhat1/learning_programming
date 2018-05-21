#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int getData(char *csvfpath,int **data); //https://stackoverflow.com/questions/34844003/changing-array-inside-function-in-c important as to why you need to pass the address the ptr, so it modifies whats inside the pointer
float calc_mean(int *data,int arrlen);
float calc_variance(int *data, int arrlen,float mean);
float calc_autocorr(int *data, int arrlen, float mean, float var,int lag);
int get_autocorr_vec(int *data, int arrlen, float mean, float var,float **autocorrvect);

int main(){
  int *data;
  int arrlen = getData("output2csv.csv",&data);  
  printf("arrlen: %d\n",arrlen);
  int idx=0;
  //printing out contents of array
  /*for(idx=0;idx<arrlen;idx++){
    printf("%d,",data[idx]);
  }*/
  float mean = calc_mean(data,arrlen);
  printf("Mean: %0.3f\n",mean);
  float var = calc_variance(data,arrlen,mean);
  printf("Variance: %0.3f\n",var);
  float *autocorrvec;
  int vect_len=get_autocorr_vec(data,arrlen,mean, var,&autocorrvec);
  int len=0;
  for(idx=0;idx<vect_len;idx++){
    printf("%0.3f\n",autocorrvec[idx]);
  }

}



float calc_mean(int *data, int arrlen){
  int i=0;
  float sum=0;
  for(i=0;i<arrlen;i++){
    sum+=(float)data[i];
  }
  return (float)(sum/(float)arrlen);
}


float calc_variance(int *data, int arrlen,float mean){
  int i=0;
  float var = 0;
  for(i=0;i<arrlen;i++){
    var += ( pow( ((float)data[i]-mean) , 2 ) / (arrlen-1)  );
  }
  return var;
}


float calc_autocorr(int *data, int arrlen, float mean, float var,int lag){
  float autocv;
  float ac_value;
  int i;

  // Loop to compute autovariance
  autocv = 0.0;	
  for(i=0;i<(arrlen-lag);i++){
    autocv += ( ( (float)data[i] - mean) * ( (float)data[i+lag] - mean) );
  }

  autocv = (1.0 / (arrlen - lag)) * autocv;
  // Autocorrelation is autocovariance divided by variance
  ac_value = autocv / var;
  return ac_value;

}

int get_autocorr_vec(int *data, int arrlen, float mean, float var,float **autocorrvec){
  int lag = 0;
  int divider = 1;
  int vect_len = (int)(arrlen/divider);
  float *autocorr = malloc(sizeof(float)*vect_len);
  *autocorrvec=autocorr;
  for(lag=0;lag<vect_len;lag+=divider){
    autocorr[lag]=calc_autocorr(data,arrlen,mean,var,lag);
  }
  return vect_len;
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



