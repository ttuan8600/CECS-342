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

const OVERHEAD_SIZE = sizeof(struct Block);
const POINTER_SIZE = sizeof(void*);
struct Block* free_head;

void my_initialize_heap(int size) {
  if (size <= 0){
    printf("Error: Size must be greater than 0.");
    return;
  }
  // Create a pointer to the heap and assign it to the free_head.
  free_head = (struct Block*)malloc(size);
  if (free_head == NULL){
    printf("Error: Malloc failed.");
    return;
  }
  // Assign the block's size and next_block.
  free_head->block_size = size - OVERHEAD_SIZE;
  free_head->next_block = NULL;
}

void* my_alloc(int size)
{
  //Check if size is valid
  if (size <= 0){
    printf("Error: Size must be greater than 0.");
    return NULL;
  }

  //Ensure size is a multiple of POINTER_SIZE
  size = (size + POINTER_SIZE - 1) / POINTER_SIZE * POINTER_SIZE;

	//Iterate through free list
	//First initialize pointers
	struct Block *curr = free_head;
	struct Block *prev = 0;
  ;
  bool found = false;

  // Iterare through each node, if a node has equal or more space than necessary to hold size, use that node.
	while (curr != NULL)
	{
		//If this block fits the size we want to allocate
		//(Uses the first fit heuristic)
		if (curr->block_size >= size){
      found = true;
      // Determine if the current block can be split.
			// if splittable
			if (curr->block_size > size + OVERHEAD_SIZE)
			{
        // Create a pointer to the newly split block's position then assign its structure members.
        struct Block *split_block = (struct Block *)((char*)curr + OVERHEAD_SIZE + size);
        split_block->block_size = curr->block_size - size - OVERHEAD_SIZE;
        split_block->next_block = curr->next_block;

        // Update Curr's block size as a result of splitting.
        curr->block_size = size;
        if (curr == free_head)
        {
          // If curr is the head, the split_block is the new head.
          free_head = split_block;
        }
        else
        {
          // If curr is not the head, the previous block points to the split_block.
          prev->next_block = split_block;
        }
			}
      //Not splittable
			else{
				//If curr is not the head, curr's next_block is the new head.
				if (curr == free_head){
					free_head = curr->next_block;
				}else{
          //If curr is not the head, the previous block points to curr's next_block.
					prev->next_block = curr->next_block;
				}
			}
      //Since we found a block, no need to keep searching.
      break;
		}else{
      //Haven't found available space yet
      prev = curr;
      curr = curr->next_block;
    }
	}
  //REturn a pointer to the allocated data, if possible.
  if (found){
    return (void*)((char*)curr + OVERHEAD_SIZE);
  }else{
    return NULL;
  }
}

// I think this part is correct so far, despite there is segment fault at line 83 while doing option 3
void my_free(void* data) {
  struct Block* freed_block = (struct Block*)((char*)data - OVERHEAD_SIZE);
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

// Allocate three ints and print their addresses, then free the second of 
// double values and print its address (to allocate an array in C, allocate 
// that the address is correct. Allocate another int and print its address; 
// same as the int that you freed.
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
  printf("Address B - Address A: %d bytes \n", (int)numTwo - (int)charOne);

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