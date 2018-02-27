"""PyAudio example: Record a few seconds of audio and save to a WAVE file."""

import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
import pdb

def record_audio(outfname):
    """
    This function handles the recording the data gives a file name and returns a numpy array of the data
    Params
    ------
    outfname: str, filename of the audio file

    Return
    ------
    data: ndarray, returns numpy array with the raw ADC data from the mic
    """
    CHUNK = 1024
    FORMAT = pyaudio.paInt16
    CHANNELS = 1
    RATE = 44100
    RECORD_SECONDS = 1#5
    WAVE_OUTPUT_FILENAME = outfname#"output.wav"

    p = pyaudio.PyAudio()

    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print("* recording")

    frames = []
    data = []

    val = RATE / CHUNK * RECORD_SECONDS
    print(val)
    for i in range(0, int(val)):
        strdata = stream.read(CHUNK)
        frames.append(strdata)

    print("* done recording")


    stream.stop_stream()
    stream.close()
    p.terminate()

    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()
    data = np.concatenate([np.fromstring(x,dtype=np.int16) for x in frames],axis=0)
    return data,RATE

def rolling_window(a,window):
    shape = a.shape[:-1]+(a.shape[-1]-window+1,window);
    strides = a.strides + (a.strides[-1],)
    return np.lib.stride_tricks.as_strided(a,shape=shape,strides=strides)

def calculate_fft(data,rate):
    N = len(data)
    sample_freq = 1.0/float(rate)
    freqs = np.linspace(0.0,1.0/(2.0*sample_freq),N/2)
    datafft = scipy.fftpack.fft(data)
    datafft = 2.0/N*np.abs(datafft[:N//2])
    return freqs,datafft

def find_note_freq(freqs,datafft):
    msk = freqs > 70
    maxamploc = np.where(datafft[msk]==np.max(datafft[msk]))[0]
    return freqs[msk][maxamploc[0]]

def calculate_autocorr(data,rate):
    data2=data.copy();
    data=np.mean(rolling_window(data,7),-1)
    samp=float(rate)
    a=(1/samp)*len(data)
    t=np.linspace(0,a,len(data))
    fdata = data.astype('float')
    norm = (fdata-np.mean(fdata))/np.std(fdata)
    corr = np.correlate(norm,norm,"full")
    result = corr[corr.size/2:] #seems like the data gets reflected (don't understand why)
    #plt.plot(result);plt.show()
    corrdiff=np.diff(result) #take the diff to look for peaks and valleys
    pos=np.where(corrdiff>0)[0] #find only the positive slopes
    neg=np.where(corrdiff<0)[0] #find only the negative slopes
    posloc=np.intersect1d(pos,neg-1) #where do indexes align for diff when you slide neg location over by 1
    idx=np.arange(len(result))
    #plt.plot(idx,result,'b.');plt.plot(idx[posloc+1],result[posloc+1],'r.');plt.show() #plots the raw signal and where the peaks are found
    maxamploc=posloc[np.where(result[posloc+1]==np.max(result[posloc+1]))[0]][0] #find the max amplitude location --> going to convert this to freq
    
    #lag = int(raw_input("Please enter value"));
    #temp = 1/(lag/float(len(fdata))*(t.max()))
    freq0 = 1/(maxamploc/float(len(fdata))*(t.max()))
    return freq0

def hi_or_lo(delta_freqs,closest_note_loc):
    if delta_freqs[closest_note_loc] > 1:
        return 'low'
    elif delta_freqs[closest_note_loc]<-1:
        return 'high'
    else:
        return 'perfect'

def find_closest_note(freqOI):
    open_guitar_string_freqs=np.array([329.63,246.94,196.0,146.83,110.0,82.41])
    open_guitar_string_notes=['E4','B3','G3','D3','A2','E2']
    delta_freqs = open_guitar_string_freqs-freqOI
    min_freq_delta = np.min(np.abs(delta_freqs))
    closest_note_loc = np.where(min_freq_delta==np.abs(delta_freqs))[0][0]
    closest_note = open_guitar_string_notes[closest_note_loc]
    tune_direction=hi_or_lo(delta_freqs,closest_note_loc)
    return closest_note,tune_direction


data=[];rate=[];
freqs=[];datafft=[];
note=[];freq=[];nearest_note=[];
tune_direction=[];
for idx in range(2):
    raw_input("HIT ENTER WHEN YOU WANT TO START THE NEXT RECORDING")
    print "recording "+str(idx)+" round"
    tempdata,temprate = record_audio('output.wav')
    data.append(tempdata);rate.append(temprate)
    tempfreqs,tempdatafft = calculate_fft(data[idx],rate[idx])
    freqs.append(tempfreqs);datafft.append(tempdatafft)
    tempnote = find_note_freq(freqs[idx],datafft[idx])
    note.append(tempnote)
    tempfreq = calculate_autocorr(data[idx],rate[idx])
    freq.append(tempfreq)
    temp_nearest_note,temp_tune_direction = find_closest_note(freq[idx])
    nearest_note.append(temp_nearest_note)
    tune_direction.append(temp_tune_direction)
    print("Frequency is (as calculated by fft): "+str(note[idx]))
    print("Frequency is (as calculated by autocorrelation): "+str(freq[idx]))
    print("The nearest note is "+str(nearest_note[idx]))
    print("Your tuning is "+tune_direction[idx]);

