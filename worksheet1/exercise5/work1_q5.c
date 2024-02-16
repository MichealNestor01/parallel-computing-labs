//
// Singly linked list in serial, with both addition and removal of items.
// Normally you would use a library routine, C++'s std::forward_list<> etc.
// The purpose here is just to demonstrate the potential pitfall of data races.
//
// Compile with:
//
// gcc -fopenmp -Wall -o work1_q5 work1_q5.c
//
// (although '-fopenmp' is redundant until OpenMP functionality is added).
//
// Execute as:
//
// ./work1_q5 10 5
//
// which will add 10 items to the list, then remove 5, displaying the full list
// after addition and removal. The first argument is required but the second is optional.
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>


//
// Structures, globals and functions for the linked list.
//
typedef struct node {
	int data;					// The data contained in each node.
	struct node *next;			// Only next node (single-linked, not doubly-linked).
} node_t;

node_t *head = NULL;			// The head of the list; initialise to NULL.

// Add to the end of the list.
void addToList( int data )
{
	// Create a new node and fill its fields.
	node_t *newNode = (node_t*) malloc( sizeof(node_t) );
	newNode->data = data;
	newNode->next = NULL;

	// Find the tail of the list and add newNode to it. If the first item, set head to it.
	#pragma omp critical 
	{
		if( head==NULL )
		{
			head = newNode;
		}
		else
		{
			node_t *tail = head;
			while( tail->next != NULL ) tail = tail->next;
			tail->next = newNode;
		}
	}
}

// Remove the last item from the list and its data, or '-1' if there are no list items.
int popFromList()
{
	int retVal;
	#pragma omp critical 
	{
		if( head==NULL ) retVal = -1;					// If no items on the list.

		if( head!=NULL && head->next==NULL )			// If exactly one item on the list, delete it and return.
		{
			retVal = head->data;						// Take copy of the data before calling free(), as there is an admittedly slim
			free( head );								// chance that the memory occupied by head->data will be overwritten before 
			head = NULL;								// reading it's data value straight afterwards.
		}

		if( head!=NULL && head->next!=NULL )			// Have at least two nodes. Loop to find the penultimate one.
		{
			node_t *node = head, *nextNode;
			do
			{
				nextNode = node->next;
				if( nextNode->next == NULL ) break;
				node = nextNode;
			}
			while( node->next != NULL );

			// Set the penultimate node's 'next' pointer to NULL
			node->next = NULL;

			// Delete the final node and return its value.
			retVal = nextNode->data;
			free( nextNode );
		}
	}
	return retVal;
}

// Clear up the list (i.e. deallocate all memory)
void clearList()
{
	// If not items, nothing to delete.
	if( head==NULL ) return;

	node_t *next, *curr = head;
	do
	{
		next = curr->next;		// Find next in list before deallocating 'curr'.
		free( curr );
	}
	while( (curr=next) != NULL );
}

// Display the list
void printList()
{
	// If no items, display a message and return immediately.
	if( head==NULL )
	{
		printf( "No items in the list.\n" );
		return;
	}

	// Otherwise print node by node in order from head to tail.
	int count = 0;
	printf( "HEAD\n" );

	node_t *node = head;
	do
	{
		printf( "Node at address %p has 'data' %d and 'next' pointer %p.\n", node, node->data, node->next );
		node = node->next;
		count++;
	}
	while( node != NULL );

	printf( "TAIL\n" );
	printf( "Total no. of nodes: %d.\n", count );
}


//
// Main
//
int main( int argc, char** argv )
{
	int i;

	//
	// Parse the command line arguments: Need one for no. of items to add, and one for number to then delete.
	//
	if( argc!=2 && argc!=3 )
	{
		printf( "Call with: No. items to add (required); no. to subsequently delete (optional).\n" );
		return EXIT_FAILURE;
	}
	int numAdd = atoi(argv[1]), numDel = ( argc<3 ? 0 : atoi(argv[2]) );
	if( numAdd<=0 )
	{
		printf( "Must be a positive number of nodes (or could nor parse argument as integer).\n" );
		return EXIT_FAILURE;
	}
	if( numDel<0 )
	{
		printf( "Cannot delete a negative number of nodes.\n" );
		return EXIT_FAILURE;
	}

	//
	// Add items to the list with consecutive data values.
	//
	#pragma omp for
	for( i=0; i<numAdd; i++ )
		addToList( i );

	printf( "\nAfter adding %d nodes, the list looks like this:\n\n", numAdd );
	printList();


	//
	// Delete items from the list
	//
	if( numDel>0 )
	{
		printf( "\n" );
		#pragma omp for
		for( i=0; i<numDel; i++ )
			popFromList();

		printf( "\nAfter deleting %d nodes, the list looks like this:\n\n", numDel );
		printList();
	}

	//
	// Clean up and quit.
	//
	clearList();
	return EXIT_SUCCESS;
}


