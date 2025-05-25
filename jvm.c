#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
    INST_PUSH,
    INST_POP,
    INST_ADD,
    INST_PRINT,
} Inst_Set;

typedef struct {
    int value;
    Inst_Set type;
} Inst;

Inst program[] = {
    {.type = INST_PUSH, .value = 15},
    {.type = INST_PUSH, .value = 10},
    {.type = INST_ADD},
    {.type = INST_PRINT},
    {.type = INST_PRINT},
};

#define PROGRAM_SIZE (sizeof(program) / sizeof(program[0]))
#define MAX_STACK_SIZE 1024
int stack[MAX_STACK_SIZE];
int stack_size;

void push(int value) {
    if (stack_size >= MAX_STACK_SIZE) {
	fprintf(stderr, "ERROR: Stack Overflow\n");
	exit(1);
    }
    stack[stack_size] = value;
    stack_size++;
}

int pop() {
    if (stack_size <= 0) {
	fprintf(stderr, "ERROR: Stack Underflow\n");
	exit(1);
    }
    return stack[--stack_size];
}

int main() {
    int a, b;
    for (size_t ip = 0; ip < PROGRAM_SIZE; ip++) {
	switch(program[ip].type) {
	    case INST_PUSH:
		push(program[ip].value);
		break;
	    case INST_POP:
		pop();
		break;
	    case INST_ADD:
		a = pop();
		b = pop();
		push(a + b);
		break;
	    case INST_PRINT:
		printf("%d\n", pop());
		break;
	}
    }
    return 0;
}
