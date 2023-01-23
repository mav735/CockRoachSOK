#pragma once

vector<string> readFileLines(string pathFile) {
    string temp;
    vector<string> lines;
    ifstream input_stream(pathFile);

    while (getline(input_stream, temp)) {
        lines.push_back(temp);
    }
    return lines;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}