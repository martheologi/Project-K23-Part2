#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>

#include "structs.h"
#include "hash.h"
#include "funct.h"
#include "cluster_funct.h"

int main(int argc, char* argv[]){

    int num_of_centr = 4;

    //diavazoume ta argv pou einai ta onomata twn arxeiwn
    string INfile=argv[2];
    string Cfile=argv[4];
    string OUTfile=argv[6];

    //anoigoume to cluster.conf kai pairnoume tis times twn metavlhtwn

    //anoigoume to input kai vlepoume an einai vectors h curves
    fstream file;
    string line;

    file.open(INfile);
    getline (file, line);
    file.close();

    //an einai vectors
    if(line == "vectors"){
        vector<Vector_Item> Items;
        int c = Initialize_Dataset_Vector(INfile, &Items);
        int d = Items.at(0).get_vector().size();
        cout << "Dataset with "<< c << " items" << endl;

        vector<Vector_Item> centroids;  //pinakas gia na krataw ta kentra

        //kaoume tis 8 periptwseis
        //1-1-2
        Random_Vector_Cetroids_Selection(&centroids, Items, num_of_centr);

        vector<Cluster> clusters;
        int flag = 0;

        while(1){
            vector<Cluster> temp_clusters = Lloids_Assignment(num_of_centr, d, centroids, Items);
            //update
            vector<Vector_Item> new_centroids;

            if(flag == 0){ ///// == 1 !!!!!!!!!!!!!!!!! to afhnw etsi gia na kanei break pros to paron
                clusters = temp_clusters;
                break;
            }
        }
        // for(int i=0; i<num_of_centr; i++){
        //     clusters.at(i).print_cluster();
        //     cout << endl;
        // }

    }

    //an einai curves
    else{

    }

    return 0;
}
