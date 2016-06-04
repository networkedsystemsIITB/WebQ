#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct node {
	int out;
	int in;
	int fail;
	struct node *next;
};

struct queue {
	struct node* root;
	struct node* current;
	int len;
	int limit;

	long long int tot_in;
	long long int tot_out;
	long long int tot_fail;
};

void insert(struct queue *q, int in, int out, int fail) {
	struct node *tmp;
	tmp = (struct node *) malloc(sizeof(struct node));
	if (tmp == NULL) {
		printf("Memory not available\n");
		return;
	}
	tmp->in = in;
	tmp->out = out;
	tmp->fail = fail;
	tmp->next = NULL;

	if (q->root == NULL) /*If Queue is empty*/
		q->root = tmp;
	else
		(q->current)->next = tmp;
	/*The above statement would next the the previous node to the newly created node*/
	q->current = tmp;
	q->len++;
	q->tot_in += in;
	q->tot_out += out;
	q->tot_fail += fail;
}

int isEmpty(struct queue *q) {
	if (q->root == NULL)
		return 1;
	else
		return 0;
}


int deleteq(struct queue *q)
{
	struct node *tmp;
	int in;
	int out;
	int fail;
	if( isEmpty(q) )
	{
		printf("Queue Underflow\n");
		//exit(1);
		return 0;
	}
	tmp = q->root;
	in = tmp->in;
	out = tmp->out;
	fail = tmp->fail;
	q->root = (q->root)->next;
	free(tmp);
	q->len--;
	q->tot_in -= in;  // Comment for considering infinite time
	q->tot_out -= out;// Comment for considering infinite time
	q->tot_fail -= fail;// Comment for considering infinite time
	return in;
}


int size(struct queue *q) {
	return q->len;
}

void display(struct queue *q) {
	struct node *ptr;
	ptr = q->root;
	if (isEmpty(q)) {
		printf("Queue is empty\n");
		return;
	}
	printf("Queue elements :\n\n");
	while (ptr != NULL) {
		printf("%d %d %d\n", ptr->in, ptr->out, ptr->fail);
		ptr = ptr->next;
	}
	printf("\n\n");
}

void set_limit(struct queue *q, int l) {
	q->limit = l;
}

void push_back(struct queue *q, int in, int out, int fail) {
	if (size(q) == q->limit) {
		deleteq(q);
		insert(q, in, out, fail);
	} else {
		insert(q, in, out, fail);
	}
}

long long int get_total_in(struct queue *q) {
	return q->tot_in;
}

long long int get_total_out(struct queue *q) {
	return q->tot_out;
}
long long int get_total_failing(struct queue *q) {
	return q->tot_fail;
}

long long int pending_requests(struct queue *q) {
	return ((q->tot_in) - (q->tot_out)) > 0 ? ((q->tot_in) - (q->tot_out)) : 0;
}

#endif
