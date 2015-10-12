#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool bsearch(const char s, const char *a, int l, int h)
{
	if (l == h)
		return (a[l] == s);

	int m = (l + h) / 2;
	if (s <= a[m])
		return bsearch(s, a, l, m);
	else
		return bsearch(s, a, m + 1, h);
}

int main(int argc, char *argv[])
{
	char s = argv[1][0];
	const char a[] = "abcdhilmoprtuz";
	printf("search %c: %s\n", s, (bsearch(s, a, 0, strlen(a) + 1) ? "y" : "n"));

	return 0;
}
