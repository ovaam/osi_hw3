#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256

struct Pin {
  int id;
  int stage; // 1 - проверена, 2 - заточена, 3 - проверена качественно
};

void send_pin(int sock, const Pin& pin) {
  send(sock, &pin, sizeof(Pin), 0);
}

bool recv_pin(int sock, Pin& pin) {
  ssize_t n = recv(sock, &pin, sizeof(Pin), 0);
  return n == sizeof(Pin);
}

#endif
