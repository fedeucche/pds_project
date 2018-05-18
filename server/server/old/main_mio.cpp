#include <iostream>
#include <set>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <thread>
#include <future>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

using std::set;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int n;
    int count= 1;
    
    char size[20];
    char sendBuff[1025];
    char recvBuff[100000];
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(15100);
    
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);
    
    char filename[30];
    sprintf(filename, "/Users/Diegomarra/GitHub/pds_project/pds_project/server/rec.txt");
    
    std::ofstream fout;
    
    if ( ! fout )
    { std::cerr << " can't open input - " << filename <<
        endl; return 1; }
    fout.open(filename, std::ofstream::out | std::ofstream::app);
    snprintf(sendBuff, sizeof(sendBuff), "Saluti dal server!\nLa informo che anche oggi la madonna è un gran troione :)\n");
    
    
    
    while(1)
    {
        cout << "Server Session : " << count<< "\n\n";
        
        fout << "Server Session : " << count<< "\n\n";
        
        printf("Waiting for client...\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        printf("Connection accepted from esp32!\n");
        
        
        bzero( recvBuff, sizeof(recvBuff));
        bzero( size, sizeof(size));
        
        n = 0;
        
        read(connfd, size, sizeof(size));
        printf("size of client buf = %d\n\n", atoi(size));
        
        int i; char c;
        
        for(i=0;i<atoi(size);i++){
            
            read(connfd, &c, sizeof(char));
            recvBuff[i] =  c;
        }
        
        for(i=0;i<atoi(size);i++){
            printf("%c",recvBuff[i]);
            
        }
        
        printf("\n");
        printf("Sending response...\n");
        
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);
        
        fout<< recvBuff << endl;
        
        //fout.close();
        
        printf("Closed connection with esp32!\n");
        sleep(1);
        
        count++;
        
    }
    
}
