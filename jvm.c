#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
    INST_PUSH,
    INST_POP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_PRINT,
} Inst_Set;

typedef struct {
    int value;
    Inst_Set type;
} Inst;

#define DEF_INST_PUSH(x) {.type = INST_PUSH, .value = x}
#define DEF_INST_POP() {.type = INST_POP}
#define DEF_INST_ADD() {.type = INST_ADD}
#define DEF_INST_SUB() {.type = INST_SUB}
#define DEF_INST_MUL() {.type = INST_MUL}
#define DEF_INST_DIV() {.type = INST_DIV}
#define DEF_INST_PRINT() {.type = INST_PRINT}

Inst program[] = {
    DEF_INST_PUSH(5),
    DEF_INST_PUSH(10),
    DEF_INST_DIV(),
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

void print_stack() {
    printf("Stack: \n");
    if (stack_size == 0) printf("EMPTY\n");
    for (int i = 0; i < stack_size; i++) {
	printf("%d. %d\n", i, stack[i]);
    }
}

void write_program_to_file(char *file_path) {
    FILE *file = fopen(file_path, "wb");

    if (file == NULL) {
	fprintf(stderr, "ERROR: Could not write to this file path: %s\n", file_path);
	exit(1);
    }

    fwrite(program, sizeof(program[0]), PROGRAM_SIZE, file);

    fclose(file);
}

Inst* read_program_from_file(char *file_path) {
    FILE *file = fopen(file_path, "rb");

    if (file == NULL) {
	fprintf(stderr, "ERROR: Could not read from this file path: %s\n", file_path);
	exit(1);
    }
    Inst* instructions = (Inst*)malloc(sizeof(Inst) * MAX_STACK_SIZE);

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(instructions, sizeof(instructions[0]), length, file);

    fclose(file);
    return instructions;
}

int main() {
    int a, b;
    write_program_to_file("test.jvm");
    Inst* loaded_program = read_program_from_file("test.jvm");
    for (size_t ip = 0; ip < PROGRAM_SIZE; ip++) {
	switch(loaded_program[ip].type) {
	    case INST_PUSH:
		push(loaded_program[ip].value);
		break;
	    case INST_POP:
		pop();
		break;
	    case INST_ADD:
		a = pop();
		b = pop();
		push(a + b);
		break;
	    case INST_SUB:
		a = pop();
		b = pop();
		push(a - b);
		break;
	    case INST_MUL:
		a = pop();
		b = pop();
		push(a * b);
		break;
	    case INST_DIV:
		a = pop();
		b = pop();
		push(a / b);
		break;
	    case INST_PRINT:
		printf("%d\n", pop());
		break;
	}
    }

    print_stack();

    return 0;
}
