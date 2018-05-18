//
//  user.cpp
//  LogAnalyzer
//
//  Created by Marrae on 15/05/18.
//  Copyright © 2018 Terzuolo Simone. All rights reserved.
//

#include "user.hpp"

User::User( int rssi, double timestamp,string mac ){
    this->rssi.push_back(rssi);
    this->timestamp.push_back(timestamp);
    this->mac=mac;
    
}

void User::append_prob(int rssi, int timestamp){
    this->rssi.push_back(rssi);
    this->timestamp.push_back(timestamp);
}

int User::get_n_prob()
{
    return (int) rssi.size();
};

string User::get_mac(){
    return mac;
};

int User::get_last_rssi()
{
    return (int) rssi.back();
};

double User::get_last_ts()
{
    return (double) timestamp.back();
};

