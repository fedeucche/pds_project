#ifndef user_hpp
#define user_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;


class User {
    vector<int> rssi, timestamp, seq_n;
    vector<string> ssid;
    string mac;
public:
    User(int rssi, double timestamp, string mac, string ssid, int seq_n);
    void append_prob(int rssi, int timestamp, string ssid, int seq_n);
    int get_n_prob();
    string get_mac();
    int get_last_rssi();
    double get_last_ts();

};

#endif /* user_hpp */