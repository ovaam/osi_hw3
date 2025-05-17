#include "common.h"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: ./worker2 <IP> <PORT> <id>\n";
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serv_addr{};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

  connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

  int stage = 3;
  send(sock, &stage, sizeof(int), 0);

  srand(time(nullptr) + atoi(argv[3]));

  Pin pin;
  while (recv_pin(sock, pin)) {
    std::cout << "[Worker2] Received pin " << pin.id << ", sharpening...\n";
    sleep(rand() % 3 + 1);
    send_pin(sock, pin); // отправляем дальше
  }

  close(sock);
  return 0;
}
