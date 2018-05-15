//
//  user.cpp
//  LogAnalyzer
//
//  Created by Terzuolo Simone on 15/05/18.
//  Copyright © 2018 Terzuolo Simone. All rights reserved.
//

#include "user.hpp"

class User {
    
public:
    User( int rssi, int timestamp,string mac ){
        this->rssi.push_back(rssi);
        this->rssi.push_back(timestamp);
        this->mac=mac;
        
    }
    
    void append_prob(int rssi, int timestamp){
        this->rssi.push_back(rssi);
        this->rssi.push_back(timestamp);
    }

};
