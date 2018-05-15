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
    User( int rssi, int timestamp,string mac );
    
    
    void append_prob(int rssi, int timestamp);
};
