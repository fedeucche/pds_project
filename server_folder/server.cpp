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
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;
using std::set;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

char clock_proto[] = "Give me your clock please";

void open_analyzer(){
    system("./analyzer/rec_analyzer");
}

int main(){

    thread analyzer(open_analyzer);

    auto startTime = chrono::high_resolution_clock::now();
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int count= 1;
    socklen_t cli_addr_len = sizeof(cli_addr);
    
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
    size_t mac_esp ;
    
    char filename[30];
    
    size_t connected_mac[20];
    int num_con_mac=0;
    
    // FILE CON MAC CONNESSI AL SERVER ALMENO UNA VOLTA
    FILE *fp= fopen("mac_list.txt", "w");
    fclose(fp);
   
    while(true){

        std::ofstream mac_list;
        mac_list.open("mac_list.txt", std::ofstream::out | std::ofstream::app);
        
        cout << "Server Session : " << count << "\n\n";
        
        printf("Waiting for client...\n");
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_addr_len );
        
        // Aggiorno MAC_Esp
        mac_esp = cli_addr.sin_addr.s_addr;
        printf("Connection accepted from esp32 nr: %d!\n", (int)mac_esp);

        if(num_con_mac == 0){
            mac_list << mac_esp << endl;
            connected_mac[num_con_mac] =  mac_esp;
            num_con_mac++;
        }
        
        int conn = 0;

        for (int l=0; l<num_con_mac; l++){
            if (connected_mac[l] !=  mac_esp)
                conn++;
        }
        
        if (conn>=num_con_mac){
            mac_list << mac_esp << endl;
            connected_mac[num_con_mac] = mac_esp;
            num_con_mac++;
        }

        sprintf(filename, "rec_%d.txt", (int)mac_esp);
        
        // CREO O APRO FILE LOG PER ESP CONNESSO
        std::ofstream fout;
        fout.open(filename, std::ofstream::out | std::ofstream::app);
        
        if (!fout){ 
            std::cerr << " can't open input - " << filename <<endl;
            return 1;
        }
        
        bzero(recvBuff, sizeof(recvBuff));
        bzero(size, sizeof(size));
        
        n = 0;
        
        read(connfd, size, sizeof(size));
        if(strcmp(size, clock_proto)==0){
            auto duration = chrono::high_resolution_clock::now() - startTime;
            auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(duration).count();
            cout<<"Responding Time: "<< elapsedTime << "s" << endl;
            sprintf(sendBuff, "%g", (double)elapsedTime);
            write(connfd, sendBuff, strlen(sendBuff));
        }
        else{
            printf("size of client buf = %d\n\n", atoi(size));
            
            int i; char c;
            for(i=0;i<atoi(size);i++){
                read(connfd, &c, sizeof(char));
                recvBuff[i] =  c;
            }
            
            /*
            for(i=0;i<atoi(size);i++)
                printf("%c",recvBuff[i]);
             */
            
            printf("\nSending response...\n");
            
            bzero(sendBuff, sizeof(sendBuff));
            auto duration = chrono::high_resolution_clock::now() - startTime;
            auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(duration).count();
            cout<<"Responding Time: "<< elapsedTime << "s" << endl;
            sprintf(sendBuff, "%g", (double)elapsedTime);
            
            write(connfd, sendBuff, strlen(sendBuff));

            fout << recvBuff << endl;
        }

        close(connfd);
        printf("Closed connection with esp32!\n");
        count++;
    }
}
