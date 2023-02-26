extern int WIP;
#include "kdtree.h"

const size_t queue_block_size = 10;

// ---

const size_t queue_block_endIndex = queue_block_size - 1; // queue_block_size - 1;

// the "queue_block" class
// Stores a block of nodes, used in the "queue" class below.

typedef struct QueueBlock {
	Node** block; // actually means Node pointer array
	struct QueueBlock* prev; // towards the front of the queue, the front is where items pop off
	struct QueueBlock* next; // towards the back of the queue, the back is where items are pushed
} QueueBlock;

QueueBlock* queue_block()
{
	QueueBlock* result = malloc(sizeof(QueueBlock));
	result->block = malloc(queue_block_size * sizeof(Node*));
	result->prev = NULL;
	result->next = NULL;
	return result;
}

// ---

// the "queue" class
// It's a queue of nodes implemented with a linked list,
// to improve efficiency the nodes are stored in blocks.

// items pushed onto the back, and they pop off at the front
typedef struct Queue
{
	QueueBlock* front;
	QueueBlock* back;
	unsigned short int front_index; // item returned last time pop was used
	unsigned short int back_index; // most recently pushed item
} Queue;

Queue* queue()
{
	Queue* result = malloc(sizeof(Queue));
	result->front = malloc(sizeof(QueueBlock));
	result->back = result->front;
	result->front_index = 0;
	result->back_index = 0;
	return result;
}

void queue_push(Queue* self, Node* item)
{
	self->back_index++;
	if (self->back_index > queue_block_endIndex) // if the queue is full
	{ // (if back_index overflows then change to "== 0")
		self->back->block[self->back_index] = item; // append the new item
		QueueBlock* new_block = queue_block(); // the queue gets a new block (another queue_block_size items)
		self->back->next = new_block; // link up the linked list correctly
		new_block->prev = self->back; // link up the linked list correctly
		self->back = new_block; // the back of the queue is now the new block
		self->back_index = 0; // start at the start of the new block
	}
}

Node* queue_pop(Queue* self)
{
	Node* item = self->front->block[self->front_index]; // the item to pop
	if (self->front->next == NULL && self->front_index == self->back_index) {
		return NULL; // queue is empty, same code as queue_isEmpty()
	}
	self->front_index++;
	if (self->front_index > queue_block_endIndex) // if the front block of the queue is empty
	{                                           // (if front_index overflows then change to "== 0")
		if (self->front->next == NULL) return NULL;
		QueueBlock* front_old = self->front;
		self->front = self->front->next; // the new front of the queue
		free(front_old);
		self->front->prev = NULL;
		self->back_index = 0; // start at the start of the block
	}
	return item;
}

GLboolean queue_isEmpty(Queue* self)
{
	return self->front->next == NULL && self->front_index == self->back_index;
}

GLboolean queue_isBug(Queue* self)
{
	// type/format is wrong
	if (self->front == NULL || self->back == NULL) {
		return GL_TRUE;
	}

	// inconsistent linking between blocks
	if ((self->front->next == NULL) != (self->back->prev == NULL)) {
		return GL_TRUE;
	}

	// object confused
	if (self->front->next == NULL && self->front_index > self->back_index) {
		return GL_TRUE; // start is after the end
	}

	return GL_FALSE;
}
