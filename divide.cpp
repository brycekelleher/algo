#include <stdlib.h>
#include <stdio.h>

static int array[] = { 5, 9, 6, 7 };

int sum(int *array, int l, int r)
{
	if(l == r)
		return array[l];
	int mid = (l + r) / 2;
	return sum(array, l, mid) + sum(array, mid + 1, r);
}

int max(int *array, int l, int r)
{
	if(l == r)
		return array[l];
	int mid = (l + r) / 2;
	int a = max(array, l, mid);
	int b = max(array, mid + 1, r);
	return (a > b ? a : b);
}

int main(int argc, char *argv[])
{
	int l = atoi(argv[1]);
	int r = atoi(argv[2]);

	printf("max %i\n", max(array, l, r));
	printf("sum %i\n", sum(array, l, r));
}
