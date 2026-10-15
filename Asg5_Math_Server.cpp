
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    double num1, num2, result;
    char op;
    int n;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for client
    if (listen(server_socket, 1) == SOCKET_ERROR)
    {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Mathematical Operations Server started on port %d.\n",
           PORT);
    printf("Waiting for client...\n");

    // Accept client connection
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET)
    {
        printf("Client connection failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");

    while (1)
    {
        // Receive request from client
        n = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        // Exit condition
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Client requested to exit.\n");
            break;
        }

        // Parse numbers and operator
        if (sscanf(buffer, "%lf %c %lf",
                   &num1, &op, &num2) != 3)
        {
            strcpy(buffer, "Invalid input.");
        }
        else
        {
            switch (op)
            {
                case '+':
                    result = num1 + num2;
                    break;

                case '-':
                    result = num1 - num2;
                    break;

                case '*':
                    result = num1 * num2;
                    break;

                case '/':
                    if (num2 == 0)
                    {
                        strcpy(buffer, "Error: Division by zero.");
                        break;
                    }
                    result = num1 / num2;
                    break;

                default:
                    strcpy(buffer, "Invalid operator.");
                    goto send_response;
            }

            if (op == '/' && num2 == 0)
            {
                /* Error message already stored in buffer. */
            }
            else if (op == '+' || op == '-' ||
                     op == '*' || op == '/')
            {
                snprintf(buffer, sizeof(buffer),
                         "Result = %.2f", result);
            }
        }

    send_response:
        printf("Client request: %.100s\n", buffer);

        if (send(client_socket, buffer,
                 (int)strlen(buffer), 0) == SOCKET_ERROR)
        {
            printf("Send failed.\n");
            break;
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    printf("Server closed.\n");
    return 0;
}
