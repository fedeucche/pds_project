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
#endif /* user_hpp */

using namespace std;


class User {
    vector<int> rssi, timestamp;
    string mac;
public:
    User( int rssi, double timestamp,string mac );
    
    
    void append_prob(int rssi, int timestamp);
    int get_n_prob();
    string get_mac();
    int get_last_rssi();
    double get_last_ts();

};
