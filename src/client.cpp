/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <thread>
#include <iostream>

using namespace std;

#define PORT "8081" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname port\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    thread recvThread([sockfd](){
            char buf[MAXDATASIZE];
            int numbytes = 0;
            while (1) {
                if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                    cout << "[RECV] Error occured, exiting" << endl;
                    break;
                } else if(numbytes == 0) {
                    cout << "[RECV] Error occured, exiting" << endl;
                    break;
                } else {
                    buf[numbytes] = '\0';
                    cout << "[RECV] Message received: " << buf << endl;
                }
            }
        });

    thread sendThread([sockfd](){
            while (1) {
                string message;
                cin >> message;
                if (send(sockfd, message.c_str(), message.size(), 0) == -1) {
                    cout << "[SEND] Error occured, exiting" << endl;
                    break;
                } else {
                    cout << "[SEND] Sent message: " << message << endl;
                }
            }
        });

    recvThread.join();
    sendThread.join();

    close(sockfd);

    return 0;
}
