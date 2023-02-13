// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 1007 // the port number

int main() {
    // Create a socket for server P3
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error creating socket\n");
        exit(1);
    }

    // Bind the socket to a local address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 2) < 0) {
        printf("Error listening on socket\n");
        exit(1);
    }

int client_socket1 = -1;
int client_socket2 = -1;

while (1) {
    // Accept connections from clients P1 and P2
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        printf("Error accepting client connection\n");
        continue;
    }

    if (client_socket1 == -1) {
        client_socket1 = client_socket;
        printf("Received connection from P1\n");
    } else if (client_socket2 == -1) {
        client_socket2 = client_socket;
        printf("Received connection from P2\n");
    } else {
        printf("Ignoring additional connection\n");
        close(client_socket);
        continue;
    }

    if (client_socket1 != -1 && client_socket2 != -1) {
        break;
    }
}


    // Open file F3 for appending
    FILE *fp = fopen("F3.txt", "a");
    if (fp == NULL) {
        printf("Error opening file F3.txt\n");
        exit(1);
    }

    // Receive contents of F1 from P1 in three messages and append to F3
    char buffer[101];
    for (int i = 0; i < 3; i++) {
        int bytes_received = recv(client_socket1, buffer, 100, 0);
        if (bytes_received < 0) {
            printf("Error receiving data\n");
            exit(1);
        }
        buffer[bytes_received] = '\0';
        fwrite(buffer, sizeof(char), bytes_received, fp);
    }

    // Receive contents of F2 from P2 in three messages and append to F3
    for (int i = 0; i < 3; i++) {
        int bytes_received = recv(client_socket2, buffer, 100, 0);
        if (bytes_received < 0) {
            printf("Error receiving data\n");
            exit(1);
        }
        buffer[bytes_received] = '\0';
        fwrite(buffer, sizeof(char), bytes_received, fp);
    }
    fclose(fp);

    //Send content of P3 to P1
    FILE *fp2 = fopen("F3.txt", "r");

    char buffer2[200] = {0};

       for (int i = 0; i < 3; i++) {
        fread(buffer2, sizeof(char), 200, fp2);
        send(client_socket1, buffer2, strlen(buffer2), 0);
    }
    fclose(fp2);

    //Send content of P3 to P2

    FILE *fp3 = fopen("F3.txt", "r");

    char buffer3[200] = {0};

       for (int i = 0; i < 3; i++) {
        fread(buffer3, sizeof(char), 200, fp3);
        send(client_socket2, buffer3, strlen(buffer3), 0);
    
    }
    fclose(fp3);
    
    // Signal to P1 and P2 that we are done sending data
    send(client_socket1, "done", 4, 0);
    send(client_socket2, "done", 4, 0);

    // Close the sockets and file
    close(client_socket1);
    close(client_socket2);
    close(server_socket);
    

}