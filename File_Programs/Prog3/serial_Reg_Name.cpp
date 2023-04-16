#include <iostream>
#include <fstream>
#include <string>

// Serial Number Program

int main() {
    std::string filename = "serial_numbers.txt";
    std::string serial_number;
    std::string registration_number;
    std::string name;

    std::cout << "Each serial number should be followed by a registration number.\n";
    std::cout << "The serial number should be a unique 4 digit number\n";
    std::cout << "The registration number should be in the format P15/... eg. P15/1234/2020\n\n";

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

        std::cout << "Enter a name: ";
        std::getline(std::cin >> std::ws, name);

        std::ofstream outfile(filename, std::ios_base::app);
        outfile << name << "\n";
        outfile.close();
        std::cout << "Name added.\n";
    }

    return 0;
}