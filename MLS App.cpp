// File: MLS App.cpp
// Purpose: Machine Learning Instructions Simulator
// Author: Marwan Osama
// Section: S17,18
// ID: 20220324
// TA: Rana Abdelkader
// Date: 15 Nov 2023

#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>
#include "ML.cpp"  // Including the implementation file; typically, you should include only the header file.
using namespace std;

int main() {
    ML machine;  // Creating an instance of the ML class.

    int op;
    vector<string> menu {
            " Load a new program from file",
            " Display screen state",
            " Display register state",
            " Display memory state",
            " Run one time (cycle)",
            " Run until halt",
    };

    cout << "Welcome to our ML Simulator, Select an option from the menu:\n";

    while (true) {
        int menuSize = (machine.ishalted() ? 4 : 6);

        // Displaying the menu options.
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        for (int i{1}; i <= menuSize; i++) {
            cout << '(' << i << ')' << menu[i - 1] << '\n';
        }
        cout << "(0) Exit\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << ">> ";
        cin >> op;

        if (op < 0 or op > menuSize) continue;

        if (op == 0) {
            return 0;
        } else if (op == 1) {  // Load a new program from file.
            machine.reset();
            string fileName, ins, content;
            char c;
            int idx{};

            cout << "Enter File Name: \n"
                    ">> ";
            cin >> fileName;
            fileName += ".txt";
            ifstream fs(fileName);
            ofstream os("temp.txt");
            string line;

            // Removing '0x' prefix from each line in the file and storing in 'temp.txt'.
            while (getline(fs, line)) {
                remove0x(line);
                os << line << '\n';
            }
            os.close();

            ifstream f("temp.txt");

            // Taking starting address in HEX form from the user.
            while (true) {
                string hex_idx;
                cout << "Starting Address (HEX form): \n" ">> ";
                cin >> hex_idx;
                idx = baseToDec(hex_idx, 16);
                if (idx >= 0 and idx < 256) break;
            }

            // Reading content from the 'temp.txt' and storing in memory cells.
            while (f >> c) {
                content.push_back(c);
            }

            for (int i{}; i + 1 < content.size(); i += 2) {
                ins.push_back(content[i]), ins.push_back(content[i + 1]);
                machine.idxM(idx).setValue(baseToDec(ins, 16));
                idx++;
                ins.clear();
            }
        } else if (op == 2) {  // Display screen state.
            cout << machine.screenState() << '\n';
        } else if (op == 3) {  // Display register state.
            for (int i{}; i < machine.registerCount(); i++) {
                cout << 'R' << decToBase(i, 16) << ": " << machine.idxR(i).hexValue() << '\n';
            }
            cout << "PC: " << machine.PC() << " IR: " << machine.IR() << '\n';
        } else if (op == 4) {  // Display memory state.
            cout << "  ";
            for (int i{}; i < 16; i++) {
                char c;
                if (i < 10) {
                    c = i + '0';
                } else {
                    c = i - 10 + 'A';
                }
                cout << c << "  ";
            }
            cout << endl;

            for (int i{}; i < 16; i++) {
                char c;
                if (i < 10) {
                    c = i + '0';
                } else {
                    c = i - 10 + 'A';
                }
                cout << c << " ";
                for (int j{}; j < 16; j++) {
                    cout << machine.idxM(i * 16 + j).hexValue() << " ";
                }
                cout << '\n';
            }
        } else if (op == 5) {  // Run one time (cycle).
            machine.runCycle();
        } else if (op == 6) {  // Run until halt.
            while (!machine.ishalted()) {
                machine.runCycle();
            }
        }
    }
}