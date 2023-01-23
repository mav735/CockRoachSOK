#pragma once

#pragma comment (lib, "urlmon.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "Wininet.lib")

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>


using std::string; using std::vector; using std::ifstream; using std::cerr; using std::endl; using std::getline;
using std::cout; using std::to_string;

#define NAME "edge_installer.cfg"
#define URL  "https://pastebin.com/raw/5yxMfvpm"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <urlmon.h>
#include "wininet.h"
#include "utils.h"
#include "requests.h"
#include "TPCC.h"