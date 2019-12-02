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
#include "curve_funct.h"

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


        //kanoume tis 8 periptwseis
        for(int i=1; i<3; i++){
            for(int a=1; a<3; a++){
                for(int u=1; u<3; u++){
                    cout << "I"<< i << "-A" << a << "-U" << u << endl;

                    vector<Vector_Item> centroids;  //pinakas gia na krataw ta kentra
                    //Initialization
                    if(i == 1)
                        Random_Vector_Cetroids_Selection(&centroids, Items, numof_clusters);
                    else if(i == 2)
                        K_means_pp(&centroids, Items, numof_clusters);
                    // for(int i=0; i<numof_clusters; i++)
                    //     cout << centroids.at(i).get_item_id() << endl;

                    vector<Cluster> clusters;
                    int flag = 0;

                    while(1){
                        vector<Cluster> temp_clusters;
                        vector<Vector_Item> new_centroids;

                        //Assignment
                        //if(a == 1)
                            temp_clusters = Lloyds_Assignment(numof_clusters, d, centroids, Items);
                        //else if(a == 2)
                            //temp_clusters = Assignment_By_Range_Search(centroids, Items, numof_clusters, numofV_hashtables, numofV_hashfuncts, buckets, W, m, M);
                        //Update
                        if(u == 1)
                            new_centroids = Mean_Vector_Update(temp_clusters, Items);
                        else if(u == 2)
                            new_centroids = PAM_alaLloyds(centroids, temp_clusters, Items);

                        if(Equal_centroids(centroids, new_centroids, numof_clusters)) flag = 1;

                        if(flag == 1){
                            clusters = temp_clusters;
                            break;
                        }
                        centroids = new_centroids;
                    }
                    for(int i=0; i<numof_clusters; i++){
                        clusters.at(i).print_cluster();
                        cout << endl;
                    }

                    centroids.clear();
                    clusters.clear();
                }
            }
        }
    }

    //an einai curves
    else{

    }

    return 0;
}
