
import doctest

files = ["Database.py", "Message.py", "rest.py"]

def main():
  flags = doctest.ELLIPSIS | doctest.IGNORE_EXCEPTION_DETAIL | doctest.FAIL_FAST
  failure_count = test_count = 0
  for file in files:
    fc, tc = doctest.testfile(file, verbose=True, optionflags=flags)
    failure_count += fc
    test_count += tc
  print("{} tests, {} failures".format(test_count, failure_count))

if __name__=='__main__':
  print("I'm main!")
  main()
