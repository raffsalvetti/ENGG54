import numpy as np
from scipy.io.wavfile import write
import sys

mode = str(sys.argv[1])
sampleRate = int(sys.argv[2])
filename = str(sys.argv[3])

noext = "_" + filename.split(".", 1)[0] + "_"

if mode == "-f":

    with open(filename, 'r') as file:
        content = file.read().rstrip()

elif mode == "-p":
    content = sys.stdin.read().rstrip()

try:
    content
except NameError:
    print("FAIL")

else:

    values = list(filter(None, content.split(",")))

    samples = [np.int16(s) for s in values]

    write(noext + ".wav", sampleRate, np.array(samples))

    print("Saved as " + noext + ".wav")