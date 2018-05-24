#include "user.hpp"

User::User(int rssi, double timestamp, string mac, string ssid, int seq_n){
    this->rssi.push_back(rssi);
    this->timestamp.push_back(timestamp);
    this->mac = mac;
    this->ssid.push_back(ssid);
    this->seq_n.push_back(seq_n);
}

void User::append_prob(int rssi, int timestamp, string ssid, int seq_n){
    this->rssi.push_back(rssi);
    this->timestamp.push_back(timestamp);
    this->ssid.push_back(ssid);
    this->seq_n.push_back(seq_n);
}

int User::get_n_prob(){
    return (int)rssi.size();
};

string User::get_mac(){
    return mac;
};

int User::get_last_rssi(){
    return (int)rssi.back();
};

double User::get_last_ts(){
    return (double)timestamp.back();
};