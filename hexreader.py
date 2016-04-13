import sys
import struct

with open("output.txt", "rb") as f:
    data = f.read()
    rowsize = int(sys.argv[1])
    for x in range(rowsize):
        line = ""
        for y in range(0,rowsize*4,4):
            ix = x*rowsize*4+y
            line += "%.3f "%struct.unpack_from('f', data, ix)[0]
        print(line)
            
    
