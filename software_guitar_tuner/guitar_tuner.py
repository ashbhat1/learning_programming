"""PyAudio example: Record a few seconds of audio and save to a WAVE file."""

import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack

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

def calculate_fft(data,rate):
    N = len(data)
    sample_freq = 1.0/float(rate)
    freqs = np.linspace(0.0,1.0/(2.0*sample_freq),N/2)
    datafft = scipy.fftpack.fft(data)
    datafft = 2.0/N*np.abs(datafft[:N//2])
    return freqs,datafft

def find_note_freq(freqs,datafft):
    msk = freqs > 80
    maxamploc = np.where(datafft[msk]==np.max(datafft[msk]))[0]
    return freqs[msk][maxamploc[0]]
    

print "recording 1st round"
data1,rate1 = record_audio('output.wav')
freqs1,datafft1 = calculate_fft(data1,rate1)
note1 = find_note_freq(freqs1,datafft1)
print("Frequency is: "+str(note1))
raw_input('HIT ENTER WHEN YOU WANT TO START THE NEXT RECORDING')
print "recording 2nd round"
data2,rate2 = record_audio('output.wav')
freqs2,datafft2 = calculate_fft(data2,rate2)
note2 = find_note_freq(freqs2,datafft2)
print("Frequency is: "+str(note2))
plt.figure(1)
plt.plot(data1,'r');
plt.plot(data2,'b');
plt.title('data1 in red, data2 in blue')
plt.figure(2)
plt.plot(freqs1,datafft1,'r');
plt.plot(freqs2,datafft2,'b');

plt.show()
    

