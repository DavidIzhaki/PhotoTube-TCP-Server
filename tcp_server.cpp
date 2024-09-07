#include "tcp_server.h"
#include "RecommendationSystem.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

RecommendationSystem recommender;

void handle_client(int client_sock) {
    char buffer[4096];
    int read_bytes;

    while ((read_bytes = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_bytes] = '\0';  // Ensure null-terminated string
        std::istringstream iss(buffer);
        std::string userId, videoId;
        getline(iss, userId, ',');
        getline(iss, videoId);

         std::cout << "Parsed userId: " << userId << std::endl;
        std::cout << "Parsed videoId: " << videoId << std::endl;

        recommender.updateHistory(userId, videoId);
        auto recommendations = recommender.getRecommendations(userId, videoId);
        // Check if recommendations are empty
if (recommendations.empty()) {
    std::cout << "No recommendations found for user " << userId << std::endl;
} else {
    std::cout << "Recommendations for user " << userId << ": ";
    for (const auto& videoId : recommendations) {
        std::cout << videoId << " ";
    }
    std::cout << std::endl; // End the line after printing all recommendations
}
        std::string response;
        for (auto& vid : recommendations) {
            if (!response.empty()) response += ",";
            response += vid;
        }
        // Print recommendations to the server console for verification
        std::cout << "Recommendations sent for user " << userId << ": " << response << std::endl;


        send(client_sock, response.c_str(), response.size(), 0);
    }

    close(client_sock);
}

int main() {
    const int server_port = 2345;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    if (listen(sock, 5) < 0) {
        perror("Error listening to a socket");
        return 1;
    }

    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *)&client_sin, &addr_len);
        if (client_sock < 0) {
            perror("Error accepting client");
            continue;
        }
        std::thread client_thread(handle_client, client_sock);
        client_thread.detach();  // Detach the thread to handle the client independently
    }

    close(sock);
    return 0;
}