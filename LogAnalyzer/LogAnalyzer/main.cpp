//
//  main.cpp
//  LogAnalyzer
//
//  Created by Terzuolo Simone on 15/05/18.
//  Copyright © 2018 Terzuolo Simone. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
        vector<string> mac_connected;
        ifstream myfile ("/Users/Simone/Desktop/Server/mac_list.txt", std::ios_base::in);
        string temp;
        ifstream fin[4];
        int i=0;
      while( getline( myfile, temp ) )
        {
            mac_connected.push_back( temp );
            cout <<temp<<endl;
            
            char filename[60];
            
            sprintf(filename, "/Users/Simone/Desktop/Server/rec_%s.txt", temp.c_str());
            fin[i].open(filename);
            

            i++;

        }
    
    string s;
    while( getline( fin[0], s) )
    {
        cout <<s<<endl;
    
    }

    
    
    return 0;
}
