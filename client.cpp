#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void run_client(const char* ip, int port, int stage_id, const std::string& name) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server{};
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server.sin_addr);

  if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
    std::cerr << "Connection failed\n";
    return;
  }

  int count = 0;
  while (true) {
    std::string msg = name + " processed item #" + std::to_string(++count) + "\n";
    send(sock, msg.c_str(), msg.size(), 0);
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1));
  }

  close(sock);
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: ./client <ip> <port> <stage> <name>\n";
    return 1;
  }

  run_client(argv[1], atoi(argv[2]), atoi(argv[3]), argv[4]);
  return 0;
}
