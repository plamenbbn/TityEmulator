#include <stdio.h>
#include <stdbool.h>


#define MAXPROGRAMSIZE 30
#define MAXDATASIZE 10

char* instructions[] = { "N/A", "LOAD", "ADD", "STORE", "SUB", "IN", "OUT", "END", "JMP", "SKIPZ", "N/A" };

typedef struct {
    int opCode, deviceOrAddress;
} Instruction;

Instruction programMemory[MAXPROGRAMSIZE]; 

int dataMemory[MAXDATASIZE];


// PC=Program Counter; MAR=Memory Address Register, ACC=Accumulator, MDR2=Memory Data Register (2)
int PC=0, MAR1=0, MAR2=0, ACC=0, MDR2=0;

// IR = Instruction Register; MDR1 = Memory Data Register (1)
Instruction IR, MDR1; 

// Outputs the current status of the Data Memory array
void dumpDataMemory() {
   printf( "MEM = [" );
   for( int i=0; i<MAXDATASIZE; i++ ) {
        printf( "%i", dataMemory[i] );
        if( i<MAXDATASIZE-1 ) printf( "," );
    }
    printf( "]\n" );
}

// Output the current list of instructions in the Program Memory
void dumpProgMemory() {
   for( int i=0; i<MAXPROGRAMSIZE; i++ ) {
        Instruction instr = programMemory[i];
        printf( "ProgramMem[%i] : (%i, %i)\t", i, instr.opCode, instr.deviceOrAddress );
        if( instr.opCode > 0 && instr.opCode < 10 ) {
            printf( "%s %i\n", instructions[instr.opCode], instr.deviceOrAddress );
        } else {
            printf( "Illegal Instruction Op Code %i\n", instr.opCode );
        }
    }
}

// Flag to indicate if the program is still running or it has terminated
bool isRunning = true;

void load( int address ) {
    printf( "Loading Accumulator from memory location %i\n", address );
    // warning: no array-bounds checking yet...
    ACC = dataMemory[ address ];
}

void add( int address ) {
    printf( "Adding dataMemory[%i] to Accumulator\n", address );
	MAR2 = address;
	MDR2 = dataMemory[ MAR2 ];
    ACC = ACC + MDR2;
}

void store( int address ) {
    printf( "Storing Accumulator to memory location %i\n", address );
    // warning: no array-bounds checking yet...	
    dataMemory[ address ] = ACC;
}

void sub( int address ) {
    printf( "Subtracting dataMemory[%i] from Accumulator\n", address );
    ACC -= dataMemory[ address ];
}

void in() {
    printf( "Inputting value to Accumulator\n> " );
    scanf( "%i", &ACC );
    printf( "( received input %i and stored it in Accumulator )\n", ACC ); 
}

void out() {
    printf( "Outputting Accumulator to screen\n" );
    printf( "> %i\n", ACC );
}

void end( int status ) {
    printf( "Terminating program with status %i.\n", status );
    isRunning = false;
}

void jmp( int address ) {
    printf( "Jumping to program memory location %i\n", address );
    PC = address;
}

void skipz() {
   printf( "Skipping next instruction conditional on Accumulator==0 (current A=%i)\n", ACC );
   if( ACC == 0 ) PC++;
}


void run() {

    while( isRunning ) {
        // Print out registers
        printf( " PC=%i | A=%i | ", PC, ACC );
        // Print out memory status....
        dumpDataMemory();

        printf( "\n" );

		// Fetching the current instruction and incrementing the Program Counter
		MAR1 = PC;
		MDR1 = programMemory[MAR1];
		PC = PC + 1;
        IR = MDR1; 

        printf( "Executing Instruction (%i, %i) - %s %i \n", IR.opCode, IR.deviceOrAddress, instructions[IR.opCode], IR.deviceOrAddress ); 

        switch( IR.opCode ) {
            case 1: load( IR.deviceOrAddress ); break;
            case 2: add( IR.deviceOrAddress ); break;
            case 3: store( IR.deviceOrAddress ); break;
            case 4: sub( IR.deviceOrAddress ); break;
            case 5: in(); break;
            case 6: out(); break;
            case 7: end( IR.deviceOrAddress ); break;
            case 8: jmp( IR.deviceOrAddress ); break;
            case 9: skipz(); break; 
            default: printf( "Illegal Instruction OpCode %i -- ignoring it...\n", IR.opCode );
        }
    }

    printf( "Program terminated normally.\n" );
}


int readInput( FILE* stream ) {
    bool success=true;
    int opCode, operand, addr=0;
    while( success ) {
        success = fscanf( stream, "%i %i", &opCode, &operand ) == 2;
        if( success ) {
            printf( "Loaded instruction (%i, %i) in address %i.\n", opCode, operand, addr );
            programMemory[addr++] = (Instruction) { opCode, operand };
        } else {
            printf( "End of input reached.\n" );
        }
    }
    return 0;
}

int readFile( char* filename ) {
    printf( "Opening file '%s' for reading...\n", filename );
    FILE* fileptr = fopen(filename,"r");
    if( fileptr==NULL ) {
        printf( "No such file '%s'. Exiting!\n", filename );
        return -1;
    }
    int result = readInput( fileptr );
    fclose( fileptr );
    printf( "Done reading file '%s'.\n", filename );
    return result;
}


int main( int argc, char* argv[] ) {

    printf( "Argument Count=%i \n", argc );

    for( int i=0; i<argc; i++ ) {
        printf( "First Argument [%i] = %s \n", i, argv[i] );

    }

    bool inputRead = false;
    if( argc > 1 ) {
       inputRead = 0 == readFile( argv[1] );
    } else {
       printf( "Reading Tiny Program Instructions from Console. Please input instructions and operands now:" );
       inputRead = 0 == readInput( stdin );
    }

    if( inputRead ) {
        dumpDataMemory();
        dumpProgMemory();
        run();
        return 0;
    } else {
        printf( "No input read... Exiting" );
        return -1;
    }

}

