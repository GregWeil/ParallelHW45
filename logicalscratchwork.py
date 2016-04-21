matrix = [[9*k + i for i in range(9)] for k in range(9)]


transpose = list(map(list, zip(*matrix)))


msum = [[matrix[i][j] + transpose[i][j] for j in range(9)] for i in range(9)]

chunk_size = 3

#data to send
bmatdat = [[27, 28, 29, 30, 31, 32, 33, 34, 35],
           [36, 37, 38, 39, 40, 41, 42, 43, 44],
           [45, 46, 47, 48, 49, 50, 51, 52, 53]]





#data to recv
btrpdat = [[3, 12, 21, 30, 39, 48, 57, 66, 75],
           [4, 13, 22, 31, 40, 49, 58, 67, 76],
           [5, 14, 23, 32, 41, 50, 59, 68, 77]]


# A[0][3] + B[0][3] + C[0][3]
# A[1][3] + B[1][3] + C[1][3]
# A[2][3] + B[2][3] + C[2][3]


#data to send
cmatdat = [[54, 55, 56, 57, 58, 59, 60, 61, 62],
           [63, 64, 65, 66, 67, 68, 69, 70, 71],
           [72, 73, 74, 75, 76, 77, 78, 79, 80]]

#data to recv
ctrpdat = [[6, 15, 24, 33, 42, 51, 60, 69, 78],
           [7, 16, 25, 34, 43, 52, 61, 70, 79],
           [8, 17, 26, 35, 44, 53, 62, 71, 80]]


tocfromb = [[33, 34, 35],
            [42, 43, 44],
            [51, 52, 53]]

transpose_rows = [[0 for i in range(9)] for k in range(9)]

#for j in range(chunk_size):
j = 1
for i in range(chunk_size):
    for k in range(chunk_size):
        transpose_rows[i][j*chunk_size + k] = tocfromb[i][k];




