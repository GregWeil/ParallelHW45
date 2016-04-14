import sys
import struct

with open("output.txt", "rb") as f:
    rowsize = int(sys.argv[1])
    done = False
    while not done:
        line = ""
        for i in range(rowsize):
            data = f.read(4)
            if not data:
                done = True
                break
            line += "%.3f "%struct.unpack('f', data)[0]
        print(line)
            
            
            
    
