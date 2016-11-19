
from enum import Enum
from struct import Struct
from util import dp

msg_fmt = Struct(">69B")
fields = ('type', 'class', 'group', 'topic', 'src', 'data')

def unpack_msg(data):
  if len(data) != msg_fmt.size:
    dp("Data size does not match size of struct")
  msg_dict = {}
  i = 0
  for f in fields:
    if f != 'data':
      msg_dict[f] = data[i]
      i += 1
  msg_dict['data'] = data[i:]
  msg_dict['type'] = mtype(msg_dict['type'])
  msg_dict['class'] = ftype(msg_dict['class'])
  return msg_dict

def pack_msg(msg_dict):
  for item in fields:
    assert item in msg_dict, "Missing field"
  return msg_fmt.pack(
    *[msg_dict[k] for k in fields])

class ftype(Enum):
  """Format type is the same as the topic in this project"""
  TEMP = 0
  ACCEL = 1
  SLEEP = 2
  LED = 3

class mtype(Enum):
  """Message type field"""
  PUBLISH = 0
  SUBSCRIBE = 1
  ENTER = 2
  LEAVE = 3


