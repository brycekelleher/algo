#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char a[], int i, int j)
{
	char temp;
	temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

/*
   The loop invariant here is that the set is divided into two pieces, a suffix and a prefix
   The prefix is sorted, the suffix unsorted. The prefix intially starts as empty, hence i = 0
 */
void selection(char a[], int n)
{
	for(int i = 0; i < n; i++)
	{
		// find the minimal element
		int min = i;
		for(int j = i; j < n; j++)
			if(a[j] < a[min])
				min = j;

		swap(a, i, min);
	}
}

/*
   Find the location in the prefix where the first element of the suffix should be placed
   the prefix starts out empty, an each loop iteration the size of the suffix (and the problem)
   is reduced by one
   Note that this doesn't use swap in the inner loop. If swap was used we can get rid of the
   'value' as the inner loop destructively overwrites a[j] so we need to save it
   It has an advantage over selection sort in that it doesn't need to scan all elements in the 
   suffix to find the absolute minimum element in the suffix
 */
void insertion(char a[], int n)
{
	for(int i = 0; i < n; i++)
	{
		char value = a[i];
		
		// walk backwards from i copying large elements up to make space
		int j = i;
		while(j > 0 && a[j - 1] > value)
		{
			a[j] = a[j - 1];
			j--;
		}

		// j is the index that i should be inserted into
		a[j] = value;
	}
}

/*
   The larger values are bubbled to the top, which means the suffix is sorted and the prefix is not
   Though there really isn't a suffix in the loop below. In theory the size of the inner loop could
   be reduced by 1 after each iteration of i as the next largest value is guarenteed to be in the
   correct place
*/
void bubble(char a[], int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n - 1; j++)
			if (a[j] > a[j + 1])
				swap(a, j , j + 1);
}

bool string_in_set(const char *s, const char *set[])
{
	for (; *set; set++)
		if (!strcmp(s, *set))
			return true;

	return false;
}

static void driver(int argc, char *argv[])
{
	const char *set[] = { "sel", "ins", "bub", NULL };
	string_in_set("bub", set);
	string_in_set("fail", set);

	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			FILE *fp = fopen(argv[i], "r");
			
			char buffer[512];
			while(fgets(buffer, 512, fp))
			{
				// strip the newline
				buffer[strlen(buffer) - 1] = 0;
				printf("read: \"%s\" ", buffer);

				selection(buffer, strlen(buffer));
				printf("result: \"%s\"\n", buffer);
			}
		}
	}
	else
	{
		char string[] = "selection sorting example";

		//selection(string, strlen(string));
		insertion(string, strlen(string));
		printf("result: \"%s\"\n", string);
	}
}

int main(int argc, char *argv[])
{
	driver(argc, argv);

	return 0;
}
