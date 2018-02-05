#!/usr/bin/env python3

import os, sys

import tptester


tests_dir = os.path.dirname(os.path.realpath(__file__))


tptester.tp_test(
  #[ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 ],
  [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ],
  os.getcwd() + '/kontest',
  
  valgrind = True,
  
  #graph = True,
  
  input_file  = lambda ix: tests_dir + '/data/toys2/in_' + str(ix) + '.in',
  answer_file = lambda ix: tests_dir + '/data/toys2/in_' + str(ix) + '.out',
  
  args = lambda ix: [ ],
  stdin = lambda ifile: ifile,
  output = lambda ofile, stdout: stdout
)
