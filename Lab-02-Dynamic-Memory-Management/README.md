# CECS-342

This includes the source code and output for the Lab2-Dynamic Memory Managament

## Output

Test 1: intOne is allocated the list then freed, then intTwo is allocated to the same list => intTwo adress = intOne adress + 8 bytes (from intOne allocation) - 8 bytes(from freeing intOne) => intTwo address == intOne address

Test 2: Two ints were allocated, and their addresses differ by 24 bytes, which is the expected size of the overhead plus the larger of the size of an integer or the minimum block size = 16(overhead_size) + 4(int1_size) + 4(int2_size) = 24 bytes.

Test 3: Three ints were allocated, and the addresses of the three ints differ by 24 bytes each, which is the expected size of the overhead plus the larger of the size of an integer or the minimum block size. The address of int B is reused to allocate an array of 2 double values, and the address of int D is expected to be the same as int B, which is verified. After freeing int B, the address of the array is expected to be the same as the address of int D plus 24 bytes, which is verified.

Test 4: An int and a char were allocated, and their addresses differ by 24 bytes, which is the expected size of the overhead plus the larger of the size of an integer or the minimum block size.

Test 5: An array of 80 ints was allocated, and its address is followed by the address of int A, which is expected to be the same as the address of the first int value in the array. The value of int A is 0, which is expected for a newly allocated variable. The difference between the address of the array and the address of int A is 336 bytes, which is the expected size of the overhead plus the size of the array. After freeing the array, the address of int A and its value remain the same, which is expected because freeing the array does not affect the value or address of int A.

## Instruction

1. To run this code using the following command:

```shell
~$ gcc lab2.c -o lab2
~$ ./lab2
```

2. Then test them out using the given menu:

```
1. Allocate an int 
2. Allocate two ints 
3. Allocate three ints 
4. Allocate one char 
5. Allocate space for an 80-element int array 
6. Quit 
Choose a menu option:
```

## Contributor

Twan Tran

Pornthep Bootchot

Gavin Lampton
