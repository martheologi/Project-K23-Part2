#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <cstring>
#include <string.h>
#include <iterator>
#include <cmath>
#include <bits/stdc++.h>
#include <bitset>

using namespace std;


void write_results(int I, int A, int U, int k, vector<Cluster> clusters, double cltime){

    ofstream Outfile;
    Outfile.open("output.txt", ios::app);

    string line;

    Outfile << "Algorithm: I" << I << "A" << A << "U" << U << endl;

    for(int i=0;i<k;i++){
        Outfile << "CLUSTER-" << i+1 << " {size: " << clusters.at(i).get_possitions().size() << ", centroid: " << clusters.at(i).get_center_id() << "}" << endl;
    }

    Outfile << "clustering_time: " << cltime << endl;

    Outfile.close();

    return;
}

int main(){
    write_results(1, 1, 2, 5);
}
