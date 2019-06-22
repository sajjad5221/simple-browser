#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <QString>
#include <QtWebKitWidgets/QWebView>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
using namespace std;
char buff[MAX];
void func(int );
void checkHostEntry(struct hostent * );
char *getIP(char *);
void checkHostName(int);
QString buffString;



void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}


void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}






void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}


char *getIP(char *address)
{
    char *IPbuffer;
    struct hostent *host_entry;
    host_entry = gethostbyname(address);
    checkHostEntry(host_entry);
    IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    printf("Host IP: %s", IPbuffer);
    return IPbuffer;
}

void func(int sockfd)
{

    //char buff[MAX];
    write(sockfd, "GET /\r\n", strlen("GET /\r\n"));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    //QString buffString = QString::fromUtf8(buff);
    bool enter = false;
    while(read(sockfd, buff, sizeof(buff) - 1) != 0){
        /*if(buff == "\n")
            enter = true;
        if(enter == true)*/
            buffString += QString::fromUtf8(buff);
        fprintf(stderr, "%s", buff);
        bzero(buff, sizeof(buff));
    }

}
int makeReq(char *url)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    char* ip = getIP(url);
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(80);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    func(sockfd);
    close(sockfd);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_clicked()
{

    QString url = ui->address->text();
    QByteArray ba = url.toLocal8Bit();
    char *urlChar = ba.data();
    makeReq(urlChar);
    ui->webView->setHtml(buffString);
}

void MainWindow::on_address_returnPressed()
{

}
//از جایی شروع به خواندن و ذخیره صفحه کن که اینتر خورده باشه
