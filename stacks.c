#include<stdio.h>
#include<stdlib.h>
#define INITIAL_CAPACITY 8
typedef struct stack //has its own struct bc it needs to keep track of all of the vars
{
    int *arr;
    size_t capacity;
    size_t size;
} Stack;

typedef Stack *StackPtr;

int pop(StackPtr stack);
int peek(Stack stack);
void push(StackPtr stack, int datum);
void destroy(StackPtr stack);
void print(Stack stack); //prints from first in to last in
Stack create(void);
//grid_stack portion;


Stack create(void)
{
    Stack stack;
    stack.arr = calloc(INITIAL_CAPACITY, sizeof(int));
    stack.capacity = INITIAL_CAPACITY;
    stack.size = 0;

    return stack;
}

void push(StackPtr stack, int datum)
{
    void resize_if_full(StackPtr stack);
    resize_if_full(stack);
    stack->arr[stack->size] = datum;
    ++stack->size;
}

void resize_if_full(StackPtr stack)
{
    if (stack->size == stack->capacity)
    {
        stack->capacity > 0 ? stack -> capacity *= 2 : (stack->capacity = INITIAL_CAPACITY);
        stack->arr = realloc(stack -> arr, stack -> capacity*sizeof(int));
    }
}

int pop(StackPtr stack)
{
        stack->size = stack->size - 1;
    int tmp = stack->arr[stack->size];
    stack->arr[stack->size] = 0;
    return tmp;
}

int peek(Stack stack)
{
    return stack.arr[stack.size - 1];
}

void print(Stack stack)
{
    for (size_t i = 0; i < stack.size; i ++)
        printf("%c ", stack.arr[i]);
    puts("");
}

void destroy (StackPtr stack)
{
    free(stack->arr);
    stack->arr = NULL;
    stack->size = 0;
    stack->capacity = 0;
}



