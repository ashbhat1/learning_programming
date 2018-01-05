"""PyAudio example: Record a few seconds of audio and save to a WAVE file."""

import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt


def record_audio(outfname):
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

    data = np.fromstring(strdata,dtype=np.int16)

    stream.stop_stream()
    stream.close()
    p.terminate()

    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()
    return data


print "recording 1st round"
data1 = record_audio('output.wav')
raw_input('HIT ENTER WHEN YOU WANT TO START THE NEXT RECORDING')
print "recording 2nd round"
data2 = record_audio('output.wav')
plt.plot(data1,'r');
plt.plot(data2,'b');
plt.title('data1 in red, data2 in blue')
plt.show()
    

