# PhotoTube-TCP-Server

## Introduction
The **PhotoTube-TCP-Server** is a core component of the PhotoTube system, designed to handle TCP communication between clients and the recommendation system. The server receives data on user video views and returns video recommendations based on shared watch history among users. The recommendation algorithm takes into account video popularity (number of times watched) and provides personalized suggestions.
*The algorithm only recommends videos that the user has not watched yet.*
## Getting Started

### 1. Installation Instructions

#### Clone the repository and install the dependencies with the following commands:

```bash
git clone https://github.com/DavidIzhaki/PhotoTube-TCP-Server.git
cd PhotoTube-TCP-Server
```

#### Setting up the TCP Port
In the `tcp_server.cpp` file, you'll find the `server_port` variable where you can set the desired port for the TCP server. It's important to ensure that this port matches the one configured in the `.env.local` file of the **PhotoTube-Server**. You can refer to the [PhotoTube-Server README](https://github.com/DavidIzhaki/PhotoTube-Server) for detailed instructions on how to configure the port.

### 2. Running the Server

Once the repository is cloned, you can compile and run the TCP server with the following steps:

#### Compilation:

```bash
g++ -Wall -std=c++17 -pthread tcp_server.cpp RecommendationSystem.cpp -o tcpServer
```

#### Running the server after compilation:

```bash
./tcpServer
```

Make sure to use the correct port, as described earlier.

## Setup and Configuration

For complete setup and configuration instructions, including integrating the TCP Server with the full PhotoTube system, please refer to the project Wiki on the **PhotoTube-Server** repository.

By following these steps, you should be able to successfully compile and run the TCP server, as well as integrate it with the Bloom Filter component and other aspects of the system. If you encounter any issues, please refer to the repository's documentation or reach out for support.

