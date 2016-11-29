
__doc__ = """

Example file demonstrates how to use the REST API of the server by requesting data on each connected client

"""

#Import requests library for making API calls
try:
  import requests
except ImportError:
  raise SystemExit("Error! You are missing the 'requests' library. Please install using the pip3 utility.")

#Other imports
from enum import Enum

#Define API call types
class RequestType(Enum):
  ECHO = 0
  LIST = 1
  RECENT = 2

#Configuration
server = "http://127.0.0.1:8080/"

def led_example():
  r = requests.post(server + "json", data='{"type": 3, "target": "192.168.0.1"}'.encode('utf-8'))
  #LED turned on!

def main():
  """Main demonstration function"""
  #Check that the server is running
  r = requests.get(server)
  assert r.text == "Hello World!"
  print("Server connectivity check succeeded")

  #Request a list of clients from the server
  r = requests.post(server + "json", data='{"type": 1}'.encode('utf-8'))
  print("Connected Devices:\n" + r.text)

  #Print last seven messages for each device
  d = r.json()
  messages_by_id = {}
  for addr, id in d.items():
    print("Messages for client %d at %s:" % (id, addr))
    r = requests.post(server + "json", data=('{"type": 2, "id": %d, "count": 7}' % id).encode('utf-8'))
    msgs = r.json()
    messages_by_id[id] = msgs
    print(msgs)

  #Extract messages numerically for each id
  temperature_lists = {(id, []) for addr, id in d.items()}
  accel_lists = {(id, []) for addr, id in d.items()}
  sleep_lists = {(id, []) for addr, id in d.items()}
  id_list = [id for addr, id in d.items()]
  for id in id_list:
    for msg in messages_by_id[id]:
      data = bytearray.fromhex(msg['data'])
      c = msg['class']
      if c == "TEMP":
        temperature_lists[id].append(data[0])
      elif c == "ACCEL":
        accel_lists[id].append((data[0], data[1], data[2]))
      elif c == "SLEEP":
        sleep_lists[id].append(data[0])
      else:
        print("Unknown message class %s for id %d" % (c, id))

  #Now, lists are dictionaries by id containing a list of numeric values to plot

#Detect if script was run directly and if so, call main
if __name__=='__main__':
  print("I'm main!")
  main()

