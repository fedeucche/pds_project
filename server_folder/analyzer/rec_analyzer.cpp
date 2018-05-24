#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include "user.hpp"
#include <unistd.h>
#include <functional>

using namespace std;

void analyzer(char filename[], string temp){

    vector<unique_ptr<User>> people;
    string s;
    double ts;
    int rssi;
    int seq_n;
    vector<string> mac;
    vector<string>::iterator it;
    string ssid;
    ifstream probe_list (filename, std::ios_base::in);

    next_line: while(getline(probe_list, s)){
        // CICLO SU OGNI LINEA DEL FILE DI TESTO es. "-94,12841338,da:a1:19:93:81:96"
        // SPLIT PROB REQUEST
        
        string i;
        string word;
        stringstream stream(s);
        int count=0;
        
        while(getline(stream, word, ',')){
        // CICLO SU OGNI TOKEN es. "-94" "12841338" "da:a1:19:93:81:96"
            switch (count){
                case 0:
                    // Token 0: RSSI
                    rssi = stoi(word);
                    count++;
                    break;
                case 1:
                    // Token 1: TIMESTAMP
                    ts = stod(word);
                    count++;
                    break;
                case 2:
                	// Token 2: SEQUENCE NUMBER
                	seq_n = stoi(word);
                	count++;
                	break;
                case 3:
                	// Token 3: SSID
                	ssid = word;
                	count++;
                	break;
                case 4:
                    // Token 4: MAC
                    it = find(mac.begin(), mac.end(), word);
                    if(it != mac.end()){
                        auto index = distance(mac.begin(), it);
                        people.at(index)->append_prob(rssi, ts, ssid, seq_n);
                    }
                    else{
                        mac.push_back(word);
                        // Istanzio nuova persona
                        people.push_back(unique_ptr<User>(new User(rssi, ts, mac.back(), ssid, seq_n)));
                    }
                    count++;
                    break;
                default:
                    break;
            }
            // fine switch    
        }
        // fine while per riga
    }
    // fine while intero file fin'ora

    if(probe_list.eof()){
    	for(int x=0;x<20;x++){
    		cout << x << endl;
    		sleep(1);
    	}
    	probe_list.clear();
    	goto next_line;
    }

}

int main(){

    ifstream mac_list ("mac_list.txt", std::ios_base::in);
    string temp;
    vector<thread> analyzer_threads;
    int i = 0;

    next_line: while(getline(mac_list, temp)){
        cout << "ESP numero:" << temp << "\n" << endl;
        char filename[60];
        sprintf(filename, "rec_%s.txt", temp.c_str());
        analyzer_threads.push_back(thread(analyzer, filename, temp));
    }

    if(mac_list.eof()){
    	sleep(20);
    	mac_list.clear();
    	goto next_line;
    }

    for_each(analyzer_threads.begin(), analyzer_threads.end(), mem_fn(&thread::join));

    return 0;
}