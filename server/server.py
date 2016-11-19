#DOS project scalable server

import asyncio

from Protocol import Protocol
from util import dp

def main():
  dp("Initializing the event loop")
  loop = asyncio.get_event_loop()

  dp("Building datagram endpoint (UDP)")
  listen = loop.create_datagram_endpoint(
	lambda: Protocol(loop), local_addr=("0.0.0.0", 5154))
  transport, _ = loop.run_until_complete(listen)
  transport.set_write_buffer_limits(10000, 0)
  dp(transport)

  dp("Entering the event loop")
  try:
    loop.run_forever()
  finally:
    transport.close()
    loop.close()

if __name__=='__main__':
  print("I'm main!")
  main()
