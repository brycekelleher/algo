typedef struct node_s
{
	struct node_s	*children[2];
	struct node_s	*parent;
	unsigned int	value;
} node_t;

static node_t *root;

#include <stdio.h>
#include <stdlib.h>

FILE *open_file(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to open \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	return fp;
}

node_t *bst_alloc_node(node_t *parent, unsigned int value)
{
	node_t *n = (node_t*)calloc(1, sizeof(node_t));
	n->parent = parent;
	n->value = value;

	return n;
}

node_t *bst_search(node_t *n, unsigned int value)
{
	if (!n)
		return NULL;

	if (value < n->value)
		bst_search(n->children[0], value);
	if (value > n->value)
		bst_search(n->children[1], value);

	return n;
}

void bst_insert(node_t *n, unsigned int value)
{
	if (value < n->value)
	{
	       if (!n->children[0])
			n->children[0] = bst_alloc_node(n, value);
		else
			bst_insert(n->children[0], value);
	}
	else
	{
	       if (!n->children[1])
			n->children[1] = bst_alloc_node(n, value);
		else
			bst_insert(n->children[1], value);
	}
}

void bst_inorder_walk(node_t *n)
{
	if (!n)
		return;

	bst_inorder_walk(n->children[0]);
	printf("%#x\n", n->value);
	bst_inorder_walk(n->children[1]);
}

node_t *bst_find_minimum(node_t *n)
{
	while(n->children[0])
		n = n->children[0];

	return n;
}

#if 0
void bst_successor(node_t *n)
{
	if (n->children[1])
		return bst_find_minimum(n->children[1]);
	else
		return bst_find_minimum(n->parent->children[1]);
		// go up one level and run bst_find_minimum on that subtree
		// what if the parent doesn't have a right subtree?
}
#endif

void read_file(const char *filename)
{
	unsigned int value;

	FILE *fp = open_file(filename);

	fscanf(fp, "%x", &value);
	root = bst_alloc_node(NULL, value);

	while (fscanf(fp, "%x", &value) != EOF)
		bst_insert(root, value);
}

int main(int argc, char *argv[])
{
	read_file(argv[1]);

	bst_inorder_walk(root);

	return 0;
}

