//
//  user.hpp
//  LogAnalyzer
//
//  Created by Terzuolo Simone on 15/05/18.
//  Copyright © 2018 Terzuolo Simone. All rights reserved.
//

#ifndef user_hpp
#define user_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;


class User {
    vector<int> rssi, timestamp;
    string mac;
public:
    User( int rssi, int timestamp,string mac );
    
    void append_prob(int rssi, int timestamp);
    int get_n_prob();
    string get_mac();
    
};

#endif /* user_hpp */