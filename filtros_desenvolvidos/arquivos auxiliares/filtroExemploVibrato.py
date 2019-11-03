import numpy as np
from numpy.fft import rfft
from scipy.io.wavfile import write
from scipy.io.wavfile import read
from math import sin, pi
import sys

input = []
output = []
temp_input = []

filename = str(sys.argv[1])

sr, temp_input = read(filename)

#parametros de configuracao

TABLE_SIZE = sr
TABLE_AVERAGE = round(0.01875*sr)               #aumentar esse numero aumenta o tamanho do atraso fixo
TABLE_WIDTH = round(0.0025*sr)                  #aumentar esse numero aumenta a variacao no tempo do delay
TABLE_MAX = TABLE_AVERAGE + TABLE_WIDTH         #nao deve ser alterado
A = 0.5                                         #peso da entrada nao alterada
B = 0.25                                        #peso da entrada com atraso fixo
C = 0.25                                        #peso da entrada com atraso variavel
TABLE_STEP = 3                                  #aumentar esse numero aumenta a frequencia de variacao do delay
AMP_STEP = 8                                    #aumentar esse numero aumenta a frequencia de variacao da amplitude
M = 0.7                                         #aumentar esse numero torna a oscilacao mais proxima de uma senoide
N = 0.3                                         #aumentar esse numero suaviza a oscilacao

#look-up table

lookUpTable = [0]*TABLE_SIZE
lookUpTable[0] = TABLE_AVERAGE

for n in range(1, TABLE_SIZE):
    lookUpTable[n] = round( (TABLE_AVERAGE + TABLE_WIDTH*sin( ((2*pi)/TABLE_SIZE)*n ))*M + N*lookUpTable[n-1] )

#filtro

tablePtr = 0
ampPtr = 0

for i in range(len(temp_input)):
    input.append(np.int16(round(temp_input[i])))
    if i < TABLE_MAX:
        output.append(input[i])
    else:
        output.append(np.int16(round( ((lookUpTable[ampPtr]/TABLE_MAX + 1)/2) * (temp_input[i]*A + temp_input[i-TABLE_AVERAGE]*B + temp_input[i-lookUpTable[tablePtr]]*C) )))
        tablePtr = tablePtr + TABLE_STEP
        ampPtr = ampPtr + AMP_STEP
        if tablePtr > TABLE_SIZE-1:
            tablePtr = tablePtr - TABLE_SIZE
        if ampPtr > TABLE_SIZE-1:
            ampPtr = ampPtr - TABLE_SIZE

write('output.wav', sr, np.array(output))

print("OK")