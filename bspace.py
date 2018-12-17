import sys

n = int(sys.argv[1])

s = 1
if len(sys.argv) > 2:
    s = int(sys.argv[2])

print("bspace=", (4*17)+(17*( int((n+s-1)/s)  )*4)+n+(n*4))
