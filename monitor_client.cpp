#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./monitor_client <ip> <port>\n";
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server{};
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &server.sin_addr);

  if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
    std::cerr << "Monitor connection failed\n";
    return 1;
  }

  char buffer[1024];
  while (true) {
    ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) break;
    buffer[bytes] = '\0';
    std::cout << buffer;
  }

  close(sock);
  return 0;
}
