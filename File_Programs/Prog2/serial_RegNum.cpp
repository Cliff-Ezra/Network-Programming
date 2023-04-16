#include <iostream>
#include <fstream>
#include <string>

// Serial Number Program

int main() {
    // The file name is hard coded here, but you could ask the user for the file name
    std::string filename = "serial_and_reg_numbers.txt";
    std::string serial_number;
    std::string registration_number;

    std::cout << "The serial number should be a unique 4 digit number\n";
    std::cout << "The registration number should be in the format P15/... \n";

    while (true) {
        std::cout << "Enter a serial number (or 'exit' to quit): ";
        std::cin >> serial_number;

        if (serial_number == "exit") {
            break;
        }

        bool exists = false;
        std::ifstream infile(filename);
        std::string line;
        while (getline(infile, line)) {
            if (line == serial_number) {
                exists = true;
                break;
            }
        }
        infile.close();

        if (!exists) {
            std::ofstream outfile(filename, std::ios_base::app);
            outfile << serial_number << "\n";
            outfile.close();
            std::cout << "Serial number added.\n";
        } else {
            std::cout << "Serial number already exists.\n";
            continue;
        }

        while (true) {
            std::cout << "Enter a Registration Number (P15/...): ";
            std::cin >> registration_number;

            exists = false;
            infile.open(filename);
            while (getline(infile, line)) {
                if (line == registration_number) {
                    exists = true;
                    break;
                }
            }
            infile.close();

            if (!exists) {
                std::ofstream outfile(filename, std::ios_base::app);
                outfile << registration_number << "\n";
                outfile.close();
                std::cout << "Registration number added.\n";
                break;
            } else {
                std::cout << "Registration number already exists. Please enter a different registration number.\n";
            }
        }
    }

    return 0;
}