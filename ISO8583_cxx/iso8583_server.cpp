#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

void processISOMessage(const std::string& msg) {
    std::cout << "Servidor recebeu mensagem ISO 8583:\n" << msg << std::endl;

    // Apenas exibe MTI (primeiros 4 bytes)
    std::string mti = msg.substr(0, 4);
    std::cout << "MTI: " << mti << std::endl;

    if (mti == "0200") {
        std::cout << "Solicitação de autorização recebida." << std::endl;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Criação do socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configuração
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Servidor aguardando conexões na porta " << PORT << "...\n";

    // Aceita conexão
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    std::cout << "Cliente conectado!\n";

    // Recebe mensagem
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    std::string receivedMsg(buffer);
    processISOMessage(receivedMsg);

    // Envia resposta
    std::string response = "021000"; // MTI 0210, código 00 (aprovado)
    send(new_socket, response.c_str(), response.length(), 0);
    std::cout << "Resposta enviada.\n";

    close(new_socket);
    close(server_fd);
    return 0;
}
