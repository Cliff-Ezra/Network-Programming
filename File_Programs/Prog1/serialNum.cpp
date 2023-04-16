#include <iostream>
#include <fstream>
#include <string>

int main() {
    // The file name is hard coded here, but you could ask the user for the file name
    std::string filename = "serial_numbers.txt";
    std::string serial_number;

    
    std::cout << "The serial number should be a unique 4 digit number\n";

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
        }
    }

    return 0;
}