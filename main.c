#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 40000

enum operations {
    P_INC = 1,
    P_DEC = 2,
    INC = 3,
    DEC = 4,
    PRINT = 5,
    GETC = 6,
    LOOP_START = 7,
    LOOP_END = 8
};


void pointer_increment(int *mem_pointer, int *instruction_pointer, char *memory) {
    ++(*mem_pointer);
    ++(*instruction_pointer);
}

void pointer_decrement(int *mem_pointer, int *instruction_pointer, char *memory) {
    --(*mem_pointer);
    ++(*instruction_pointer);
}

void memory_increment(int *mem_pointer, int *instruction_pointer, char *memory) {
    ++memory[*mem_pointer];
    ++(*instruction_pointer);
}

void memory_decrement(int *mem_pointer, int *instruction_pointer, char *memory) {
    --memory[*mem_pointer];
    ++(*instruction_pointer);
}

void print_mem(int *mem_pointer, int *instruction_pointer, char *memory) {
    printf("%c", memory[*mem_pointer]);
    ++(*instruction_pointer);
}

void set_mem(int *mem_pointer, int *instruction_pointer, char *memory) {
    memory[*mem_pointer] = getc(stdin);
    ++(*instruction_pointer);
}

void loop_start(int *mem_pointer, int *instruction_pointer, char *memory) {
    if(memory[*mem_pointer] == 0) {
        int skip_loops = 1;
        int operation = LOOP_START;
        while(operation != LOOP_END || skip_loops != 0) {
            ++(*instruction_pointer);
            operation = memory[*instruction_pointer];
            if(operation == LOOP_START) {
               skip_loops++;
            } else if (operation == LOOP_END && skip_loops < 0) {
               skip_loops--;
            }
        }
    } else {
        ++(*instruction_pointer);
    }
}

void loop_end(int *mem_pointer, int *instruction_pointer, char *memory) {
    if(memory[*mem_pointer] != 0) {
        int skip_loops = 1;
        int operation = LOOP_END;
        while(operation != LOOP_START || skip_loops != 0) {
            --(*instruction_pointer);
            operation = memory[*instruction_pointer];
            if(operation == LOOP_END) {
               skip_loops++;
            } else if (operation == LOOP_START && skip_loops < 0) {
               skip_loops--;
            }
        }
    } else {
        ++(*instruction_pointer);
    }
}

int read_program(char *memory, char *filename) {
    int mem_pointer = 0;

    FILE *f = fopen(filename, "r");
    int data = fgetc(f);

    while(data > 0) {
        switch((char)data) {
        case '>':
            memory[mem_pointer] = P_INC;
            break;
        case '<':
            memory[mem_pointer] = P_DEC;
            break;
        case '+':
            memory[mem_pointer] = INC;
            break;
        case '-':
            memory[mem_pointer] = DEC;
            break;
        case '.':
            memory[mem_pointer] = PRINT;
            break;
        case ',':
            memory[mem_pointer] = GETC;
            break;
        case '[':
            memory[mem_pointer] = LOOP_START;
            break;
        case ']':
            memory[mem_pointer] = LOOP_END;
            break;
        }
        if(memory[mem_pointer] > 0) {
            mem_pointer++;
        }
        data = fgetc(f);
    }
    return mem_pointer;
}

int execute_program(unsigned char *memory, int mem_pointer) {
    //void (*functions[])(int *, int *, char *) = {NULL, pointer_increment, pointer_decrement, memory_increment, memory_decrement, print_mem, set_mem, loop_start, loop_end};
    int mem_p = mem_pointer;
    int instruction_pointer = 0;
    unsigned char operation = memory[instruction_pointer];
    while(operation != 0) {
        operation = memory[instruction_pointer];
	switch(operation) {
	case P_INC:
		pointer_increment(&mem_p, &instruction_pointer, memory);
		break;
	case P_DEC:
		pointer_decrement(&mem_p, &instruction_pointer, memory);
		break;
	case INC:
		memory_increment(&mem_p, &instruction_pointer, memory);
		break;
	case DEC:
		memory_decrement(&mem_p, &instruction_pointer, memory);
		break;
	case PRINT:
		print_mem(&mem_p, &instruction_pointer, memory);
		break;
	case GETC:
		set_mem(&mem_p, &instruction_pointer, memory);
		break;
	case LOOP_START:
		loop_start(&mem_p, &instruction_pointer, memory);
		break;
	case LOOP_END:
		loop_end(&mem_p, &instruction_pointer, memory);
		break;
	}
        //printf("I: %d O: %d A: %d M: %d\n", instruction_pointer, operation, mem_p, memory[mem_pointer]);
        //(*functions[operation])(&mem_p, &instruction_pointer, memory);
    }
}

int main(int argv, char *argc[]) {
    if(argv < 2) {
        printf("You must supply a filename as command-line argument.\n");
        exit(1);
    }
    char *filename = argc[1];

    unsigned char *memory = calloc(sizeof(char), MEM_SIZE);

    int mem_pointer = read_program(memory, filename) + 10;
    execute_program(memory, mem_pointer);
}
