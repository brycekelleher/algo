#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>

// settings taken from
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
uint32_t hash_fnv1a(void *data, int numbytes)
{
	static const uint32_t FNV_PRIME = 16777619;
	static const uint32_t FNV_OFFSET_BASIS = 2166136261;

	uint8_t	*bytes = (uint8_t*)data;
	uint32_t hash = FNV_OFFSET_BASIS;

	for (int32_t i = 0; i < numbytes; i++)
	{
		hash ^= bytes[i];
		hash *= FNV_PRIME;
	}

	return hash;
}

// _______________________________________________________
// atoms 

typedef struct atom_s
{
	struct atom_s	*next;
	int		numbytes;
	void		*data;

} atom_t;

static atom_t *new_atom(void *data, uint32_t numbytes)
{
	atom_t *a = (atom_t*)malloc(sizeof(atom_t) + numbytes);

	a->numbytes	= numbytes;
	a->data		= (void*)(a + 1);
	memcpy(a->data, data, numbytes);
	
	return a;
}

void *atom_lookup(void* data, int32_t numbytes)
{
	static atom_t *table[31];

	uint32_t hash = hash_fnv1a(data, numbytes) % 31;

	// search for the item
	atom_t *a = table[hash];
	for (; a; a = a->next)
		if ((numbytes == a->numbytes) && !memcmp(data, a->data, numbytes))
			break;

	// if it doesn't create it
	if (!a)
	{
		a = new_atom(data, numbytes);

		a->next = table[hash];
		table[hash] = a;
	}

	return a->data;
}

// _______________________________________________________
// markov and hash intermixed...

// this is effectively a line in the transition table
typedef struct next_state_s
{
	struct next_state_s	*next;
	int count;
	int random;
	char c;

} next_state_t;

typedef struct state_s
{
	struct state_s		*globalnext;
	struct state_s		*next;

	// the statekey
	void			*statekey;

	// next state
	struct next_state_s	*next_states;

	int count;

} state_t;

static state_t *statelist = NULL;
static state_t *stable[31] = { 0 };

// this shouldn't really need to know about states...
state_t *hash_lookup(void *key)
{
	uint32_t hash = (uint32_t)key % 31;

	state_t *s = stable[hash];
	for (; s; s = s->next)
		if (s->statekey == key)
			break;

	return s;
}

state_t *markov_newstate(void *statekey)
{
	state_t *s = (state_t*)malloc(sizeof(state_t));

	s->globalnext = NULL;
	s->next = NULL;
	s->statekey = statekey;
	s->next_states = NULL;

	// link it into the global list
	s->globalnext = statelist;
	statelist = s;

	return s;
}

state_t *markov_state_lookup(char s0, char s1)
{
	// get the state key from the current state
	uint8_t data[2] = { s0, s1 };
	void *statekey = atom_lookup(data, 2);

	// get the state from the hashtable
	state_t *s = hash_lookup(statekey);

	if (!s)
	{
		s = markov_newstate(statekey);

		// link it into the table
		uint32_t hash = (uint32_t)statekey % 31;
		s->next = stable[hash];
		stable[hash] = s;
	}

	return s;
}

// lookup the state, update the transition table
void markov_add(char s0, char s1, char n)
{
	state_t *state = markov_state_lookup(s0, s1);

	// update the counts
	for (next_state_t *ns = state->next_states; ns; ns = ns->next)
	{
		if (ns->c == n)
		{
			ns->count++;
			return;
		}
	}

	// add a new transition
	next_state_t *ns = (next_state_t*)malloc(sizeof(next_state_t));
	ns->count = 1;
	ns->c = n;

	// link it into the next state linked list
	ns->next = state->next_states;
	state->next_states = ns;
}

void markov_build()
{
	for (state_t *s = statelist; s; s = s->globalnext)
	{
		s->count = 0;
		for (next_state_t *ns = s->next_states; ns; ns = ns->next)
		{
			s->count += ns->count;
			ns->random = s->count;
		}
	}
}

char markov_random_state(char s0, char s1)
{
	state_t *s = markov_state_lookup(s0, s1);

	int r = rand() % s->count;

	for (next_state_t *ns = s->next_states; ns; ns = ns->next)
		if (r < ns->random)
			return ns->c;

	printf("failure...\n");
	exit(0);
	return 0;
}

void markov_generate()
{
	char s0, s1, n;

	s0 = 'f'; s1 = 'i';
	printf("fi");
	for (int i = 0; i < 2000; i++)
	{
		char n = markov_random_state(s0, s1);
		printf("%c", n);
		s0 = s1;
		s1 = n;
	}

	printf("\n");
}
		


static char ch(char c)
{
	return (isprint(c) ? c : '*');
}

// ___________________________________________
//driver
int main(int argc, char *argv[])
{
	char s0, s1, c;

	FILE *fp;
	if (argc == 1)
		fp = stdin;
	else
		fp = fopen(argv[1], "rb");

	s0 = fgetc(fp);
	s1 = fgetc(fp);
	while ((c = fgetc(fp)) != EOF)
	{
		markov_add(s0, s1, c);
		s0 = s1;
		s1 = c;
	}

	// build the probabilities
	markov_build();

	markov_generate();

#if 0	
	for (state_t *s = statelist; s; s = s->globalnext)
	{
		char *data = (char*)s->statekey;
		printf("%c%c: ", ch(data[0]), ch(data[1]));

		for (next_state_t *ns = s->next_states; ns; ns = ns->next)
			printf("(%c %d)", ch(ns->c), ns->count);

		printf("\n");
	}
#endif
}


