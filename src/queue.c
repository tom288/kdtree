extern int WIP;
#include "queue.h"
#include "kdtree.h"

const size_t queue_block_size = 10;

// ---

const size_t queue_block_endIndex = queue_block_size - 1; // queue_block_size - 1;

// the "queue_block" class
// Stores a block of nodes, used in the "queue" class below.

QueueBlock* queue_block()
{
	QueueBlock* result = malloc(sizeof(QueueBlock));
	// This implicitly assigns 0 (NULL) to members which are not mentioned
	*result = (QueueBlock) {
		.block = malloc(queue_block_size * sizeof(Node*)),
	};
	return result;
}

void queue_block_free(QueueBlock* block)
{
	free(block->block);
	block->block = NULL;
	free(block);
}

// ---

// the "queue" class
// It's a queue of nodes implemented with a linked list,
// to improve efficiency the nodes are stored in blocks.

// items pushed onto the back, and they pop off at the front

Queue* queue()
{
	Queue* result = malloc(sizeof(Queue));
	result->front = malloc(sizeof(QueueBlock));
	result->front = queue_block();
	result->back = result->front;
	result->front_node_index = 0;
	result->back_node_index = 0;
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
	{                                             // (if front_index overflows then change to "== 0")
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

size_t queue_len_blocks(Queue* self)
{
	int len = 0;
	QueueBlock* block = self->front;
	while (block != NULL) {
		block = block->next;
		len++;
	}
	return len;
}

size_t queue_len_items(Queue* self)
{
	size_t len = queue_len_blocks(self);
	if (len == 0) return 0;
	else if (len == 1) len = 0;
	else len -= 2; // discount the end blocks
	len *= queue_block_size;
	return len + self->back_index - self->front_index;
}
