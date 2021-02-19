
if me == 0
  {
    send(me, est);
    recv(me, ouest);
  }
else
  {
    recv(me, ouest);
    me++;
    send(me, est);
  }
