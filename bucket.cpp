#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <random>
#include <cmath>
#include <bits/stdc++.h>

#include "structs.h"

Bucket::Bucket(){}

Bucket::~Bucket(){}

void Bucket::set_key(int k){
    key = k;
}

void Bucket::push_pos(int pos){
    //cout << "pushing " << pos<< "\t";
    point_pos.push_back(pos);
}

int Bucket::get_key(){
    return key;
}

vector<int> Bucket::get_point_pos(){
    return point_pos;
}

void Bucket::print(){
    cout << "Bucket " << key << " -----> ";
    for(int i=0; i<point_pos.size(); i++){
        cout << point_pos.at(i) << " ";
    }
    cout << endl;
}
