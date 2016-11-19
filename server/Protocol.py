
import asyncio

from util import dp
from Message import msg_fmt, unpack_msg

class Protocol(asyncio.DatagramProtocol):

  def __init__(self, loop):
    self.loop = loop
    self.transport = None
    self.write_lock = asyncio.Lock()
    self.bufs = {}

  def connection_made(self, transport):
    """Passes in the transport from asyncio, otherwise vestigial because UDP is connectionless"""
    self.transport = transport

  def datagram_received(self, data, addr):
    dp("Got some data from %s", addr)
    if not addr in self.bufs:
      self.bufs[addr] = bytearray()
    b = self.bufs[addr]
    b += data
    s_size = msg_fmt.size
    if len(b) >= s_size:
      p = b[0:s_size]
      self.bufs[addr] = b[s_size:]
      self.handle(unpack_msg(data))

  def pause_writing(self):
    loop.create_task(self.write_lock.acquire())

  def resume_writing(self):
    self.write_lock.release()

  def handle(self, data):
    pass #TODO implement handle message using database

