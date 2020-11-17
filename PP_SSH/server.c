#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "log.h"
#include "server.h"
#include "prg.h"
#define PORT 8000
#define BUFFER_LENGTH (100 * 1024 * 1024L)

void encryptFunction(char *command)
{
    for (int x = 0; x < strlen(command); x++)
    {
        command[x] = (command[x] ^ ((unsigned char)getNum()));
        if (command[x] == '\0' || command[x] == '\255')
        {
            command[x] = 'A';
        }
    }
}

void showHex(char *command)
{
    char hs[strlen(command) * 2];
    for (int x = 0; x < strlen(command); x++)
    {
        hs[2 * x] = command[x];
        hs[2 * x + 1] = command[x] >> 4;
        if ((hs[2 * x] & 0x0F) >= 10)
        {
            hs[2 * x] = (hs[2 * x] & 0x0F) + 'A' - 10;
        }
        else
        {
            hs[2 * x] = (hs[2 * x] & 0xF0) + '0';
        }

        if ((hs[2 * x + 1] & 0x0F) >= 10)
        {
            hs[2 * x + 1] = (hs[2 * x + 1] & 0x0F) + 'A' - 10;
        }
        else
        {
            hs[2 * x + 1] = (hs[2 * x + 1] & 0xF0) + '0';
        }
    }
    // printf("\n");
    printf(" HEX String :: %s\n", hs);
}

int strcompare(char *s1, char *s2)
{
    if (strlen(s1) > strlen(s2))
    {
        return 0;
    }
    for (int x = 0; x < strlen(s1); x++)
    {
        if (s1[x] != s2[x])
        {
            return 0;
        }
    }

    return 1;
}

void recAck(int fd)
{
    int rl = 0;

    do
    {
        char *ack = (char *)calloc(100, 1);
        read(fd, ack, 100);
        rl = strlen(ack);
        if (rl != 0)
        {
            printf("[DEBUG]Ack recieved :: %s\n", ack);
        }
        free(ack);
    } while (rl == 0);
}

void sendAck(int fd)
{
    send(fd, "ack", 3, 0);
    printf("Acknowledgement sent to client.\n");
}

char *trimString(char *str)
{
    char *ret = (char *)malloc(1024);
    int ind = 0, flag = 0;
    int lastIndex = strlen(str) - 1;
    while (lastIndex >= 0)
    {
        if (str[lastIndex] != ' ' && str[lastIndex] != '\n' && str[lastIndex] != '\t')
        {
            break;
        }
        lastIndex--;
    }
    for (int x = 0; x <= lastIndex; x++)
    {
        if (str[x] != ' ' && str[x] != '\t')
        {
            flag = 1;
        }
        if (flag != 0)
        {
            ret[ind++] = str[x];
        }
    }
    return ret;
}

int understand(char *msg, int new_socket)
{
    recAck(new_socket); // ready to accept new message
    char *str = (char *)calloc(MINLEN * 5, 1);
    strcat(msg, " > tmpop.txt");
    printf("\nCommand sent to system :: %s \n\n", msg);
    system(msg);
    int fd1 = open("tmpop.txt", O_RDWR | __O_LARGEFILE | O_CREAT, 0666);
    if (fd1 < 0)
    {
        char ss[3] = "INV";
        encryptFunction(ss);
        showHex(ss);
        send(new_socket, ss, 3, 0);
        return 0;
    }
    int rl = 1;
    rl = read(fd1, str, MINLEN * 5);
    // printf("[ debug ] %s is str,ms is %s\n", str, msg);
    // encrypt
    encryptFunction(str);
    showHex(str);
    send(new_socket, str, rl, 0); // sends command response

    printf("[ debug ] command response sent to client is %s\n", str);
    if (str != NULL)
        free(str);      // ERROR HERE
    recAck(new_socket); // receives acknowledgement from client for command completed
    close(fd1);
    system("rm -rf tmpop.txt ");
    return 0;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;         // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons(PORT);       // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    int ll = listen(server_fd, 3);
    if (ll < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int pid, new;
    static int counter = 0;
    for (;;)
    {
        // label to go to on fork
    a:
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

        if ((pid = fork()) == -1)
        {
            close(new_socket);
            continue;
        }
        else if (pid > 0)
        {
            counter++;
            //wait();
            // code for server continue here
            goto a;
            // printf("here2");
            //close(new);
            // continue;
        }
        else if (pid == 0)
        {
            counter++;
            printf("Client %d has joined the server\n", counter);
            initMain("PASSWORD1234", 12);
            // code for all clients here
            while (1)
            {
                char *buffer = (char *)calloc(1024, 0);
                valread = read(new_socket, buffer, 1024); // accepts the command
                showHex(buffer);
                encryptFunction(buffer);
                sendAck(new_socket); // sends acknowledgement for command recieved
                // no need to trim string

                if (understand(buffer, new_socket)) // interpret the command
                {
                    break; // in case of exit
                }
                free(buffer);
                sendAck(new_socket); // send acknowledgement for command completed
            }
            printf("Client %d has left the server\n", counter);
        }
    }
    close(server_fd);
    return 0;
}
