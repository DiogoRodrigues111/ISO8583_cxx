#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

std::string buildISOMessage() {
    std::string mti = "0200"; // Solicitação de autorização
    std::string bitmap = "7230001000000000";
    std::string field2 = "5412345678901234"; // Cartão
    std::string field3 = "000000";           // Transação
    std::string field4 = "000000010000";     // Valor
    std::string field7 = "0318123456";
    std::string field11 = "123456";
    std::string field14 = "2503";
    std::string field41 = "TERM1234";
    std::string field49 = "986";

    return mti + bitmap + field2 + field3 + field4 + field7 + field11 + field14 + field41 + field49;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    std::string message = buildISOMessage();
    send(sock, message.c_str(), message.length(), 0);
    std::cout << "Mensagem enviada para o servidor.\n";

    // Recebe resposta
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Resposta do servidor: " << buffer << std::endl;

    close(sock);
    return 0;
}
