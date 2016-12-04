
import asyncio
import traceback

from util import dp
from Message import msg_fmt, unpack_msg, pack_msg, mtype
from Database import Database

db = Database()

class Protocol(asyncio.DatagramProtocol):

  def __init__(self, loop):
    self.loop = loop
    self.transport = None
    self.write_lock = asyncio.Lock()
    self.bufs = {}

  def connection_made(self, transport):
    """Passes in the transport from asyncio, otherwise vestigial because UDP is connectionless"""
    global d_transport
    self.transport = transport
    d_transport = transport

  def datagram_received(self, data, addr):
    addr = addr[0]
    dp("Got some data from %s" % addr)
    if not addr in self.bufs:
      self.bufs[addr] = bytearray()
    b = self.bufs[addr]
    b += data
    s_size = msg_fmt.size
    if len(b) >= s_size:
      p = b[0:s_size]
      self.bufs[addr] = b[s_size:]
      self.loop.create_task(self.handle(unpack_msg(p), addr))

  def pause_writing(self):
    self.loop.create_task(self.write_lock.acquire())

  def resume_writing(self):
    self.write_lock.release()

  async def handle(self, msg, addr):
    m_type = msg["type"]
    dp(msg)
    try:
      if m_type == mtype.ENTER:
        dp("Got ENTER from " + addr)
        db.map_ip(addr)
      elif m_type == mtype.LEAVE:
        dp("Got LEAVE from " + addr)
        db.drop_id(db.get_id(addr))
      elif m_type == mtype.PUBLISH:
        dp("Got PUBLISH from " + addr)
        ips = db.get_subs_ips(msg["topic"])
        msg["src"] = db.get_id(addr)
        db.log_msg(msg)
        await self.write_lock.acquire()
        for ip in ips:
          self.transport.send(pack_msg(msg), ip)
          dp("Forwarded message to " + addr)
        self.write_lock.release()
      elif m_type == mtype.SUBSCRIBE:
        dp("Got SUBSCRIBE from " + addr)
        db.subscribe(db.get_id(addr), msg["topic"])
      else:
        dp("Unknown message type")
    except Exception as e:
      dp("Caught exception while attempting to handle message from " + addr)
      traceback.print_exc()


