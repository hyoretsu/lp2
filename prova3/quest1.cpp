#include <arpa/inet.h>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sys/socket.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define CLIENTS_QUANTITY 8
#define COMMAND_SIZE 6
#define PORT 9905

using namespace std;

class Monitor {
    public:
        ofstream log;
        mutex mutex;

        explicit Monitor(const char* filename) {
            log.open(filename);
        }
};

Monitor global_monitor("log.txt");

void write_log(int socket_id, const string &command, Monitor &monitor) {
    monitor.mutex.lock();

    monitor.log << "cliente " << socket_id << ": " << command << endl;

    monitor.mutex.unlock();
}

void close_log(Monitor &monitor) {
    monitor.mutex.lock();

    monitor.log.close();

    monitor.mutex.unlock();
}

void handle_client(int socket_id) {
    const char* firstMsg = "WELCOME\n";
    send(socket_id, firstMsg, sizeof(firstMsg), 0);

    char buffer[COMMAND_SIZE + 1];

    // Há um bug que ele passa pelo loop 2 vezes antes de parar no recv. Na segunda ele passou a receber um "\n" de algum canto, no começo tava dando certo (Já testei e não é por causa da função de escrever no log)
    while (true) {
        // Limpando o buffer
        memset(buffer, 0, COMMAND_SIZE);

        // Recebe uma mensagem enviada pelo cliente pela socket e armazena no buffer
        int recv_result = read(socket_id, &buffer, COMMAND_SIZE);
        if (recv_result < 0) {
            perror("recv() error!");
            exit(-1);
        }
        string command = buffer;

        cout << "Odeio C\n";
        cout << "O cliente " << socket_id << " solicitou o comando " << command << "\n";
        write_log(socket_id, command, global_monitor);

        if (strncmp(command.c_str(), "RNDNUM", COMMAND_SIZE) == 0) {
            string message = to_string(rand() % 1000) + "\n";

            send(socket_id, message.c_str(), message.length(), 0);
        } else if (strncmp(command.c_str(), "CRTIME", COMMAND_SIZE) == 0) {
            time_t t = time(NULL);
            tm datetime = *localtime(&t);

            string message;

            send(socket_id, message.c_str(), message.length(), 0);
        } else if (strncmp(command.c_str(), "FINISH", COMMAND_SIZE) == 0) {
            close(socket_id);

            cout << "Cliente " << socket_id << " desconectado" << "\n";

            break;
        } else if (strncmp(command.c_str(), "SHTDWN", COMMAND_SIZE) == 0) {
            close_log(global_monitor);
            exit(0);
        }

            // close(socket_id);
            // exit(0);
    }
}

int main() {
    cout << "Iniciando servidor na porta\n" << PORT << "\n";
    srand(time(NULL));

    // Cria a socket do servidor
    int	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket() error!");
        exit(-1);
    }

    // Cria e configura o endereço do servidor
    sockaddr_in server_address = {
        .sin_family = AF_INET,
	    .sin_port = htons(PORT),
	    .sin_addr = {
            .s_addr = INADDR_ANY
        },
    };

    // Linka o endereço com a socket
	int bind_result = bind(server_socket, (sockaddr*) &server_address, sizeof(server_address));
    if (bind_result < 0) {
        perror("bind() error!");
        exit(-1);
    }

    // Fica esperando (ouvindo) conexões no servidor
    int listen_result = listen(server_socket, CLIENTS_QUANTITY);
    if (listen_result < 0) {
        perror("listen() error!");
        exit(-1);
    }

    thread client_threads[CLIENTS_QUANTITY];
    sockaddr_in client_address;
    socklen_t client_address_size;
    int count = 0;

    while (true) {
        if (count > CLIENTS_QUANTITY) {
            cout << "O máximo de clientes já está conectado" << "\n";
            continue;
        }

        // Para nessa parte até algum cliente tentar se conectar
        int client_socket = accept(server_socket, (sockaddr*) &client_address, &client_address_size);
        if (client_socket < 0) {
            perror("accept() error!");
            exit(-1);
        }

        cout << "Novo cliente conectado!" << "\n";

        client_threads[count] = thread(handle_client, client_socket);
        client_threads[count].join();

        count++;
    }

    return 0;
}
