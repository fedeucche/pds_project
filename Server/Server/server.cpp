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

using namespace std;
using std::set;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

/*

Praticamente bisogna fare in modo che se è la prima volta che sente
il mac (nuova esp si aggiunge alla lan), invia solamente il proprio clock
poichè quando una nuova esp si connette per la prima volta chiede prima
di ogni altra cosa il clock al server per sincronizzarsi.

PS. usare char stringa_clock[50]; read(connfd, stringa_clock, sizeof(stringa_clock));
	che altrimenti va tutto a puttane

*/

int main(int argc, char *argv[])
{
    //Clock
    auto startTime = chrono::high_resolution_clock::now();
    
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    
    //clock_t elapsedTime = (clock() - startTime)/CLOCKS_PER_MS;
    
    int n;
    int count= 1;
    
    char size[20];
    char sendBuff[25];
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
    
    // FILE CON MAC CONNESSI AL SERVER ALMENO UNA VOLTA
    FILE *fp= fopen("mac_list.txt", "w");
    fclose(fp);
    std::ofstream mac_list;
    mac_list.open("mac_list.txt", std::ofstream::out | std::ofstream::app);
    int connected_mac[20];
    int num_con_mac=0;
    
    while(1) {
        
        cout << "Server Session : " << count<< "\n\n";
        
        auto duration = chrono::high_resolution_clock::now() - startTime;
        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout<<"Start Time:"<< elapsedTime <<"ms"<< endl;
        printf("Waiting for client...\n");
        
        // ACCEPT CLIENT
        connfd = accept(listenfd, (struct sockaddr*) &cli_addr, &cli_addr_len );
        printf("Connection accepted from esp32!\n");
        
        duration = chrono::high_resolution_clock::now() - startTime;
        elapsedTime = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout<<"Time: "<< elapsedTime <<"ms"<< endl;
        
        // Aggiorno MAC_Esp
        mac_esp =   cli_addr.sin_addr.s_addr;
        if( num_con_mac==0){
            mac_list<<mac_esp<<endl;
            connected_mac[num_con_mac]=(int) mac_esp;
            num_con_mac++;
        }
        for (int l=0; l<num_con_mac; l++)
        {
            
             if ( connected_mac[l] != (int) mac_esp)
             {   mac_list<<mac_esp<<endl;
                 connected_mac[num_con_mac]=(int) mac_esp;
                 num_con_mac++;
                 break;
             }
        
        }
        
        sprintf(filename, "rec_%d.txt", (int) mac_esp);
        
        // CREO O APRO FILE LOG PER ESP CONNESSO
        std::ofstream fout;
        fout.open(filename, std::ofstream::out | std::ofstream::app);
        
        if ( ! fout )
        { std::cerr << " can't open input - " << filename <<endl;
            return 1; }

        fout << "Server Session : " <<  count <<"\t"<<mac_esp<< "\n\n";

        
        bzero( recvBuff, sizeof(recvBuff));
        bzero( size, sizeof(size));
        
        n = 0;
        
        // RECEIVE
        read(connfd, size, sizeof(size));
        printf("Size of Client Response = %d \n\n", atoi(size));
        
        int i; char c;
        for(i=0;i<atoi(size);i++){
            
            read(connfd, &c, sizeof(char));
            recvBuff[i] =  c;
        }
        
          /*
        for(i=0;i<atoi(size);i++){
            printf("%c",recvBuff[i]);
            
        }
         */
        
        //RESPONSE

        printf("\n");
        printf("Sending response...\n");
        
        bzero(sendBuff, sizeof(sendBuff));
        duration = chrono::high_resolution_clock::now() - startTime;
        elapsedTime = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout<<"Responding Time: "<< elapsedTime << "s" << endl;
        sprintf(sendBuff, "%g", (double)elapsedTime);
        cout << sendBuff << endl;
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);

        float secnds = elapsedTime/1000;
        
        fout<< recvBuff << endl;
        fout<<"Time: "<< secnds <<"s\n\n"<< endl;

        
        printf("Closed connection with esp32!\n");
        sleep(1);
        
        count++;
        
    }
    
}