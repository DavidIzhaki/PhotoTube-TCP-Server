#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Mock database to store user watch history and video popularity
std::map<std::string, std::vector<std::string>> userWatchHistory;
std::map<std::string, int> videoPopularity;

// Function to handle client connections
void handle_client(int client_sock) {
    char buffer[4096];
    int read_bytes;

    while ((read_bytes = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_bytes] = '\0';  // Ensure null-terminated string
        std::string data(buffer);

        // Parse the data (userId,videoId)
        std::istringstream iss(data);
        std::string userId, videoId;
        getline(iss, userId, ',');
        getline(iss, videoId);
        std::cout << "Parsed userId: " << userId << " and videoId: " << videoId << std::endl;

        // Update user watch history and video popularity
        userWatchHistory[userId].push_back(videoId);
        videoPopularity[videoId]++;

        // Dummy recommendation logic (simply returns the last few videos watched by the user)
        std::vector<std::string> recommendations;
        for (auto& entry : userWatchHistory) {
            for (auto& vid : entry.second) {
                recommendations.push_back(vid);
                if (recommendations.size() >= 6) break;
            }
            if (recommendations.size() >= 6) break;
        }

        // Convert recommendations to a single string
        std::string response;
        for (auto& vid : recommendations) {
            if (!response.empty()) response += ",";
            response += vid;
        }
        std::cout << "Sending recommendations: " << response << std::endl;


        // Send back the recommendations
        send(client_sock, response.c_str(), response.size(), 0);
    }

    std::cout << "Client disconnected." << std::endl;
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

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
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
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
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
