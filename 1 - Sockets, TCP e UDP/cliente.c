#include <stdio.h>
#include <winsock2.h>
#include <locale.h>
#include <stdbool.h>
#include <process.h>
#include <locale.h>

#define MAX_MESSAGE_LENGTH 1024

void receiveMessages(void* socketPtr) {
    SOCKET socket = *(SOCKET*)socketPtr;
    char server_reply[MAX_MESSAGE_LENGTH];
    
    
    while (1) {
        if (recv(socket, server_reply, MAX_MESSAGE_LENGTH, 0) == SOCKET_ERROR) {
            printf("Erro ao receber resposta do servidor\n");
            break;
        }
        printf("\nResposta do servidor: %s", server_reply);
    }
}


int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char server_reply[MAX_MESSAGE_LENGTH];
    char client_message[MAX_MESSAGE_LENGTH];
    setlocale(LC_ALL, "Portuguese");


    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha ao inicializar a biblioteca Winsock. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Falha ao criar o socket. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);


    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Falha ao conectar-se ao servidor. Erro: %d\n", WSAGetLastError());
        return 1;
    }


    printf("Conectado\nEU SOU O CLIENTE\n");

    
    _beginthread(receiveMessages, 0, &s);
    printf("Digite uma mensagem ou digite 'sair' para encerrar o programa.\n");


    while (1) {
       
        fgets(client_message, MAX_MESSAGE_LENGTH, stdin);


        if (strcmp(client_message, "sair\n") == 0)
            break;


        if (send(s, client_message, strlen(client_message), 0) < 0) {
            printf("Falha ao enviar mensagem para o servidor\n");
            break;
        }
    }


    closesocket(s);
    WSACleanup();
    printf("Programa encerrado. Pressione qualquer tecla para sair...\n");
    getchar();
    return 0;
}
