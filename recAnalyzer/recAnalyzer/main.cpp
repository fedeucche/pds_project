 //
//  main.cpp
//  LogAnalyzer
//
//  Created by Terzuolo Simone on 15/05/18.
//  Copyright © 2018 Terzuolo Simone. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include "user.hpp"
#include <unistd.h>

using namespace std;



void analyzer(char filename[], string temp){
    //User *p[100];
    vector <unique_ptr<User> > people;

    string s;
    vector<double> ts;
    vector<int> rssi;
    vector<string> mac;
    vector<string>::iterator it;
    

    while(1){
        sprintf(filename, "./server/server/rec_%s.txt", temp.c_str());
        ifstream fin2 (filename, std::ios_base::in);
        

        while(getline(fin2, s)){
            // CICLO SU OGNI LINEA DEL FILE DI TESTO es. "-94,12841338,da:a1:19:93:81:96"
            // SPLIT PROB REQUEST
            
            string i;
            string word;
            stringstream stream(s);
            
            int count=0;
            
                
                while(getline(stream, word, ',')){
                // CICLO SU OGNI TOKEN es. "-94" "12841338" "da:a1:19:93:81:96"

                    switch (count) {
                        case 0:
                        // Token 0 : RSSI
                        rssi.push_back(stoi(word));
                        count++;
                        break;
                            case 1:
                            // Token 1 :TIMESTAMP
                            ts.push_back(stod(word));
                            count++;
                        break;
                            case 2:
                            // Token 2 :MAC
                            //cout << word << "\n\n";
                            it = find(mac.begin(), mac.end(), word);
                            if (it != mac.end())
                            {
                                auto index = distance(mac.begin(), it);
                                //cout << "MAC already in List\nAppending Probe Request to user Nr. "<< index<<"\n";
                                people.at(index)->append_prob(rssi.back(), ts.back());
                                //cout << "Numero di Probe Request inviate:"<< people.at(index)->get_n_prob()<<"\n\n"<<endl;
                            
                            
                                //Static _ p[index]->append_prob(rssi.back(), ts.back());
                                //cout << "Numero di Probe Request inviate:"<< p[index]->get_n_prob()<<"\n\n"<<endl;

                            }
                            else{
                                //cout << "Creating New User nr."<<(int) mac.size()<<" \n"<<endl;
                                mac.push_back(word);
                                // Istanzio nuova persona

                                people.push_back(unique_ptr<User>(new User(rssi.back(), ts.back(), mac.back())));
                                //p[(int) mac.size()-1]=new User (rssi.back(), ts.back(), mac.back());
        
                            }
                            count++;
                            break;
                        default:
                            break;
                    }
                    // fine switch
                    
                    }
            
        // Fine While token
            stringstream msg;
            // Fine While riga
            for (int per=0; per<mac.size(); per++)
            {
                
                msg<<temp.c_str()<<": Dispositivo nr: " <<per+1 <<": "<< people.at(per)->get_mac()<<" ha inviato "<<people.at(per)->get_n_prob()<<" Prob Request - Last RSSI : "<< people.at(per)->get_last_rssi()<<" \t Last Timestamp : "<< people.at(per)->get_last_ts()<<"ms"<<endl;
                //Static : cout<<"Dispositivo nr: " <<per <<": "<< p[per]->get_mac()<<" ha inviato "<<p[per]->get_n_prob()<<" Prob Request"<<endl;
                
            }
            msg<<"\n"<<endl;
            cout<< msg.str();
            sleep(0.7);
        }
    }
}

int main(int argc, const char * argv[]) {
    vector<string> mac_connected;
    ifstream myfile ("./server/server/mac_list.txt", std::ios_base::in);
    
    string temp;
    vector<thread> analyzer_threads;
    
    
    while(true){
        while(getline(myfile, temp))
        {
            mac_connected.push_back(temp);
            cout <<"ESP numero:"<<temp<<"\n"<<endl;
            
            char filename[60];
            
            sprintf(filename, "./server/server/rec_%s.txt", temp.c_str());
            analyzer_threads.push_back(thread(analyzer,filename,temp));
            
        }
            for_each(analyzer_threads.begin(), analyzer_threads.end(), mem_fn(&thread::join));
        //sleep(1);
    }

   
    return 0;
}
