void swap(int x, int y)
{
	x = x ^ y;
	y = y ^ x;
	x = x ^ y;
}

void swap2(int x, int y)
{
	x ^= y;
	y ^= x;
	x ^= y;
}
