import numpy as np
from scipy.io.wavfile import read
import sys

input = []

mode = str(sys.argv[1])
filename = str(sys.argv[2])

sr, input = read(filename)

noext = filename.split(".", 1)[0]

with open(noext + ".csv", "w") as file:

    if mode == "-f":
        for i in range(input.size):
            file.write(str(np.int16(round(input[i]))) + ",") #saida num arquivo
        print("Saved as " + noext + ".csv")
        file.write("\n")

    elif mode == "-p":
        for i in range(input.size):
            print(str(np.int16(round(input[i]))) + ",") #se quiser saida padrao pra jogar num pipe por exemplo
        print("\n")
    
    else:
        print("FAIL")