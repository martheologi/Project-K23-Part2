#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>

#include "structs.h"

Cluster::Cluster(){}

Cluster::~Cluster(){}

void Cluster::set_center_id(string id){
    center_id = id;
}

void Cluster::push_position(int pos){
    positions.push_back(pos);
}

void Cluster::print_cluster(){
    cout << "Cluster with center " << center_id << " -----> ";
    for(int i=0; i<positions.size(); i++)
        cout << positions.at(i) << " ";
    cout << endl;
}

string Cluster::get_center_id(){
    return center_id;
}

vector<int> Cluster::get_positions(){
    return positions;
}
