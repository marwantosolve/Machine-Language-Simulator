#include "ML.h"

// Function to remove 0x from instractions to make it implementable by the program
void remove0x (string& line) {
    // Remove "0x"
    size_t pos = 0;
    while ((pos = line.find("0x", pos)) != string::npos) {
        line.replace(pos, 2, "");  // Replace "0x" with an empty string
    }
    // Remove spaces
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
}

// Function to convert a decimal value to a given base and return it as a string
string decToBase(int value, int base) {
    int val = value;
    string newValue;

    // Loop until the value becomes zero
    while (val) {
        // Find the remainder when dividing the value by the base
        int rem = val % base;
        // Update the value by dividing it by the base
        val /= base;

        char d;
        // Convert the remainder to a character
        if (rem < 10) {
            d = rem + '0'; // If the remainder is less than 10, use the digit
        } else {
            d = 'A' + rem - 10; // If the remainder is 10 or greater, use the corresponding letter
        }

        // Add the character to the front of the result string
        newValue.push_back(d);
    }

    // If the result is empty, the original value was 0
    if (newValue.empty()) newValue.push_back('0');

    // Reverse the result string to get the correct order
    reverse(newValue.begin(), newValue.end());

    // Return the result string
    return newValue;
}

// Function to convert a string representation of a number in a given base to decimal
int baseToDec(string value, int base) {
    // Reverse the input string to simplify the conversion process
    reverse(value.begin(), value.end());

    int pos{1}, num{}; // Initialize position and result variables

    // Iterate through each character in the reversed string
    for (auto &c : value) {
        int d;
        // Convert the character to a numeric value
        if (isalpha(c)) {
            d = c - 'A' + 10; // If the character is a letter, convert it to a number
        } else {
            d = c - '0'; // If the character is a digit, convert it to a number
        }

        // Add the numeric value multiplied by its positional weight to the result
        num += d * pos;
        // Update the positional weight for the next iteration
        pos *= base;
    }

    // Return the decimal representation of the number
    return num;
}

// Function to convert a single character to its decimal value
int baseToDec(char c) {
    int d;
    // Convert the character to a numeric value
    if (isalpha(c)) {
        d = c - 'A' + 10; // If the character is a letter, convert it to a number
    } else {
        d = c - '0'; // If the character is a digit, convert it to a number
    }

    // Return the decimal representation of the character
    return d;
}

/////////////////////////////////////////////////////////////////////////////////////////

// Memory class constructor, initializes the 'value' member with the provided 'val'.
Memory::Memory(int val): value{val} {

}

// Set the value of the 'Memory' object to the specified 'val'.
void Memory::setValue(int val) {
    value = val;
}

// Get the current value of the 'Memory' object (read-only).
int Memory::getValue() const {
    return value;
}

// Compute and return the two's complement representation of the stored value.
int Memory::towsCompValue() const {
    // Get the binary representation of the value.
    string bi_val = binValue();

    int num{};
    bool rev{};

    // Check if the value is negative (first bit is 1).
    if (bi_val[0] == '1') {
        int sz = bi_val.size();

        // Compute the two's complement by flipping bits and adding 1.
        for (int i{sz - 1}; i >= 0; i--) {
            if (rev) bi_val[i] = !(bi_val[i] - '0') + '0';
            if (bi_val[i] - '0') rev = true;
        }

        // Convert the binary representation back to decimal and multiply by -1.
        return baseToDec(bi_val, 2) * -1;
    }

    // If the value is positive, return it as is.
    return value;
}

// Get the binary representation of the stored value with leading zeros.
string Memory::binValue() const {
    // Convert the decimal value to binary.
    string bi_val = decToBase(value, 2);

    // Ensure the binary representation has 8 bits by adding leading zeros if necessary.
    while (bi_val.size() < 8) bi_val = '0' + bi_val;

    return bi_val;
}

