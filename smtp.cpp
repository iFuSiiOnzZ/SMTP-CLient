#include "./smtp.h"

void ShowError(char *err)
{
    printf("%s : %ld\n", err, WSAGetLastError());
}

unsigned int ConnectToHost(SOCKET *sockfd, unsigned int port, char *hostName)
{
    WSADATA wsadata = { 0 };
    if(0 != WSAStartup(MAKEWORD(2, 2), &wsadata)) 
    {
        ShowError("Error WSAStartup!");
        return(1);
    }

    SOCKADDR_IN server = { 0 };
    HOSTENT *hostInfo = gethostbyname(hostName);
    char *ip =  inet_ntoa(*(struct in_addr *) *hostInfo->h_addr_list);

    server.sin_addr.S_un.S_addr = inet_addr(ip);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    
    *sockfd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == *sockfd) 
    {
        ShowError("Error socket!");
        WSACleanup();
        return(1);
    }

    if(SOCKET_ERROR == connect(*sockfd, (SOCKADDR *) &server, sizeof(SOCKADDR_IN))) 
    {
        ShowError("Error connect!");
        WSACleanup();
        return(1);
    }
    
    char buffer[256] = { 0 };
    recv(*sockfd, buffer, 256, 0);
    printf("%s", buffer);

    return(0);
}

void CloseHostConnection(SOCKET sockfd)
{
    closesocket(sockfd);
    WSACleanup();
}

void SendHelo(SOCKET sockfd, char *str)
{
    char buffer[256] = "HELO ";
    strcat(strcat(buffer, str), "\r\n");

    if(SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    { ShowError("HELO send error"); }
    
    memset(buffer, 0, 256);

    if(SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    { ShowError("HELO recv error"); }

    printf("%s", buffer);
}

void MailCloseConnection(SOCKET sockfd)
{
    if(SOCKET_ERROR == send(sockfd, "QUIT\r\n", strlen("QUIT\r\n"), 0))
    { ShowError("DATA send error"); }
}

void MailFrom(SOCKET sockfd)
{
    char userEmail[256] = { 0 };
    char buffer[256] = { 0 };

    printf("Your Email: ");
    gets(userEmail);

    strcat(buffer, "MAIL FROM: <");
    strcat(buffer, userEmail);
    strcat(buffer, ">\r\n");

    if(SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    { ShowError("MAIL FROM send error"); }

    memset(buffer, 0, 256);

    if(SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    { ShowError("MAIL FROM recv error"); }

    printf("%s", buffer);
}

void MailTo(SOCKET sockfd)
{
    char userEmail[256] = { 0 };
    char buffer[256] = { 0 };

    printf("Destination Email: ");
    gets(userEmail);

    strcat(buffer, "RCPT TO: <");
    strcat(buffer, userEmail);
    strcat(buffer, ">\r\n");

    if(SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    { ShowError("RCPT TO send error"); }

    memset(buffer, 0, 256);

    if(SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    { ShowError("RCPT TO recv error"); }

    printf("%s", buffer);
}

void MailCompose(SOCKET sockfd)
{
    char buffer[256] = { 0 };

    strcat(buffer, "DATA\r\n");

    if(SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    { ShowError("DATA send error"); }

    memset(buffer, 0, 256);

    if(SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    { ShowError("DATA recv error"); }

    printf("%s", buffer);
    memset(buffer, 0, 256);

    strcat(buffer, "\r\n");
    printf("Compose: "); gets(buffer);
    strcat(buffer, "\r\n.\r\n");

    char aux[256] = { 0 };
    strcat(strcat(aux, "\r\n"), buffer);

    if(SOCKET_ERROR == send(sockfd, aux, strlen(aux), 0))
    { ShowError("DATA compose send error"); }

    memset(buffer, 0, 256);

    if(SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    { ShowError("DATA compose recv error"); }  

    printf("%s", buffer);
}
