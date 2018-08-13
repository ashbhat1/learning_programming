import numpy as np

data2=np.genfromtxt('output2csv.csv',delimiter=',')
x=data2[:10000]
n=len(x)
variance = np.var(x)
r = np.correlate(x,x,mode='full')[-n:]
result = r/(variance*(np.arange(n,0,-1)))
plt.plot(result);plt.show()
