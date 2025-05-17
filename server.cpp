#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::vector<int> monitor_sockets;
std::mutex monitor_mutex;

void broadcast_to_monitors(const std::string& msg) {
  std::lock_guard<std::mutex> lock(monitor_mutex);
  for (auto it = monitor_sockets.begin(); it != monitor_sockets.end();) {
    if (send(*it, msg.c_str(), msg.size(), 0) <= 0) {
      close(*it);
      it = monitor_sockets.erase(it);
    } else {
      ++it;
    }
  }
}

void handle_worker(int sock, int stage) {
  char buffer[1024];
  while (true) {
    ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) break;
    buffer[bytes] = '\0';

    std::string msg = "[Stage " + std::to_string(stage) + "] " + buffer;
    std::cout << msg;
    broadcast_to_monitors(msg);
  }
  close(sock);
}

void monitor_acceptor(int monitor_server_sock) {
  while (true) {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    int mon_sock = accept(monitor_server_sock, (sockaddr*)&addr, &len);
    if (mon_sock >= 0) {
      std::lock_guard<std::mutex> lock(monitor_mutex);
      monitor_sockets.push_back(mon_sock);
      std::cout << "[MONITOR] New monitor connected\n";
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./server <ip> <port>\n";
    return 1;
  }

  const char* ip = argv[1];
  int port = atoi(argv[2]);

  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr));
  listen(server_sock, 10);

  // Монитор-сокет на следующем порту
  int monitor_sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in monitor_addr{};
  monitor_addr.sin_family = AF_INET;
  monitor_addr.sin_port = htons(port + 1);
  inet_pton(AF_INET, ip, &monitor_addr.sin_addr);
  bind(monitor_sock, (sockaddr*)&monitor_addr, sizeof(monitor_addr));
  listen(monitor_sock, 10);

  std::thread(monitor_acceptor, monitor_sock).detach();

  std::cout << "Server running. Listening for clients...\n";
  while (true) {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_sock = accept(server_sock, (sockaddr*)&client_addr, &len);
    int stage = 1; // Допустим, все клиенты - Stage 1 (можно расширить)
    std::thread(handle_worker, client_sock, stage).detach();
  }

  return 0;
}