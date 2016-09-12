import random

def perm(size):
	values = range(0, size);

	for x in range(0, size):
		swap = random.randrange(0, size)
		temp = values[x]
		values[x] = values[swap]
		values[swap] = temp

	for x in range(0, size):
		if not (x % 16):
			print "\n",
		print ("%3i," % values[x]),

def main():
	perm(256)
	
main()	
