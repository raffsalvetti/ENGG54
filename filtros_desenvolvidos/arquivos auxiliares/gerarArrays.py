import numpy as np
from scipy.io.wavfile import read
import sys

input = []

count = 0

filename = str(sys.argv[1])
lineSize = int(sys.argv[2])

sr, input = read(filename)

lines = input.size//lineSize

noext = filename.split(".", 1)[0]

with open(noext + ".txt", "w") as file:

    for i in range(lines*lineSize):
        if i%lineSize == 0:
            if count != 0:
                file.write("};\n")
            file.write("const Int16 samples" + str(count) + "[SIGNAL_SIZE] = {")
            count = count + 1
        file.write(str(np.int16(round(input[i]))))
        if i%lineSize != lineSize-1:
            file.write(",")
    file.write("};\n")
    
print("Saved as " + noext + ".txt")