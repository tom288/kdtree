extern int WIP;
#include "kdtree.h"

// A block of nodes - a component of Queue
typedef struct QueueBlock {
	Node** block; // Array of pointers to nodes
	struct QueueBlock* prev; // towards the front of the queue, the front is where items pop off
	struct QueueBlock* next; // towards the back of the queue, the back is where items are pushed
} QueueBlock;

// Allocate for a new block and again for its nodes, and return that block
QueueBlock* queue_block();

// Free memory allocated for a queue block
void queue_block_free(QueueBlock* block);

// The "queue" class
// It's a queue of nodes implemented with a linked list,
// to improve efficiency the nodes are stored in blocks.

// items pushed onto the back, and they pop off at the front
typedef struct {
	QueueBlock* front;
	QueueBlock* back;
	unsigned short int front_node_index; // item returned last time pop was used
	unsigned short int back_node_index; // most recently pushed item
} Queue;

Queue* queue();

void queue_push(Queue* self, Node* item);
Node* queue_pop(Queue* self);
GLboolean queue_isEmpty(Queue* self);
GLboolean queue_isBug(Queue* self);
size_t queue_len_blocks(Queue* self);
size_t queue_len_items(Queue* self);