// Get the hexadecimal representation of the stored value with leading zeros.
string Memory::hexValue() const {
    // Convert the decimal value to hexadecimal.
    string hex_val = decToBase(value, 16);

    // Ensure the hexadecimal representation has 2 characters by adding leading zeros if necessary.
    while (hex_val.size() < 2) hex_val = '0' + hex_val;

    return hex_val;
}

////////////////////////////////////////////////////////////////////////////////////////

// Perform addition of two integers and return the result.
int Operation::add(int x1, int x2) {
    // Calculate the sum of the two integers.
    int sum = x1 + x2;

    // Convert the sum to binary representation.
    string binSum = decToBase(sum, 2);

    // Ensure that the binary representation has at most 8 bits by trimming if necessary.
    if (binSum.size() > 8) binSum = binSum.substr(binSum.size() - 8, 8);

    // Convert the trimmed binary sum back to decimal and return the result.
    return baseToDec(binSum, 2);
}

////////////////////////////////////////////////////////////////////////////////////////

// Equality operator for Register and Memory comparison.
bool Register::operator==(const Memory &x) const {
    // Check if the value of the Register is equal to the value of the Memory object.
    return value == x.getValue();
}

// Inequality operator for Register and Memory comparison.
bool Register::operator!=(const Memory &x) const {
    // Check if the value of the Register is not equal to the value of the Memory object.
    return value != x.getValue();
}

// Assignment operator for assigning the value of a Memory object to a Register.
Register Register::operator=(const Memory &x) {
    // Set the value of the Register to the value of the Memory object.
    value = x.getValue();
    return *this; // Return the updated Register.
}

// Assignment operator for assigning an integer to a Register.
Register Register::operator=(const int &x) {
    // Set the value of the Register to the provided integer.
    value = x;
    return *this; // Return the updated Register.
}

// Prefix increment operator for incrementing the value of a Register.
Register Register::operator++() {
    value++; // Increment the value of the Register.
    return *this; // Return the updated Register.
}

// Compound assignment operator for adding the value of another Register to this Register.
Register Register::operator+=(const Register &x) {
    value += x.value; // Add the value of the other Register to this Register's value.
    return *this; // Return the updated Register.
}

// Compound assignment operator for adding an integer to the value of a Register.
Register Register::operator+=(const int &x) {
    value += x; // Add the provided integer to the value of the Register.
    return *this; // Return the updated Register.
}

/////////////////////////////////////////////////////////////////////////////////////////

// Copy from memory to register
void ML::i1(string ins) {
    // Extract register index and memory cell index from the instruction.
    int register_idx = baseToDec(ins[1]), memory_cell_idx = baseToDec(ins.substr(2, 2), 16);

    // Copy the value from memory to the specified register.
    R[register_idx] = M[memory_cell_idx];
}

// Assign a value to a register
void ML::i2(string ins) {
    // Extract register index and value from the instruction.
    int register_idx = baseToDec(ins[1]), value = baseToDec(ins.substr(2, 2), 16);

    // Assign the specified value to the register.
    R[register_idx] = value;
}

// Copy from register to memory
void ML::i3(string ins) {
    // Extract register index and memory cell index from the instruction.
    int register_idx = baseToDec(ins[1]), memory_cell_idx = baseToDec(ins.substr(2, 2), 16);

    // Copy the value from the register to the specified memory cell.
    M[memory_cell_idx] = R[register_idx];

    // If copying to memory cell 0, update the screen with the value.
    if (memory_cell_idx == 0) screen.push_back(M[memory_cell_idx].getValue());
}

// Copy from register to register
void ML::i4(string ins) {
    // Extract source and destination register indices from the instruction.
    int register_idx_1 = baseToDec(ins[2]), register_idx_2 = baseToDec(ins[3]);

    // Copy the value from one register to another.
    R[register_idx_2] = R[register_idx_1];
}

