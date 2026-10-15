
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main(void)
{
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    double num1, num2;
    char op;
    int n;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to Mathematical Operations Server!\n");

    while (1)
    {
        printf("\nEnter first number (or 0 to continue): ");
        if (scanf("%lf", &num1) != 1)
            break;

        printf("Enter operator (+, -, *, /) or x to exit: ");
        scanf(" %c", &op);

        if (op == 'x' || op == 'X')
        {
            strcpy(buffer, "exit");
            send(client_socket, buffer, (int)strlen(buffer), 0);
            break;
        }

        printf("Enter second number: ");
        if (scanf("%lf", &num2) != 1)
            break;

        // Prepare request
        snprintf(buffer, sizeof(buffer), "%.10g %c %.10g",
                 num1, op, num2);

        // Send request
        if (send(client_socket, buffer,
                 (int)strlen(buffer), 0) == SOCKET_ERROR)
        {
            printf("Send failed.\n");
            break;
        }

        // Receive result
        n = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);
    }

    closesocket(client_socket);
    WSACleanup();

    printf("Client closed.\n");
    return 0;
}
