// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1007 // the port number
#define SERVER_ADDR "192.168.1.35" //  IP address of C3

int main() {
    int client_socket, valread;
    struct sockaddr_in serv_addr;
    char buffer[100] = {0};
    FILE *fp = fopen("F2.txt", "r");

    if (fp == NULL) {
        printf("Error opening file F2.txt\n");
        return 1;
    }

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    // Set server address and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    // Read file contents into buffer and send to server in three messages
    for (int i = 0; i < 3; i++) {
        fread(buffer, sizeof(char), 100, fp);
        send(client_socket, buffer, strlen(buffer), 0);
    }
 fclose(fp);
    // Tell the server that there is no more data to send
    send(client_socket, "DONE", strlen("DONE"), 0);

    // Receive contents of F3 from server and write to file
    char buffer3[201];
    FILE *fp2 = fopen("F3.txt", "wb");
    for (int i = 0; i < 3; i++) {
        int bytes_received = recv(client_socket, buffer3, 200, 0);
        if (bytes_received < 0) {
            printf("Error receiving data\n");
            exit(1);
        }
        buffer3[bytes_received] = '\0';
        fwrite(buffer3, sizeof(char), bytes_received, fp2);
    }

    fclose(fp2);

    // Close the socket and file
    close(client_socket);
   

    return 0;
}