// Add (2's complement) operation
void ML::i5(string ins) {
    // Extract indices of the registers involved in the addition.
    int r[3];
    for (int i{}; i < 3; i++) r[i] = baseToDec(ins[i + 1]);

    // Perform the addition and store the result in the specified register.
    R[r[0]] = op.add(R[r[1]].getValue(), R[r[2]].getValue());
}

// Jump instruction
void ML::iB(string ins) {
    // Extract register index and memory cell index from the instruction.
    int register_idx = baseToDec(ins[1]), memory_cell_idx = baseToDec(ins.substr(2, 2), 16);

    // If the value in the specified register is equal to the value in register 0, jump to the specified memory cell.
    if (R[register_idx] == R[0]) PCtr = memory_cell_idx;
}

///////////////////////////////////////////////////////////////////////////////////////////

// Constructor for the ML class, initializes memory and registers, sets program counter, and halting flag.
ML::ML(int memSize, int regCount) : mSize(memSize), rCount(regCount) {
    // Allocate memory and registers.
    M = new Memory[memSize];
    R = new Register[regCount];

    // Initialize program counter and halting flag.
    PCtr = 0;
    halt = false;
}

// Returns the number of registers in the ML instance.
int ML::registerCount() {
    return rCount;
}

// Returns the size of the memory in the ML instance.
int ML::memorySize() {
    return mSize;
}

// Execute one cycle of the machine language program.
bool ML::runCycle() {
    // Check if the program counter exceeds the memory size or the halt flag is set.
    if (PCtr.getValue() > 254 or halt) {
        halt = true;
        return false;
    }

    // Fetch the instruction from memory using the program counter.
    string ins = M[PCtr.getValue()].hexValue();

    // Increment the program counter.
    ++PCtr;

    // Concatenate the next part of the instruction to create the full instruction.
    ins += M[PCtr.getValue()].hexValue();

    // If the program counter is less than 255, increment it again.
    if (PCtr.getValue() < 255) ++PCtr;

    // Convert the instruction to decimal for further processing.
    InsR = baseToDec(ins, 16);

    // Decode the instruction based on the opcode and execute the corresponding operation.
    if (ins[0] == '1') {
        i1(ins);
        return true;
    } else if (ins[0] == '2') {
        i2(ins);
        return true;
    } else if (ins[0] == '3') {
        i3(ins);
        return true;
    } else if (ins[0] == '4') {
        i4(ins);
        return true;
    } else if (ins[0] == '5') {
        i5(ins);
        return true;
    } else if (ins[0] == 'B') {
        iB(ins);
        return true;
    }

    // Check for halt instruction (C000).
    if (ins == "C000") {
        halt = true;
        return true;
    }

    // If none of the conditions are met, return false.
    return false;
}

// Checks if the machine is halted.
bool ML::ishalted() {
    return halt;
}

// Resets the machine, clearing registers, memory, resetting program counter, and halting flag.
void ML::reset() {
    // Reset all registers to zero.
    for (int i{}; i < rCount; i++) {
        R[i] = 0;
    }

    // Reset all memory cells to zero.
    for (int i{}; i < mSize; i++) {
        M[i] = 0;
    }

    // Reset program counter, halting flag, and screen.
    PCtr = 0;
    halt = false;
    screen.clear();
}

// Returns the content of the screen as a string.
string ML::screenState() {
    return screen;
}

// Returns the hexadecimal value of the program counter.
string ML::PC() {
    return PCtr.hexValue();
}

// Returns the hexadecimal value of the current instruction register.
string ML::IR() {
    return InsR.hexValue();
}

// Returns a reference to the memory cell at the specified index.
Memory & ML::idxM(int index) {
    return M[index];
}

// Returns a reference to the register at the specified index.
Register & ML::idxR(int index) {
    return R[index];
}

// Destructor for the ML class, deallocates memory and registers.
ML::~ML() {
    delete [] M;
    delete [] R;
}