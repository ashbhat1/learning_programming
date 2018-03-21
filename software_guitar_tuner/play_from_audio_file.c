/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

int main() {
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
  int ret;
  
  //Read from audio file
  size_t fsize, size_begin, size_end;
  FILE *fp;
  fp = fopen("output2.wav","rb");
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
  int z=0;
  /*for(z=0;z<size;z++){
    printf("0x%x ",audiofile[z]);
  }
  printf("\n");*/

  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  /* explains frames and channels and such
  https://www.alsa-project.org/main/index.php/FramesPeriods*/


  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);

  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params,
                                    &val, &dir);
  /* 5 seconds in microseconds divided by
   * period time */

  

  int i=0;
  int idx=0;
  /*can't use while loop and look for 0 char value because
    it means a value of 0V for the output 
    (https://stackoverflow.com/questions/27006249/wav-file-contents-what-does-those-byte-values-mean)*/
  for(idx=0;idx<fsize;idx+=size){
    //printf("0x%x\n",*audiofile);
    //audiofile++;
    //printf("Buffer: ");
    for(i=0;i<size;i++){
      buffer[i]=audiofile[i];
      //buffer[i] = (char)((i^2)*1000);
      //printf("0x%x ",audiofile[i]);
    }
    printf("idx: %d\n",idx);
    //audiofile+=size;
    //idx+=size;
    //printf("idx: %d ",idx++);
    //printf("0x%x\n",audiofile[i-1]);
    rc = snd_pcm_writei(handle, buffer, frames);
    if (rc == -EPIPE) {
     /* EPIPE means underrun */
      fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(handle);
    } else if (rc < 0) {
      fprintf(stderr,
              "error from writei: %s\n",
              snd_strerror(rc));
    }  else if (rc != (int)frames) {
      fprintf(stderr,
              "short write, write %d frames\n", rc);
    }
    audiofile+=size;
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
  printf("idx: %d\n",idx);
  return 0;
}
