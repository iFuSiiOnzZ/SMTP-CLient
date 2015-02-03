#include "./smtp.h"

int main(int argc, char *argv[])
{
    SOCKET sockfd = NULL;

    ConnectToHost(&sockfd, 25, "127.0.0.1");
    SendHelo(sockfd, "host.local");

    MailFrom(sockfd);  
    MailTo(sockfd);

    MailCompose(sockfd);
    MailCloseConnection(sockfd);
    CloseHostConnection(sockfd);


    system("pause");
    return(EXIT_SUCCESS);
}
