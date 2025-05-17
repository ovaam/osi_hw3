#include <iostream>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define PORT 12345
#define MAX_CLIENTS 10

// Обработка клиента — "рабочий участка 1"
void handle_client(int client_sock, int stage) {
  std::cout << "[STAGE " << stage << "] Client connected: socket " << client_sock << std::endl;

  // Случайное ожидание (симуляция работы)
  int work_time = rand() % 3 + 1;
  sleep(work_time);

  // Чтение/запись — простая симуляция передачи "булавки"
  char buffer[1024];
  int bytes = read(client_sock, buffer, sizeof(buffer));
  if (bytes > 0) {
    buffer[bytes] = '\0';
    std::cout << "[STAGE " << stage << "] Received: " << buffer << std::endl;

    // Ответ клиенту (например, "булавка не кривая")
    const char* reply = "OK\n";
    write(client_sock, reply, strlen(reply));
  }

  close(client_sock);
  std::cout << "[STAGE " << stage << "] Client finished.\n";
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));  // для случайных задержек

  int stage = 1;  // участок 1 (можно заменить аргументом командной строки)

  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    perror("socket");
    return 1;
  }

  sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    return 1;
  }

  if (listen(server_sock, MAX_CLIENTS) < 0) {
    perror("listen");
    return 1;
  }

  std::cout << "Server listening on port " << PORT << "...\n";

  while (true) {
    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);

    int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
      perror("accept");
      continue;
    }

    std::thread t(handle_client, client_sock, stage);
    t.detach();  // фоновый поток
  }

  close(server_sock);
  return 0;
}
