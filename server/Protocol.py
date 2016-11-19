
import asyncio

from util import dp

class Protocol(asyncio.DatagramProtocol):

  def __init__(self, loop):
    self.loop = loop
    self.transport = None
    self.write_lock = asyncio.Lock()
    self.buf = bytearray()

  def connection_made(self, transport):
    """Passes in the transport from asyncio, otherwise vestigial because UDP is connectionless"""
    self.transport = transport

  def datagram_received(self, data, addr):
    dp("Got some data from %s", addr)

  def pause_writing(self):
    loop.create_task(self.write_lock.acquire())

  def resume_writing(self):
    self.write_lock.release()
