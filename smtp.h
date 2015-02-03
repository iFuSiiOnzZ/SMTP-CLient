#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

// http://smtp4dev.codeplex.com/
// http://es.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol

unsigned int ConnectToHost(SOCKET *sockfd, unsigned int port, char *hostName);
void CloseHostConnection(SOCKET sockfd);

void SendHelo(SOCKET sockfd, char *str);
void MailCloseConnection(SOCKET sockfd);
void MailCompose(SOCKET sockfd);
void MailFrom(SOCKET sockfd);
void MailTo(SOCKET sockfd);
void ShowError(char *err);
