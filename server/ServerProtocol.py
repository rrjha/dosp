
import asyncio

from util import dp

class ServerProtocol(asyncio.DatagramProtocol):

  def __init__(self, loop):
    self.loop = loop
    self.transport = None
    self.write_lock = asyncio.Lock()

  def connection_made(self, transport):
    self.transport = transport

  def datagram_received(self, data, addr):
    dp("Got some data from %s", addr)

  def pause_writing(self):
    loop.call_soon(self.write_lock.acquire)

  def resume_writing(self):
    self.write_lock.release()
