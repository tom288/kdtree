extern int WIP;

/*
#include "kdtree.h"

// que_block_size_type = unsigned short int
const size_t que_block_size = 10; // 1000 // 8 KB

// ---

const size_t que_block_endIndex = que_block_size - 1;

// the "que_block" class
// Stores a block of nodes, used in the "que" class below.

typedef struct QueueBlock {
	Node* block;
	QueueBlock* prev; // towards the front of the que, the front is where items pop off
	QueueBlock* next; // towards the back of the que, the back is where items are pushed
} QueueBlock;

QueueBlock* que_block()
{
	QueueBlock* result = malloc(sizeof(QueueBlock));
	result->block = malloc(que_block_size * sizeof(Node*));
	result->prev = NULL;
	result->next = NULL;
	return result;
}

// the "que" class
// It's a que of nodes implemented with a linked list,
// to improve efficiency the nodes are stored in blocks.

void que_push(Queue* self, Node* item)
{
	self->back_index++;
	if (self->back_index > que_block_endIndex) // if the que is full
	{ // (if back_index overflows then change to "== 0")
		self->back[self->back_index] = item; // append the new item
		QueueBlock* new_block = que_block(); // the que gets a new block (another que_block_size items)
		self->back->next = new_block; // link up the linked list correctly
		new_block->prev = self->back; // link up the linked list correctly
		self->back = new_block; // the back of the que is now the new block
		self->back_index = 0; // start at the start of the new block
	}
}

Node* que_pop(Queue* self)
{
	Node* item = self->front[self->front_index]; // the item to pop
	if (self->front->next == NULL && self->front_index == self->back_index)
	{
		return NULL; // que is empty, same code as que_isEmpty()
	}
	self->front_index++;
	if (self->front_index > que_block_endIndex) // if the front block of the que is empty
	{ // (if front_index overflows then change to "== 0")
		if (self->front->next == NULL)
		{
			return NULL;
		}

		QueueBlock* front_old = self->front;
		self->front = self->front->next; // the new front of the que
		free(front_old);
		self->front->prev = NULL;
		self->back_index = 0; // start at the start of the block
	}
	return item;
}

GLboolean que_isEmpty(Queue* self)
{
	return self->front->next == NULL && self->front_index == self->back_index;
}

GLboolean que_isBug(Queue* self)
{
	// type/format is wrong
	if (
		self->front == NULL || self->back == NULL || self->push == NULL ||
		self->pop == NULL || self->isBug == NULL || self->isEmpty == NULL
	) {
		return GL_TRUE;
	}

	// inconsistent linking between blocks
	if ((self->front->next == NULL) != (self->back->prev == NULL))
	{
		return GL_TRUE;
	}

	// object confused
	if (self->front->next == NULL && self->front_index > self->back_index)
	{
		// start is after the end
		return GL_TRUE;
	}

	return GL_FALSE;
}

// items pushed onto the back, and they pop off at the front
typedef struct Queue
{
	QueueBlock* front;
	QueueBlock* back;
	unsigned short int front_index; // item returned last time pop was used
	unsigned short int back_index; // most recently pushed item
	func* push;
	func* pop;
	func* isBug;

	// extra stuff
	func* isEmpty;
} Queue;

Queue* que()
{
	Queue* result = malloc(sizeof(Queue));
	result->front = malloc(sizeof(QueueBlock));
	result->back = result->front;
	result->front_index = 0;
	result->back_index = 0;
	result->push = &que_push;
	result->pop = &que_pop;
	result->isEmpty = &que_isEmpty;
	return result;
}
*/
