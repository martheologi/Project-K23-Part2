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

    //diavazoume ta argv pou einai ta onomata twn arxeiwn
    string INfile=argv[2];
    string Cfile=argv[4];
    string OUTfile=argv[6];
    int numof_clusters, numof_grids, numofV_hashtables, numofV_hashfuncts;

    //anoigoume to cluster.conf kai pairnoume tis times twn metavlhtwn
    configuration(Cfile, &numof_clusters, &numof_grids, &numofV_hashtables, &numofV_hashfuncts);
    double W = 3000;
    long int m = pow(2, 32) - 5;
    int M = pow(2, (32/numofV_hashfuncts));

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
        int buckets = c/8;
        cout << "Dataset with "<< c << " items" << endl;

        vector<Vector_Item> centroids;  //pinakas gia na krataw ta kentra

        //kanoume tis 8 periptwseis
        //1-1-2
        Random_Vector_Cetroids_Selection(&centroids, Items, numof_clusters);
        for(int i=0; i<numof_clusters; i++)
            cout << centroids.at(i).get_item_id() << endl;

        vector<Cluster> clusters;
        int flag = 0;

        while(1){
            vector<Cluster> temp_clusters = Lloyds_Assignment(numof_clusters, d, centroids, Items);
            //update
            vector<Vector_Item> new_centroids = Mean_Vector_Update(temp_clusters, Items);

            if(Equal_centroids(centroids, new_centroids, numof_clusters)) flag = 1;

            if(flag == 1){ ///// == 1 !!!!!!!!!!!!!!!!! to afhnw etsi gia na kanei break pros to paron
                clusters = temp_clusters;
                break;
            }
            centroids = new_centroids;
        }
        for(int i=0; i<numof_clusters; i++){
            clusters.at(i).print_cluster();
            cout << endl;
        }

        //2-2-2
        vector<Vector_Item> centroids2;
        K_means_pp(&centroids2, Items, numof_clusters);
        cout << endl;
        for(int i=0; i<numof_clusters; i++)
            cout << centroids2.at(i).get_item_id() << endl;

            vector<Cluster> clusters2;
            flag = 0;

            while(1){
                vector<Cluster> temp_clusters = Assignment_By_Range_Search(centroids, Items, numof_clusters, numofV_hashtables, numofV_hashfuncts, buckets, W, m, M);
                //update
                vector<Vector_Item> new_centroids;

                if(flag == 0){ ///// == 1 !!!!!!!!!!!!!!!!! to afhnw etsi gia na kanei break pros to paron
                    clusters = temp_clusters;
                    break;
                }
            }
            // for(int i=0; i<numof_clusters; i++){
            //     clusters.at(i).print_cluster();
            //     cout << endl;
            // }
    }

    //an einai curves
    else{

    }

    return 0;
}
