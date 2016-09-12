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


node_t *bst_node_alloc(node_t *parent, unsigned int value)
{
	node_t *n = (node_t*)calloc(1, sizeof(node_t));
	n->parent = parent;
	n->value = value;

	return n;
}

node_t *bst_node_copy(node_t *s)
{
	node_t *d = (node_t*)bst_node_alloc(NULL, s->value);
	d->children[0] = s->children[0];
	d->children[1] = s->children[1];
	d->parent = s->parent;
	d->value = s->value;

	return d;
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

node_t *bst_copy_path(node_t *n)
{
	if (!n->parent)
		return bst_node_copy(n);

	node_t *p = bst_copy_path(n->parent);

	node_t *d = bst_node_copy(n);
	d->parent = p;
	if (p->children[0] == n)
		p->children[0] = d;
	else
		p->children[1] = d;

	return d;
}

node_t *bst_follow_to_root(node_t *n)
{
	while (n->parent)
		n = n->parent;
	return n;
}

node_t *bst_find_insert_node(node_t *n, unsigned int value)
{
	if (value < n->value)
	{
	       if (!n->children[0])
			return n;
		else
			return bst_find_insert_node(n->children[0], value);
	}
	else
	{
	       if (!n->children[1])
			return n;
		else
			return bst_find_insert_node(n->children[1], value);
	}
}

node_t *bst_insert(node_t *root, unsigned int value)
{
	// find the insert node and copy the path
	node_t *n = bst_find_insert_node(root, value);
	n = bst_copy_path(n);

	if (value < n->value)
		n->children[0] = bst_node_alloc(n, value);
	else 
		n->children[1] = bst_node_alloc(n, value);

	// return the new root
	return bst_follow_to_root(n);
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

node_t *bst_copy(node_t *s)
{
	if (!s)
		return NULL;
	
	node_t *d = bst_node_copy(s);
	d->children[0] = bst_copy(s->children[0]);
	d->children[1] = bst_copy(s->children[1]);
	return d;
}

void read_file(const char *filename)
{
	unsigned int value;

	FILE *fp = open_file(filename);

	fscanf(fp, "%x", &value);
	root = bst_node_alloc(NULL, value);

	while (fscanf(fp, "%x", &value) != EOF)
	{
		printf("insert %#x\n", value);
		root = bst_insert(root, value);
		bst_inorder_walk(root);
	}
}

int main(int argc, char *argv[])
{
	read_file(argv[1]);

	bst_inorder_walk(root);

	return 0;
}

