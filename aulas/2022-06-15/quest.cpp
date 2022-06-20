#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define BUFFER_SIZE 16
#define PORT 8080
#define QTD_CLIENTS 5

class Monitor {
    public:
        int soma;
        std::mutex mutex;
};

Monitor monitor;

int get_soma() {
    return monitor.soma;
}

void inc_soma(int n) {
    monitor.mutex.lock();
    monitor.soma += n;
    monitor.mutex.unlock();
}

void handle_client(int id) {
    char buffer[BUFFER_SIZE + 1];

    while (true) {
        int recv_result = recv(id, buffer, sizeof(buffer), 0);
        if (recv_result < 0) {
            perror("recv() error!");
            exit(-1);
        }

        std::cout << "Recebi: " << buffer << "\n";

        int n = atoi(buffer);

        if (n == 0) {
            std::cout << "Finalizando servidor..." << "\n";
            exit(0);
        }

        inc_soma(n);
        std::cout << "Soma: " << monitor.soma << "\n";
        n *= 2;

        std::string str_return = std::to_string(n);
        std::cout << "Retornei: " << n << "\n";

        send(id, str_return.c_str(), str_return.length(), 0);
    }
}

int main() {
    std::cout << "Iniciando servidor de soma na porta\n" << PORT << "\n";

    int	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket() error!");
        exit(-1);
    }

    sockaddr_in server_address = {
        .sin_family = AF_INET,
	    .sin_port = htons(PORT),
	    .sin_addr = {
            .s_addr = INADDR_ANY
        },
    };
	int bind_result = bind(server_socket, (sockaddr*) &server_address, sizeof(server_address));
    if (bind_result < 0) {
        perror("bind() error!");
        exit(-1);
    }

    int listen_result = listen(server_socket, QTD_CLIENTS);
    if (listen_result < 0) {
        perror("listen() error!");
        exit(-1);
    }

    std::thread client_threads[QTD_CLIENTS];
    sockaddr_in client_address;
    socklen_t client_address_size;
    int count = 0;

    while (true) {
        int client_socket = accept(server_socket, (sockaddr*) &client_address, &client_address_size);
        if (client_socket < 0) {
            perror("accept() error!");
            exit(-1);
        }

        std::cout << "Novo cliente conectado!" << "\n";

        char cli_ip_addr[16];
        inet_ntop(AF_INET, &client_address.sin_addr.s_addr, cli_ip_addr, sizeof(cli_ip_addr));

        std::cout << "Endereço do cliente: " << cli_ip_addr << "\n";

        client_threads[count] = std::thread(handle_client, client_socket);
        client_threads[count].join();
        count++;
    }

    return 0;
}
