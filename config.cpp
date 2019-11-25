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


int main(int argc, char* argv[]){

    ifstream Configfile;
    Configfile.open("cluster.conf", ios::in);

    string line;
    int numof_clusters, numof_grids, numofV_hashtables, numofV_hashtfuncts;
    int flag=0;
    int var_counter=1;

    while(Configfile.good()){
        if (!getline (Configfile, line)) break;
        string var="";

        for(auto x: line){
            if(flag==1){
                var=var+x;
                if(var_counter==1){
                    numof_clusters=stoi(var);
                }else if(var_counter==2){
                    numof_grids=stoi(var);
                }else if(var_counter==3){
                    numofV_hashtables=stoi(var);
                }else if(var_counter==4){
                    numofV_hashtfuncts=stoi(var);
                }
            }
            if(x==' ')  flag=1;
        }
        var_counter++;
        var="";
        flag=0;
    }
    
    cout << "Number of clusters=" << numof_clusters << endl;
    cout << "Numner of grids=" << numof_grids << endl;
    cout << "Number of vector hash tables=" << numofV_hashtables << endl;
    cout << "Number of vector hash functions=" << numofV_hashtfuncts << endl;
}
