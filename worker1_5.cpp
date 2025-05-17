#include "common.h"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: ./worker1 <IP> <PORT> <id>\n";
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serv_addr{};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

  connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

  int stage = 1;
  send(sock, &stage, sizeof(int), 0); // отправляем стадию

  srand(time(nullptr) + atoi(argv[3]));

  for (int i = 0; i < 5; ++i) {
    sleep(rand() % 3 + 1); // задержка
    Pin pin{1000 + i * 10 + atoi(argv[3]), stage};
    std::cout << "[Worker1] Sending pin " << pin.id << "\n";
    send_pin(sock, pin);
  }

  close(sock);
  return 0;
}
