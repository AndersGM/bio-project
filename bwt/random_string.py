import string
import random
import sys

def id_generator(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))

length = int(sys.argv[1])
alphabet = sys.argv[2]
output = sys.argv[3]

rand = id_generator(length, alphabet)

f = open(output, "w")
f.write(rand)
f.close()
with open(output, "a") as myfile:
    myfile.write("$")
