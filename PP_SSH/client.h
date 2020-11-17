#ifndef __CLIENT__
#define __CLIENT__

typedef struct socket_conn
{
    int sockfd;
    // more data elements if needed
} socket_conn;

socket_conn *make_connection(char *ip, char *port);
void send_command(char *command, socket_conn *con);
void close_connection(socket_conn *con,int forced);


#endif