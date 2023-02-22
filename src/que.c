int n = 0;

/*
// que_block_size_type = unsigned short int
unsigned short int que_block_size = 10 // 1000 // 8 KB

// ---

unsigned short int que_block_endIndex = que_block_size - 1

# the "que_block" class
# Stores a block of nodes, used in the "que" class below.

typedef struct que_block_struct
{
	Node* block[que_block_size];
	que_block* prev; // towards the front of the que, the front is where items pop off
	que_block* next; // towards the back of the que, the back is where items are pushed
} que_block;

que_block_struct* que_block()
{
	que_struct result;
	result.block = malloc(sizeof(Node*) * que_block_size);
	result.prev = malloc(sizeof(que_block_struct*));
	result.next = malloc(sizeof(que_block_struct*));
	return result;
}

# ---

# the "que" class
# It's a que of nodes implemented with a linked list,
# to improve efficency the nodes are stored in blocks.

void que_push(que_struct* self, Node* item)
{
	(&self).back_index++;
	if ((&self).back_index > que_block_endIndex) // if the que is full
												 // (if back_index overflows then change to "== 0")
	{
		(&self).back[back_index] = item; // append the new item
		que_block* new_block = *que_block(); // the que gets a new block (another que_block_size items)
		(&((&self).back)).next = new_block; // link up the linked list correctly
		new_block.prev = (&self).back; // link up the linked list correctly
		&((&self).back) = new_block; // the back of the que is now the new block
		(&self).back_index = 0; // start at the start of the new block
	}
}

Node* que_pop(que_struct* self, que_block* self)
{
	item = (&self.front)[front_index]; // the item to pop
	if (
		((&((&self).front)).next == null) &&
		((&self).front_index == back_index)
		) {return null} // que is empty, same code as que_isEmpty()
	(&self).front_index++;
	if ((&self).front_index > que_block_endIndex) // if the front block of the que is empty
	                                              // (if front_index overflows then change to "== 0")
	{
		if (&((&self).front)).next == null) {return null}
		else {
			que_block* front_old = (&self).front
			(&self).front = &((&self).front).next; // the new front of the que
			del front_old; // is it (del &front_old)?
			&(self).front.prev = null;
			(&self).back_index = 0; // start at the start of the block
		}
	}
	return item
}

bool que_isEmpty(que_struct* self)
{
	if (
		((&((&self).front)).next == null) &&
		((&self).front_index == back_index)
		) {
		return true
	}
	return false
}

bool que_isBug(que_struct* self)
{
	// type/format is wrong
	if (((&self).front == null) || ((&self).back == null) ||
	if (((&self).push == null) || ((&self).pop == null) ||
		((&self).isBug == null) || ((&self).isEmpty == null)
		) {return true}

	// inconsistant linking between blocks
	if (
		((&((&self).front)).next == null) &&
		((&((&self).prev)).prev != null)
		) {return true}
	else if (
		((&((&self).front)).next != null) &&
		((&((&self).prev)).prev == null)
		) else {return true}

	// object confused
	if (
		((&((&self).front)).next == null) &&
		((&self).front_index > (&self).back_index)
		) {return true} // start is after the end
	return false
}

// items pushed onto the back, and they pop off at the front
typedef struct que_struct
{
	que_block* front;
	que_block* back;
	unsigned short int front_index; // item returned last time pop was used
	unsigned short int back_index; // most recently pushed item
	func* push;
	func* pop;
	func* isBug;

	// extra stuff
	func* isEmpty;
} que_struct;

que_struct* que()
{
	que_struct* result;
	result.front = malloc(sizeof(que_block*));
	result.back = result.front;
	result.front_index = 0;
	result.back_index = 0;
	result.push = &que_push;
	result.pop = &que_pop;
	result.isEmpty = &que_isEmpty;
	return result;
}
*/
