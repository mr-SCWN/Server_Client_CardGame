#include <iostream>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

int main(int argc, char ** argv)  { 
    WSADATA wd; 
    SOCKET fd;
    struct sockaddr_in sa;
    struct hostent* host;

    WSAStartup(MAKEWORD(2, 2), &wd);
    host = gethostbyname(argv[1]);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr.s_addr, host->h_addr, host->h_length);
    sa.sin_port = htons(atoi(argv[2]));
    connect(fd, (struct sockaddr*)&sa, sizeof(sa));

    while (true) {
        char buf[4096] = { 0 };
        int numBytes = recv(fd, buf, sizeof(buf) - 1, 0);

        if (numBytes > 0) { // checking what message is sended by server
            buf[numBytes] = '\0';
            string message(buf);

            if (message == "Twoja kolej") { // answering the server (M/m - more, L/l - less) 
                cout << "Twoja kolej\n ";
                cout << "Napisz, czy następna katra jest większa (M/m) lub mniejsza (L/l) " << endl;
                string userInput;
                getline(cin, userInput); // writing the message

                if (!userInput.empty()) {
                    send(fd, userInput.c_str(), userInput.size(), 0);
                }
            } else { // if the message isnt about your turn just show it
                cout << message << endl;
            }
        }
        else if (numBytes == 0) {  // server is offerd
            cout << "Serwer został wyłączony" << endl;
            break;
        }
        else if (numBytes < 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
            cerr << "Błąd wysyłania wiadomości: " << WSAGetLastError() << endl;
            break;
        }
    }


    closesocket(fd);
    WSACleanup();
    

    return 0;
}
