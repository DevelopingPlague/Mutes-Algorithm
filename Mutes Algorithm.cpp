#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <regex>

// 🌎 Global variables
std::string filepath;
std::string filename;
std::string wordsourcepath;
std::unordered_map<std::string, int> wordDictionary;
std::unordered_map<int, std::string> reverseWordDictionary;
int nextIndex = 0;  // Keeps track of next available index for new words

// ✅ Convert wstring to string (Supports Unicode)
std::string wstringToString(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// ✅ Extract filename from full file path
std::string extractFilename(const std::string& fullPath) {
    size_t lastSlash = fullPath.find_last_of("\\/");
    return (lastSlash == std::string::npos) ? fullPath : fullPath.substr(lastSlash + 1);
}

// ✅ Extract directory from full file path
std::string extractDirectory(const std::string& fullPath) {
    size_t lastSlash = fullPath.find_last_of("\\/");
    return (lastSlash == std::string::npos) ? "" : fullPath.substr(0, lastSlash);
}

// ✅ Open File Dialog
std::wstring openFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L""; // Buffer for file path
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.TXT\0\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = L"C:\\";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        std::string fullFilePath = wstringToString(fileName);
        filepath = fullFilePath;
        filename = extractFilename(fullFilePath);
        return std::wstring(fileName);
    }
    return L""; // No file selected
}

// ✅ Append new words to the word source
void appendToWordSource(const std::string& word) {
    std::ofstream file(wordsourcepath, std::ios::app);
    if (!file) {
        std::cerr << "Error: Could not open word source file for writing: " << wordsourcepath << std::endl;
        return;
    }
    file << word << std::endl;
    file.close();
}

// ✅ Process each line for encoding with adaptive learning
void processLine(const std::string& line) {
    std::string output = "";
    std::istringstream stream(line);
    std::string word;

    while (stream >> word) {
        auto it = wordDictionary.find(word);
        if (it != wordDictionary.end()) {
            output += "[" + std::to_string(it->second) + "] ";
        }
        else {
            // New word detected! Add it dynamically
            wordDictionary[word] = nextIndex;
            reverseWordDictionary[nextIndex] = word;
            appendToWordSource(word);

            // Encode the new word
            output += "[" + std::to_string(nextIndex) + "] ";
            nextIndex++;
        }
    }

    std::string directory = extractDirectory(filepath);
    std::ofstream outFile(directory + "\\[output]" + filename, std::ios::app);
    if (!outFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }
    outFile << output << std::endl;
    outFile.close();
}

// ✅ Decode line by replacing [index] with words
std::string decodeLine(const std::string& encodedLine) {
    std::string decodedLine = encodedLine;
    std::regex indexPattern(R"(\[(\d+)\])");
    std::smatch match;

    while (std::regex_search(decodedLine, match, indexPattern)) {
        int index = std::stoi(match[1].str());
        std::string replacement = reverseWordDictionary.count(index) ? reverseWordDictionary[index] : "[UNKNOWN]";
        decodedLine.replace(match.position(0), match.length(0), replacement);
    }

    return decodedLine;
}

// ✅ Read & encode entire file
void readAndProcessFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    std::string line;
    std::string directory = extractDirectory(filePath);
    std::ofstream outFile(directory + "\\[output]" + filename);
    if (!outFile) {
        std::cerr << "Error: Could not create output file." << std::endl;
        return;
    }
    outFile.close(); // Create file

    while (std::getline(file, line)) {
        processLine(line);
    }

    file.close();
}

// ✅ Read & decode entire file
void decodeFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    std::string directory = extractDirectory(filePath);
    std::ofstream decodedFile(directory + "\\[decoded]" + filename);
    if (!decodedFile) {
        std::cerr << "Error: Could not create output file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        decodedFile << decodeLine(line) << std::endl;
    }

    file.close();
    decodedFile.close();
    std::cout << "Decoded file created." << std::endl;
}

// ✅ Load word source for encoding & decoding
void loadWordSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open word source file: " << filePath << std::endl;
        return;
    }

    std::string word;
    while (std::getline(file, word)) {
        wordDictionary[word] = nextIndex;
        reverseWordDictionary[nextIndex] = word;
        nextIndex++;
    }

    file.close();
    std::cout << "Loaded " << wordDictionary.size() << " words into RAM.\n";
}

// ✅ Handle user input & start process
void query() {
    bool isrunning = true;
    while (isrunning) {
        std::string input;
        std::cout << "  __  ___      __      __         ___    __                 _ __  __  \n"
            " /  |/  /_  __/ /____ / _/ _____  /   |  / /___ _____  _____(_) / _/ /_  ____ ___  \n"
            "/ /|_/ / / / / __ `/ _ \\ / ___/  / /| | / / __ `/ __ \\/ ___/ / __ / __ \\ / __ `__ \\ \n"
            "/ /  / / /_/ / /_/ /  __/ /__/  / ___ |/ / /_/ / /_/ / /  / / /_/ / / / / / / / / /  \n"
            "/_/  /_/\\__,_/\\__, /\\___/\\___/  /_/  |_/_/\\__,_/\\____/_/  /_/\\__,_/_/ /_/ /_/ /_/   \n"
            "             /____/                                                                  \n"
            "Welcome! Please type in your option.\n"
            "0. Encode\n"
            "1. Decode\n"
            "2. Exit\n"
            << std::endl;

        std::wstring wordsource = openFileDialog();
        wordsourcepath = wstringToString(wordsource);
        loadWordSource(wordsourcepath);
        std::cin >> input;

        std::wstring selectedFilePath = openFileDialog();
        if (selectedFilePath.empty()) {
            std::cerr << "Error: No file selected!" << std::endl;
            return;
        }

        if (input == "0") {
            readAndProcessFile(filepath);
        }
        else if (input == "1") {
            decodeFile(filepath);
        }
        else if (input == "2") {
            isrunning = false;
        }
        else {
            std::cout << "ERROR: Invalid input `" << input << "`.\n";
        }
    }
}

int main() {
    query();
}
