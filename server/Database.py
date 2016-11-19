
import sqlite3
import binascii

from NAssign import assigner
from Message import ftype, mtype

class Database:

  tables = ("create table if not exists message(type text, class text, group integer, topic text, src integer, data text)",
	    "create table if not exists subscriber(id integer, topic text)",
	    "create table if not exists ipmap(ip text, id integer)",
	   )

  def __init__(self, db="./sys.db"):
    self.db = db
    self.conn = sqlite3.connect(db,8)
    self.c = self.conn.cursor()
    self.c.execute("begin transaction")
    [self.c.execute(s) for s in Database.tables]
    self.c.execute("end transaction")
    self.cn = lambda g,x: str(g(x)).split('.')[1]

  def map_ip(self, addr):
    assert not self.map_exists(addr)
    n = assigner.poll()
    self.c.execute("insert into ipmap values(?, ?)", (addr, n))
    self.conn.commit()
    return n

  def map_exists(self, addr):
    self.c.execute("select * from ipmap where ip=?", (addr,))
    if self.c.fetchone():
      return True
    return False

  def get_id(self, addr):
    self.c.execute("select id from ipmap where ip=?", (addr,))
    return self.c.fetchone()[0]

  def log_msg(self, msg_dict):
    msg_dict['type'] = self.cn(mtype, msg_dict['type'])
    msg_dict['class'] = self.cn(ftype, msg_dict['class'])
    msg_dict['topic'] = self.cn(ftype, msg_dict['topic'])
    self.c.execute("insert into message values(?,?,?,?,?,?)", (
	msg_dict['type'],
	msg_dict['class'],
	msg_dict['group'],
	msg_dict['topic'],
	msg_dict['src'],
	binascii.hexify(msg_dict['data']),
    ))
    self.conn.commit()

  def get_subs(self, topic):
    """Assuming topic is an enumeration"""
    self.c.execute("select id from subscribers where topic=?", (self.cn(ftype, topic),))
    return list(map(lambda x: x[0], self.c.fetchall()))

  def get_subs_ips(self, topic):
    """Assuming topic is an enumeration"""
    self.c.execute("select ip from subscribers inner join ipmap on subscribers.id=ipmap.id where topic=?", (self.cn(ftype, topic),))
    return list(map(lambda x: x[0], self.c.fetchall()))

  def subscribed(self, id, topic):
    self.c.execute("select * from subscribers where topic=? and id=?", (self.cn(ftype, topic), id))
    if self.c.fetchone():
      return True
    return False

  def subscribe(self, id, topic):
    assert not self.subscribed(id, topic)
    self.c.execute("insert into subscribers values(?, ?)", (id, self.cn(ftype, topic)))
    self.conn.commit()

  def drop_id(self, id):
    self.c.execute("delete from ipmap where id=?", (id,))
    self.c.execute("delete from subscribers where id=?", (id,))
    self.conn.commit()

  def __del__(self):
    """Don't rely on this"""
    self.conn.close()
