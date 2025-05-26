#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STACK_SIZE 1024

typedef enum {
    INST_PUSH,
    INST_POP,
    INST_DUP,
    INST_SWAP,
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

typedef struct {
    int stack[MAX_STACK_SIZE];
    int stack_size;
    size_t program_length;
    Inst* instructions;
} Machine;

#define DEF_INST_PUSH(x) {.type = INST_PUSH, .value = x}
#define DEF_INST_POP() {.type = INST_POP}
#define DEF_INST_DUP() {.type = INST_DUP}
#define DEF_INST_SWAP() {.type = INST_SWAP}
#define DEF_INST_ADD() {.type = INST_ADD}
#define DEF_INST_SUB() {.type = INST_SUB}
#define DEF_INST_MUL() {.type = INST_MUL}
#define DEF_INST_DIV() {.type = INST_DIV}
#define DEF_INST_PRINT() {.type = INST_PRINT}

Inst program[] = {
    DEF_INST_PUSH(5),
    DEF_INST_PUSH(10),
    DEF_INST_SWAP(),
};

#define PROGRAM_SIZE (sizeof(program) / sizeof(program[0]))

void push(Machine* machine, int value) {
    if (machine->stack_size >= MAX_STACK_SIZE) {
	fprintf(stderr, "ERROR: Stack Overflow\n");
	exit(1);
    }
    machine->stack[machine->stack_size] = value;
    machine->stack_size++;
}

int pop(Machine* machine) {
    if (machine->stack_size <= 0) {
	fprintf(stderr, "ERROR: Stack Underflow\n");
	exit(1);
    }
    return machine->stack[--machine->stack_size];
}

void print_stack(Machine* machine) {
    printf("Stack: \n");
    if (machine->stack_size == 0) printf("EMPTY\n");
    for (int i = 0; i < machine->stack_size; i++) {
	printf("%d. %d\n", i, machine->stack[i]);
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

void read_program_from_file(Machine* machine, char *file_path) {
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

    machine->program_length = length / 8;
    machine->instructions = instructions;

    fclose(file);
}

int main() {
    int a, b;
    write_program_to_file("test.jvm");
    Machine* machine = (Machine*)malloc(sizeof(Machine));
    read_program_from_file(machine, "test.jvm");
    for (size_t ip = 0; ip < machine->program_length; ip++) {
	switch(machine->instructions[ip].type) {
	    case INST_PUSH:
		push(machine, machine->instructions[ip].value);
		break;
	    case INST_POP:
		pop(machine);
		break;
	    case INST_DUP:
		a = pop(machine);
		push(machine, a);
		push(machine, a);
		break;
	    case INST_SWAP:
		a = pop(machine);
		b = pop(machine);
		push(machine, a);
		push(machine, b);
		break;
	    case INST_ADD:
		a = pop(machine);
		b = pop(machine);
		push(machine, a + b);
		break;
	    case INST_SUB:
		a = pop(machine);
		b = pop(machine);
		push(machine, a - b);
		break;
	    case INST_MUL:
		a = pop(machine);
		b = pop(machine);
		push(machine, a * b);
		break;
	    case INST_DIV:
		a = pop(machine);
		b = pop(machine);

		if (b == 0) {
		    fprintf(stderr, "ERROR: Dividing by 0.\n");
		    exit(1);
		}

		push(machine, a / b);
		break;
	    case INST_PRINT:
		printf("%d\n", pop(machine));
		break;
	}
    }

    print_stack(machine);

    return 0;
}
