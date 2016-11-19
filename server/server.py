#DOS project scalable server

import asyncio

from ServerProtocol import ServerProtocol
from util import dp

def main():
  dp("Initializing the event loop")
  loop = asyncio.get_event_loop()

  dp("Building datagram endpoint (UDP)")
  listen = loop.create_datagram_endpoint(
	lambda: ServerProtocol(loop), local_addr=("0.0.0.0", 5154))
  transport, protocol = loop.run_until_complete(listen)
  dp(transport)
  dp(protocol)

  dp("Entering the event loop")
  try:
    loop.run_forever()
  finally:
    transport.close()
    loop.close()

if __name__=='__main__':
  print("I'm main!")
  main()
