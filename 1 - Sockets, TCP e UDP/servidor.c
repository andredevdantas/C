#include <stdio.h>
#include <winsock2.h>
#include <locale.h>
#include <stdbool.h>
#include <process.h>
#define MAX_MESSAGE_LENGTH 1024

void receiveMessages(void* socketPtr) {
    SOCKET socket = *(SOCKET*)socketPtr;
    char message[MAX_MESSAGE_LENGTH];

    while (1) {
        if (recv(socket, message, MAX_MESSAGE_LENGTH, 0) == SOCKET_ERROR) {
            printf("Erro ao receber mensagem do cliente\n");
            break;
        }
        printf("\nMensagem recebida: %s", message);
    }
}



int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char message[MAX_MESSAGE_LENGTH];
    setlocale(LC_ALL, "Portuguese");


    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("biblioteca Winsock não inicia. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket não criado. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);


    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Erro ao associar o socket à porta. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    printf("Aguardando conexão\n");


    listen(s, 3);


    c = sizeof(struct sockaddr_in);
    if ((new_socket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET) {
        printf("Conexão feita com êxito\nEU SOU O SERVIDOR\n");


        _beginthread(receiveMessages, 0, &new_socket);


        printf("Digite uma mensagem ou digite 'sair' para encerrar o programa.\n");


        while (1) {
           
            fgets(message, MAX_MESSAGE_LENGTH, stdin);


            if (strcmp(message, "sair\n") == 0)
                break;


            send(new_socket, message, strlen(message), 0);
        }
    }
    else {
        printf("Deu erro Junin Erro: %d\n", WSAGetLastError());
        return 1;
    }


    closesocket(new_socket);
    closesocket(s);
    WSACleanup();


    printf("Programa encerrado. Pressione qualquer tecla para sair...\n");
    getchar();
    return 0;
}
