/*
 * Twan Tran @029136612 
 * Pornthep Bootchot @027817775 
 * Gavin Lampton @026357147 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Block {
    int block_size;
    struct Block* next_block;
};

const size_t OVERHEAD_SIZE = sizeof(struct Block);
const size_t POINTER_SIZE = sizeof(void*);
struct Block* free_head;

void my_initialize_heap(int size) {
  free_head = (struct Block*) malloc(size);
  free_head -> block_size = size - OVERHEAD_SIZE;
  free_head -> next_block = 0;
    // Create a pointer to the heap and assign it to the free_head.
    //free_head = (struct Block*)malloc(size);
    // Assign the block's size and next_block.
    //free_head->block_size = size - OVERHEAD_SIZE;
    //free_head->next_block = NULL;
}

void* my_alloc(int size)
{
	//First, the data size must be a multiple of (void*) size
	//Take the ceiling of the data size / (void*) size
	//to know which multiple of (void*) to use
	double dblSizeAsMultiple = ((double)size / (double)size);

	//Calculate the number of bytes to allocate for
	//the requested size and cast to integer
	int sizeAsMultiple = (int)(dblSizeAsMultiple * POINTER_SIZE);


	//Iterate through free list
	//First initialize pointers
	struct Block *curr = free_head;
	struct Block *prev = 0;
  bool found = false;
  void* ptr;
  // Iterare through each node, if a node has equal or more space than necessary to hold size, use that node.
	while (curr)
	{
		//If this block fits the size we want to allocate
		//(Uses the first fit heuristic)
		if (curr->block_size >= sizeAsMultiple)
		{
      found = true;
      // Determine if the current block can be split.
			int excessSpace = curr->block_size - sizeAsMultiple;

			//if splittable
			if (excessSpace >= (OVERHEAD_SIZE + POINTER_SIZE))
			{
					// Create a pointer to the newly split block's position then assign its structure members.
          // Update Curr's block size as a result of splitting.
					curr->block_size = sizeAsMultiple;

					// Adjust the double linked list, depending on whether curr is the head or not.
					struct Block *new_split_block = (struct Block *)((char*)curr + OVERHEAD_SIZE + sizeAsMultiple);
					new_split_block->block_size = excessSpace;

					//Initialize data pointer
					ptr = ((char*)new_split_block + OVERHEAD_SIZE);

					//Link the new (split) block into the free list
					//by making the new block the head of the free list 
					//and linking its next pointer to what used to be 
					//the head of the list.
					if (curr == free_head)
					{
						//new_split_block->next_block = currBlock->next_block;
						//currBlock->next_block = new_split_block;
						free_head = new_split_block;
					}
					else
					{
            // Not splittable If curr is the head, curr's next block is the new head.
						new_split_block->next_block = curr->next_block;
						prev->next_block = new_split_block;
					}
			}
			else
			{
					//If curr is not the head, the previous block points to curr's next block.
					if (curr == free_head)
					{
						free_head = curr->next_block;
					}
					else
					{
            // Haven't found an available space yet.
						prev->next_block = curr->next_block;
					}
					curr->next_block = 0;
			}

			//Initialize data pointer
			ptr = ((char*)curr + OVERHEAD_SIZE);

			// Return a pointer to the allocated data, if possible.
			return ptr;
		}
	}
}


// I think this part is correct so far, despite there is segment fault at line 83 while doing option 3
void my_free(void* data) {
    struct Block * freed_block = (struct Block *)((char*)data - OVERHEAD_SIZE);
    freed_block->next_block = free_head;
    free_head = freed_block;
}

void menuOptionOne() {
  int *numOne = my_alloc(sizeof(int));
  printf("Address of int A: %p\n", numOne);
  my_free(numOne);
  int *numTwo = my_alloc(sizeof(int));
  printf("Address of int B: %p\n", numTwo);
};

//Allocate two ints and print their addresses; they should be exactly the 
//larger of (the size of an integer; the minimum block size) apart.
void menuOptionTwo() {
  int *numOne = my_alloc(sizeof(int));
  printf("Address of int A: %p\n", numOne);
  int *numTwo = my_alloc(sizeof(int));
  printf("Address of int B: %p\n", numTwo);
  printf("Verifying Results...\n");
  int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*);
  printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d bytes\n", overheadPlusLarger);
  printf("Address B - Address A: %d bytes \n", (int)numTwo - (int)numOne);
};

//Allocate three ints and print their addresses, then free the second of 
//double values and print its address (to allocate an array in C, allocate 
//that the address is correct. Allocate another int and print its address; 
//same as the int that you freed.
void menuOptionThree() {
  int *numOne = my_alloc(sizeof(int));
  printf("Address of int A: %p\n", numOne);
  int *numTwo = my_alloc(sizeof(int));
  printf("Address of int B: %p\n", numTwo);
  int *numThree = my_alloc(sizeof(int));
  printf("Address of int C: %p\n", numThree);
  my_free(numTwo);
  printf("After freeing int B...\n");
  double *arr = my_alloc(2 * sizeof(double));
  printf("Address of array of 2 double values: %p\n", arr);
  int *numFour = my_alloc(sizeof(int));
  printf("Address of int D (should be the int B): %p\n", numFour);
};
//Allocate one char, then allocate one int, and print their addresses. They should be exactly the same
//distance apart as in test #2.
void menuOptionFour() {
  int check = 0;
  char *charOne = my_alloc(sizeof(char));
  printf("Address of char A: %p\n", charOne);
  int *numTwo = my_alloc(sizeof(int));
  printf("Address of int B: %p\n", numTwo);
  int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*);
  printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d\n", overheadPlusLarger);
};
//Allocate space for a 80-element int array, then for one more int value. 
//the int value is 80 * sizeof(int) + the size of your header after the 
//Verify that the int's address and value has not changed.
void menuOptionFive() {
  int *arr = my_alloc(80 * sizeof(int));
  int *numOne = my_alloc(sizeof(int));
  printf("Address of array: %p\n", arr);
  printf("Address of int A: %p\n", numOne);
  printf("Address of int value: %p\n", arr + 80 + sizeof(int));
  printf("Value of int A: %d\n", *numOne);
  printf("Difference betwween array and int A: %d\n", (int)numOne - (int)arr);
  my_free(arr);
  printf("After freeing array...\n");
  printf("Address of int value: %p\n", numOne);
  printf("Value of int A: %d\n", *numOne);
}
//Testing case
int main() {
  int menuChoice = 0;
  int runAgain = 1;
  while (runAgain == 1) {
    printf("\n1. Allocate an int \n2. Allocate two ints \n3. Allocate three ints \n4. Allocate one char \n5. Allocate space for an 80-element int array \n6. Quit \nChoose a menu option: ");
    scanf("%d", &menuChoice);
    printf("\n---Test Case %d---\n", menuChoice);
    my_initialize_heap(1000);
    if (menuChoice == 1) {
      menuOptionOne();
    }
    else if (menuChoice == 2) {
      menuOptionTwo();
    }
    else if (menuChoice == 3) {
      menuOptionThree();
    }
    else if (menuChoice == 4) {
      menuOptionFour();
    }
    else if (menuChoice == 5) {
      menuOptionFive();
    }
    else if (menuChoice == 6) {
      printf("Done!");
      runAgain = 0;
    }
  }
  return 0;
}