import numpy as np, pandas as pd,matplotlib.pyplot as plt

df=pd.read_csv('temp.txt',header=None)
arr=df[0].values
plt.plot(arr,'.');plt.show()
