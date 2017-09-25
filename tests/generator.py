#!/usr/bin/env python3

import random
import sys


if len(sys.argv) != 2:
  print('Missing argument', file = sys.stderr)
  sys.exit(1)


size = int(sys.argv[1])

matrix = [ [ 0.0 for i in range(size) ] for j in range(size) ]

for i in range(size):
  for j in range(i + 1, size):
    matrix[i][j] = random.random()
    matrix[j][i] = 1 - matrix[i][j]

print(size)

for i in range(size):
  for j in range(size):
    print("{0:.5f} ".format(matrix[i][j]), end='')
  print()
