#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

// Function to remove '0x' prefix from a string.
void remove0x(string& line);

// Function to convert a decimal value to a specified base.
string decToBase(int value, int base);

// Function to convert a value from a given base to decimal.
int baseToDec(string value, int base);

// Function to convert a character representing a digit in a base to decimal.
int baseToDec(char c);

// Class representing a memory cell.
class Memory {
protected:
    int value{0};

public:
    // Constructor for Memory class.
    Memory(int val = 0);

    // Set the value of the memory cell.
    void setValue(int val);

    // Get the value of the memory cell.
    int getValue() const;

    // Get the binary representation of the memory cell's value.
    string binValue() const;

    // Get the hexadecimal representation of the memory cell's value.
    string hexValue() const;

    // Get the two's complement representation of the memory cell's value.
    int towsCompValue() const;
};

// Class representing a register, inherits from Memory.
class Register : public Memory {
public:
    // Equality operator for comparing a register with a memory cell.
    bool operator==(const Memory &x) const;

    // Inequality operator for comparing a register with a memory cell.
    bool operator!=(const Memory &x) const;

    // Assignment operator for assigning the value of a memory cell to a register.
    Register operator=(const Memory &x);

    // Assignment operator for assigning an integer to a register.
    Register operator=(const int &x);

    // Prefix increment operator for incrementing the value of a register.
    Register operator++();

    // Compound assignment operator for adding the value of another register to this register.
    Register operator+=(const Register &x);

    // Compound assignment operator for adding an integer to the value of a register.
    Register operator+=(const int &x);
};

// Class representing mathematical operations.
class Operation {
public:
    // Function to perform addition of two integers.
    int add(int x1, int x2);
};

// Class representing the machine language processor.
class ML {
    Operation op;      // Instance of the Operation class.
    Memory *M;         // Array representing memory cells.
    Register *R, PCtr, InsR;   // Arrays representing registers and program counter.
    int rCount, mSize;  // Number of registers and size of memory.
    bool halt;          // Flag to indicate whether the machine is halted.
    string screen;      // Screen content.

    //////////////////////////////
    // Private helper function to check the validity of an instruction.
    bool isvalid(string ins);

    // Functions for executing different instructions.
    void i1(string ins);
    void i2(string ins);
    void i3(string ins);
    void i4(string ins);
    void i5(string ins);
    void iB(string ins);

public:
    // Constructor for the ML class.
    ML(int memory_size = 256, int register_count = 16);

    // Function to get the number of registers.
    int registerCount();

    // Function to get the size of memory.
    int memorySize();

    // Function to execute one cycle of the machine language program.
    bool runCycle();

    // Function to get the content of the screen.
    string screenState();

    // Function to get a reference to a memory cell at a specified index.
    Memory &idxM(int index);

    // Function to get a reference to a register at a specified index.
    Register &idxR(int index);

    // Function to get the hexadecimal value of the program counter.
    string PC();

    // Function to get the hexadecimal value of the current instruction register.
    string IR();

    // Function to reset the machine.
    void reset();

    // Function to check if the machine is halted.
    bool ishalted();

    // Destructor for the ML class.
    ~ML();
};