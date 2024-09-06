#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

void handle_client(int client_sock) {
    char buffer[4096];
    int read_bytes;
    while ((read_bytes = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << "Received: " << buffer << std::endl;
        send(client_sock, buffer, read_bytes, 0);
    }
    std::cout << "Client disconnected." << std::endl;
    close(client_sock);
}

int main() {
    const int server_port = 5555;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
        return 1;
    }

    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
        return 1;
    }

    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0) {
            perror("error accepting client");
            continue;
        }
        std::thread client_thread(handle_client, client_sock);
        client_thread.detach();  // Detach the thread to handle the client independently.
    }

    close(sock);
    return 0;
}
