
import sqlite3
import binascii
import time

from NAssign import assigner
from Message import ftype, mtype

class Database:
  """System Database

  >>> import os
  >>> from Database import Database
  >>> if os.path.exists("sys.db"):
  ...   os.remove("sys.db")
  >>> db = Database()

  """

  tables = ("create table if not exists message(type text, class text, topic text, src integer, data text, time integer)",
	    "create table if not exists subscriber(id integer, topic text)",
	    "create table if not exists ipmap(ip text, id integer)",
	   )

  def __init__(self, db="./sys.db"):
    self.db = db
    self.conn = sqlite3.connect(db,8)
    self.c = self.conn.cursor()
    [self.c.execute(s) for s in Database.tables]
    self.conn.commit()
    self.cn = lambda g,x: str(g(x)).split('.')[1]

  def map_ip(self, addr):
    """Create mapping from assigned id to ip

    >>> db.map_ip("192.168.0.1")
    0
    >>> db.map_ip("192.168.0.2")
    1
    >>> db.map_ip("192.168.0.3")
    2

    """

    assert not self.map_exists(addr)
    n = assigner.poll()
    self.c.execute("insert into ipmap values(?, ?)", (addr, n))
    self.conn.commit()
    return n

  def dump_map(self):
    """Dump map table

    >>> db.dump_map()
    [('192.168.0.1', 0), ('192.168.0.2', 1), ('192.168.0.3', 2)]

    """
    self.c.execute("select * from ipmap order by id asc")
    return self.c.fetchall()

  def map_exists(self, addr):
    """Check that an address is mapped

    >>> db.map_exists("192.168.0.1")
    True
    >>> db.map_exists("192.168.0.2")
    True
    >>> db.map_exists("192.168.0.8")
    False

    """

    self.c.execute("select * from ipmap where ip=?", (addr,))
    if self.c.fetchone():
      return True
    return False

  def id_exists(self, n):
    """Check that an id exists and is valid

    >>> db.id_exists(1)
    True
    >>> db.id_exists(9)
    False

    """

    self.c.execute("select * from ipmap where id=?", (n,))
    if self.c.fetchone():
      return True
    return False

  def get_id(self, addr):
    """Retreive a local id mapping from the database

    >>> db.get_id("192.168.0.2")
    1
    >>> db.get_id("192.168.0.7")

    """

    self.c.execute("select id from ipmap where ip=?", (addr,))
    t = self.c.fetchone()
    if t:
      return t[0]
    return None

  def log_msg(self, msg_dict):
    msg_dict['type'] = self.cn(mtype, msg_dict['type'])
    msg_dict['class'] = self.cn(ftype, msg_dict['class'])
    msg_dict['topic'] = self.cn(ftype, msg_dict['topic'])
    self.c.execute("insert into message values(?,?,?,?,?,?,?)", (
	msg_dict['type'],
	msg_dict['class'],
	msg_dict['group'],
	msg_dict['topic'],
	msg_dict['src'],
	binascii.hexify(msg_dict['data']),
	int(time.time()),
    ))
    self.conn.commit()

  def dump_recent(self, id, count=10):
    """Dump recent 'count' messages from 'id'"""
    self.c.execute("select * from message where id=? order by time dec limit ?", (id, count))
    return self.c.fetchall()

  def subscribe(self, id, topic):
    """Subscribe id to a topic

    >>> db.subscribe(1, 0)
    >>> db.subscribe(1, 0)
    Traceback (most recent call last):
    AssertionError
    >>> db.subscribe(6, 0)
    Traceback (most recent call last):
    AssertionError

    """
    assert self.id_exists(id)
    assert not self.subscribed(id, topic)
    self.c.execute("insert into subscriber values(?, ?)", (id, self.cn(ftype, topic)))
    self.conn.commit()

  def get_subs(self, topic):
    """Assuming topic is an enumeration, get subscribers to a topic

    >>> db.get_subs(1)
    []
    >>> db.get_subs(0)
    [1]

    """
    self.c.execute("select id from subscriber where topic=?", (self.cn(ftype, topic),))
    return list(map(lambda x: x[0], self.c.fetchall()))

  def get_subs_ips(self, topic):
    """Assuming topic is an enumeration

    >>> db.get_subs_ips(0)
    ['192.168.0.2']
    >>> db.get_subs_ips(1)
    []
    >>> db.subscribe(0,0)
    >>> db.get_subs_ips(0)
    ['192.168.0.2', '192.168.0.1']

    """
    self.c.execute("select ip from subscriber inner join ipmap on subscriber.id=ipmap.id where topic=?", (self.cn(ftype, topic),))
    return list(map(lambda x: x[0], self.c.fetchall()))

  def subscribed(self, id, topic):
    """Test if an id is subscribed to a topic

    >>> db.subscribed(0,0)
    True
    >>> db.subscribed(2,0)
    False

    """
    self.c.execute("select * from subscriber where topic=? and id=?", (self.cn(ftype, topic), id))
    if self.c.fetchone():
      return True
    return False

  def drop_id(self, id):
    """Drop and id from the relevant tables

    >>> db.drop_id(0)

    """
    self.c.execute("delete from ipmap where id=?", (id,))
    self.c.execute("delete from subscriber where id=?", (id,))
    self.conn.commit()

  def wipe(self):
    """Clean up all the tables

    >>> db.wipe()

    """
    self.c.execute("delete from ipmap")
    self.c.execute("delete from subscriber")
    self.c.execute("delete from message")
    self.conn.commit()

  def __del__(self):
    """Don't rely on this"""
    self.conn.close()
