#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include  "ros/ros.h"
//#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <errno.h>
#include <arpa/inet.h>
//#include <windows.h>
//#include <WS2tcpip.h>
#include <unistd.h>
#include <string.h>
#define MAXLINE 128
#define Random(x) (rand()%x)

using namespace std;

int main(int argc,char **argv) {
    ros::init(argc, argv, "sub_soket");
    ros::NodeHandle nh("~");
    char *servInetAddr = "192.168.1.3";
    int NumSize = 0;
    int socketfd;
    char buff[MAXLINE];
    struct sockaddr_in sockaddr;
    char recvline[MAXLINE], sendline[MAXLINE];
    int n;

	 	socketfd = socket(AF_INET,SOCK_STREAM,0);
		memset(&sockaddr,0,sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(10004);
		inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
		//inet_addr(AF_INET,servInetAddr,&sockaddr.sin_addr);
	 	if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 ) {
			// printf("connect error %s errno: %d\n",strerror(errno),errno);
			exit(0);
	 	}
	 	printf("receive mes from server:\n");
  
	 	n = recv(socketfd,buff,MAXLINE,0);
	 	buff[n] = '\0';
		printf("------%s\n",buff);


    close(socketfd);
    return 0;
}