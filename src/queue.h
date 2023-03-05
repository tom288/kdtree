extern int WIP;
#include "kdtree.h"

// the "queue_block" class
// Stores a block of nodes, used in the "queue" class below.

typedef struct QueueBlock {
	Node** block; // actually means Node pointer array
	struct QueueBlock* prev; // towards the front of the queue, the front is where items pop off
	struct QueueBlock* next; // towards the back of the queue, the back is where items are pushed
} QueueBlock;

QueueBlock* queue_block();

// the "queue" class
// It's a queue of nodes implemented with a linked list,
// to improve efficiency the nodes are stored in blocks.

// items pushed onto the back, and they pop off at the front
typedef struct {
	QueueBlock* front;
	QueueBlock* back;
	unsigned short int front_index; // item returned last time pop was used
	unsigned short int back_index; // most recently pushed item
} Queue;

Queue* queue();

void queue_push(Queue* self, Node* item);
Node* queue_pop(Queue* self);
GLboolean queue_isEmpty(Queue* self);
GLboolean queue_isBug(Queue* self);
size_t queue_len_blocks(Queue* self);
size_t queue_len_items(Queue* self);
