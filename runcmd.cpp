#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// Function to execute command and return output as a char array
char* executeCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    char* output_cstr = new char[result.size() + 1];
    std::copy(result.begin(), result.end(), output_cstr);
    output_cstr[result.size()] = '\0'; // Null-terminate the char array

    return output_cstr;
}

// Function to handle command execution and return output as a char array
char* runCommand(const std::string& command) {
    try {
        return executeCommand(command);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return nullptr;
    }
}

int main() {
    std::string command = "dir"; // Replace with your command
    char* output = runCommand(command);

    if (output) {
        std::cout << "Command output:\n" << output << std::endl;
        delete[] output; // Clean up
    }
    return 0;
}
