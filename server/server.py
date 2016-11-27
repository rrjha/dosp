#DOS project scalable server

import asyncio

from Protocol import Protocol, db
from util import dp

try:
  import aiohttp
except ImportError:
  print("ERROR! You are missing aiohttp dependency. See README for installation instructions.")
  exit(1)

from rest import build_app

def main():
  dp("Initializing the event loop")
  loop = asyncio.get_event_loop()

  dp("Building datagram endpoint (UDP)")
  listen = loop.create_datagram_endpoint(
	lambda: Protocol(loop), local_addr=("0.0.0.0", 5154))
  transport, _ = loop.run_until_complete(listen)
  transport.set_write_buffer_limits(10000, 0)
  dp(transport)

  dp("Building REST API endpoint (TCP)")
  rest_app = build_app(db)
  rest_handler = rest_app.make_handler()
  server_f = loop.create_server(rest_handler, '0.0.0.0', 8080)
  rest_srv = loop.run_until_complete(server_f)
  dp(rest_handler)

  dp("Entering the event loop")
  try:
    loop.run_forever()
  except KeyboardInterrupt:
    pass
  finally:
    transport.close()
    rest_srv.close()
    loop.run_until_complete(rest_srv.wait_closed())
    loop.run_until_complete(rest_app.shutdown())
    loop.run_until_complete(rest_handler.finish_connections(3.0))
    loop.run_until_complete(rest_app.cleanup())
    loop.close()

if __name__=='__main__':
  print("I'm main!")
  main()
