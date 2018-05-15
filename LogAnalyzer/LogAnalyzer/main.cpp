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

void analyzer(char filename[], string temp){
    
    string s;
    sprintf(filename, "/Users/Simone/Desktop/Server/rec_%s.txt", temp.c_str());
    ifstream fin2 (filename, std::ios_base::in);

    while( getline( fin2, s) )
    {
        cout <<s<<endl;
        
    }



}

int main(int argc, const char * argv[]) {
        vector<string> mac_connected;
        ifstream myfile ("/Users/Simone/Desktop/Server/mac_list.txt", std::ios_base::in);
        string temp;
        int i=0;
      while( getline( myfile, temp ) )
        {
            mac_connected.push_back( temp );
            cout <<temp<<endl;
            
            char filename[60];
            
            sprintf(filename, "/Users/Simone/Desktop/Server/rec_%s.txt", temp.c_str());
            analyzer(filename,temp);

            i++;

        }
    

    
    
    return 0;
}
