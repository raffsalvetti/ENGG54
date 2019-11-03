from numpy.random import random_integers
from math import sin, pi

#lookUpTable = random_integers(80, 120, 800)

lookUpTable = [0]*32000
lookUpTable[0] = 600

for n in range(1, 32000):
    #lookUpTable[n] = lookUpTable[n]*0.5 + lookUpTable[n-1]*0.25 + lookUpTable[n-2]*0.25
    #lookUpTable[n] = 240 + 100*sin( ((2*pi)/19200)*n )
    #lookUpTable[n] = 960 + 60*sin( ((2*pi)/19200)*n )
    #lookUpTable[n] = (480 + 360*sin( ((2*pi)/19200)*n ))*0.75 + 0.25*lookUpTable[n-1]
    #lookUpTable[n] = 960
    lookUpTable[n] = round( (600 + 80*sin( ((2*pi)/32000)*n ))*0.7 + 0.3*lookUpTable[n-1] )

for n in range(32000):
    print(str(lookUpTable[n]) + ",", end = '')

print("\nOK")