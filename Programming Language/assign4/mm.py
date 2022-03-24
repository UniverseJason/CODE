# python mm.py 4 8 -5 0 20
# prints: mm9: min=-5 max=20

import sys

def mm(args):
  x = 1000
  y = -1000
  x = min(int(x) for x in sys.argv[1:])
  y = max(int(y) for y in sys.argv[1:])
  return (x, y)


x,y = mm(sys.argv)
print("mm9: min=%d max=%d" % (x, y))