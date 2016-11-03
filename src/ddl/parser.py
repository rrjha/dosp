#!/usr/bin/python3

#Parse, verify, and compile DDL into upper-level driver

import xml.etree.ElementTree
import argparse
import os

def get_args():
  parser = argparse.ArgumentParser(description='Parse DDL into HL driver interface')
  parser.add_argument('driver.ddl', help='DDL XML driver description file')
  parser.add_argument('out_path', help='Output path for .h and .c files', default=os.getcwd())
  return parser.parse_args()

def main():
  pass

if __name__=='__main__':
  print("I'm main!")
  main()
  print("Processing completed.")

