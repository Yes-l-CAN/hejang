#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
void error_handling(char* messege);

#define BUF_SIZE 10

int main(int argc, char** argv)
{
    int                sock;
    struct sockaddr_in serv_addr;
    char               message[30];
    int                str_len;
    char*              result;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("socket from client: %d\n", sock);
    if (sock == -1)
        error_handling("socket() error\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    printf("before connect\n");
    printf("ccccccc\n");
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!\n");
    printf("connect");
    printf("after connect");

    puts("Connected!!\n");

    while (1)
    {
        fputs("Input message(press Q to quit) : ", stdout);
        if ((result = fgets(message, BUF_SIZE, stdin)) != NULL)
            printf("The string is %s\n", result);
        if (!strcmp(result, "Q\n"))
            break;
       str_len = send(sock, result, strlen(result), MSG_OOB);
        printf("socketfd : %d\n", sock);
      //  recv(sock, result, BUF_SIZE, 0);
        result[str_len] = 0;
        printf("result from server : %s(end)\n", result);
    }
    close(sock);
    return (0);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}



// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>

// #define BUF_SIZE 1024
// void error_handling(char *message);

// int main(int argc, char *argv[])
// {
//     int sock;
//     char message[BUF_SIZE];
//     int str_len;
//     struct sockaddr_in serv_adr;

//     if(argc!=3) {
//         printf("Usage : %s <IP> <port>\n", argv[0]);
//         exit(1);
//     }
    
//     sock=socket(PF_INET, SOCK_STREAM, 0);   
//     if(sock==-1)
//         error_handling("socket() error");
    
//     memset(&serv_adr, 0, sizeof(serv_adr));
//     serv_adr.sin_family=AF_INET;
//     serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
//     serv_adr.sin_port=htons(atoi(argv[2]));
    
//     if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
//         error_handling("connect() error!");
//     else
//         puts("Connected...........");
    
//     while(1) 
//     {
//         fputs("Input message(Q to quit): ", stdout);
//         fgets(message, BUF_SIZE, stdin);
//         if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
//             break;

//         write(sock, message, strlen(message));
//         str_len = read(sock, message, BUF_SIZE-1);
//         message[str_len] = 0;
//         printf("Message from server: %s", message);
//     }
    
//     close(sock);
//     return 0;
// }

// void error_handling(char *message)
// {
//     fputs(message, stderr);
//     fputc('\n', stderr);
//     exit(1);
// }