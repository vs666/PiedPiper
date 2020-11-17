#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "client.h"
#include "log.h"
#include "prg.h"

int readAck(socket_conn *con)
{
    // code here
    int rl = 0;

    do
    {
        char *ack = (char *)calloc(100, 1);
        read(con->sockfd, ack, 100);
        rl = strlen(ack);
        free(ack);
    } while (rl == 0);
    return 0;
}

int sendAck(socket_conn *con)
{
    send(con->sockfd, "Ack", 3, 0);
    return 0;
}

socket_conn *make_connection(char *ip, char *port)
{
    initMain("PASSWORD1234", 12);
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return NULL;
    }

    socket_conn *ob = (socket_conn *)malloc(sizeof(socket_conn));
    ob->sockfd = sock;

    return ob;
}

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

void send_command(char *command, socket_conn *con)
{
    // add decryption function here
    encryptFunction(command);
    showHex(command);
    send(con->sockfd, command, strlen(command), 0); // client sends command as string
    readAck(con);                                   // client recieves the acknowledgement from the server for command received

    sendAck(con); // sends acknowledgement to server that it is ready to accept output

    char *op_command = (char *)calloc(MINLEN * 5, 1); // = (char *)calloc(MINLEN * 5, 1); // max size of output is minlen*5 (arbritrary)
    int rl = 0;
    rl = read(con->sockfd, op_command, MINLEN * 5); // client reads command response
    showHex(op_command);
    encryptFunction(op_command);
    printf("response is :: %s", op_command); // display the command
    printf("\n");
    free(op_command);

    sendAck(con); // sends acknowledgement for command recieved
}

void close_connection(socket_conn *con, int forced)
{
    char *message = (char *)malloc(MINLEN);
    sprintf(message, "%s", CLOSE);
flag:
    send(con->sockfd, message, strlen(message), 0);
    if (readAck(con) != 2)
    {
        fprintf(stderr, "Error in closing connection. Connection still open\n");
    }
    else
    {
        exit(0);
    }
    if (forced)
    {
        goto flag;
    }
}

void assertCommand(int argc, char *argv[])
{
    printf("IP : %s \nPORT : %s \n", argv[1], argv[2]);
    // code here
}

char *trimString(char *str)
{
    char *ret = (char *)calloc(1024, 0);
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

int main(int argc, char *argv[]) // command :: ./client <ip> <port>
{
    argv[1] = trimString(argv[1]);
    argv[2] = trimString(argv[2]);
    assertCommand(argc, argv);
    socket_conn *con = make_connection(argv[1], argv[2]);
    if (con == NULL)
    {
        fprintf(stderr, "Error :: Unable to establish connection.\n");
        exit(1); // connection unestablished
    }

    size_t s = MINLEN;
    printf("Enter client Name : ");
    char *cl_name = (char *)malloc(MINLEN);
    getline(&cl_name, &s, stdin);
    cl_name = trimString(cl_name);

    while (1)
    {
        printf("\033[1;33m<client:%s>\033[0m ", cl_name);
        char *command = (char *)calloc(MINLEN, 0);
        getline(&command, &s, stdin);
        command = trimString(command);
        printf("Command is %s\n", command);
        send_command(command, con);
        readAck(con); // recieved acknowledgement for command completion
        free(command);
    }
}