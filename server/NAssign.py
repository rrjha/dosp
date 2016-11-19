

from util import dp

class NAssign:

  def __init__(self):
    self.taken = [0 for _ in range(256+1)]
    self.last = 255
    self.count = 0

  def poll(self):
    nstart = self.last
    nsearch = (nstart+1) % 256
    while nsearch != nstart:
      if self.taken[nsearch] == 0:
        break
      nsearch = (nsearch+1) % 256
    else:
      dp("No slots available to register client")
      return None
    assert not self.taken[nsearch]
    self.taken[nsearch] = 1
    self.count += 1
    self.last = nsearch
    return nsearch

  def kill(self, n):
    assert self.taken[n]
    self.taken[n] = 0
    self.count -= 1

assigner = NAssign()
