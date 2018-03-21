import numpy
import matplotlib.pyplot as plt
data = numpy.memmap("output2.wav",dtype='h',mode='r')
#dtype of h is: h              : <type 'numpy.int16'>
#this can be found with 
#for k,v in np.sctypeDict.iteritems(): print '{0:14s} : {1:40s}'.format(str(k), v)
print "VALUES:", data
plt.plot(data)
plt.show()
