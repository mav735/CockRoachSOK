#pragma once
int initSocket(WSADATA *wsaData, addrinfo *hints) {
    int iResult = WSAStartup(MAKEWORD(2, 2), wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(hints, sizeof(*hints));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;

    return 0;
}

int connectToServer(WSADATA* wsaData, addrinfo* hints, addrinfo* result, addrinfo* ptr, SOCKET* ConnectSocket) {
    vector<string> settings = getSettingsFromPaste();

    int iResult = getaddrinfo(settings[0].c_str(), settings[1].c_str(), hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return iResult;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        *ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (*ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(*ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(*ConnectSocket);
            *ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (*ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    return 0;
}

int initServerSocket(WSAData *wsaData, addrinfo* hints, addrinfo* result, addrinfo* ptr, SOCKET* ConnectSocket) {
    int status_connect = 1;
    while (status_connect) {
        status_connect = connectToServer(wsaData, hints, result, ptr, ConnectSocket);
        if (status_connect) {
            WSADATA wsaData;
            SOCKET ConnectSocket = INVALID_SOCKET;
            struct addrinfo* result = NULL, * ptr = NULL, hints;
            if (initSocket(&wsaData, &hints)) {
                system("pause");
                return 1;
            }
        }
        Sleep(1000);
    }
}

int sendMessageToServer(SOCKET *ConnectSocket, const char *sendbuf) {
    int iResult = send(*ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    return 0;
}

int stopConnection(SOCKET *ConnectSocket) {
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(*ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(*ConnectSocket);
        WSACleanup();
        return 1;
    }
}

vector <char> recieveMessage(SOCKET *ConnectSocket, char *recvbuf, int recvbuflen) {
    int iResult = 1;
    vector <char> message;
    do {
        iResult = recv(*ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            for (int i = 0; i < iResult; i++) {
                message.push_back(recvbuf[i]);
            }
            printf("Got message: ");
            for (int i = 0; i < message.size(); i++)
            {
                printf("%c", message[i]);
            }
            printf("\n");
            iResult = shutdown(*ConnectSocket, SD_RECEIVE);
        }
        else if (iResult == 0) {
            printf("\n");
        }
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
    } while (iResult > 0);
    return message;
}