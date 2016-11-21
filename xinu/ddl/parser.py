#!/usr/bin/python3

#Parse, verify, and compile DDL into upper-level driver

import xml.etree.ElementTree as ET
import argparse
import os

from validator import validate
from generator import generate

def get_args():
  """Parse command line arguments to the utility"""
  parser = argparse.ArgumentParser(prog='DDL XML Preprocessor', description='Parse DDL into HL driver interface')
  parser.add_argument('driver_ddl', help='DDL XML driver description file', type=str)
  local_path = os.path.dirname(os.path.realpath(__file__))
  parser.add_argument('out_path', help='Output path for .h and .c files', default=local_path, type=str, nargs='?')
  return vars(parser.parse_args())

def main():
  args = get_args()
  print("DDL: %s" % args['driver_ddl'])
  print("Output Path: %s" % args['out_path']) 
  tree = ET.parse(args['driver_ddl'])
  validate(tree)
  generate(tree)

if __name__=='__main__':
  print("I'm main!")
  main()
  print("Processing completed.")

