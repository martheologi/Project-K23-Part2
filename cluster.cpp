#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>

#include "structs.h"
#include "hash.h"
#include "funct.h"
#include "cluster_funct.h"
#include "curve_funct.h"
#include "cluster_curve_funct.h"

int main(int argc, char* argv[]){

    //diavazoume ta argv pou einai ta onomata twn arxeiwn
    string INfile=argv[2];
    string Cfile=argv[4];
    string OUTfile=argv[6];
    int complete;
    if(argc == 9)
        complete = stoi(argv[8]);
    else
        complete = 0;

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
                    vector<Cluster> clusters;
                    vector<Bucket>** HT = NULL;
                    if(a==2)
                        HT =  HT_initialize(numofV_hashtables, buckets, c, Items, numofV_hashfuncts, W, M, m);

                    std::clock_t start;
                    start = std::clock();

                    //Initialization
                    if(i == 1)
                        Random_Vector_Cetroids_Selection(&centroids, Items, numof_clusters);
                    else if(i == 2)
                        K_means_pp(&centroids, Items, numof_clusters);

                    int flag = 0;

                    int it = 0;
                    while(1){
                        vector<Cluster> temp_clusters;
                        vector<Vector_Item> new_centroids;

                        //Assignment
                        if(a == 1)
                            temp_clusters = Lloyds_Assignment(numof_clusters, d, centroids, Items);
                        else if(a == 2){
                            it++;
                            temp_clusters = Assignment_By_Range_Search(centroids, Items, HT, numof_clusters, numofV_hashtables, numofV_hashfuncts, buckets, W, m, M);
                        }
                        //Update
                        if(u == 1)
                            new_centroids = PAM_alaLloyds(centroids, temp_clusters, Items);
                        else if(u == 2)
                            new_centroids = Mean_Vector_Update(temp_clusters, Items);

                        if(Equal_centroids(centroids, new_centroids, numof_clusters)) flag = 1;
                        if((flag == 1) || (it == 10)){
                            clusters = temp_clusters;
                            break;
                        }
                        centroids = new_centroids;
                    }
                    double time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

                    double stotal = 0.0;
                    vector<double> s = Silhouette(clusters, Items, centroids, &stotal);

                    write_results(OUTfile, i, a, u, numof_clusters, clusters, centroids, time, complete, s, stotal);

                    centroids.clear();
                    clusters.clear();
                    if(HT != NULL){
                        for(int l=0; l<numofV_hashtables; l++){
                            delete HT[l];
                        }
                        delete[] HT;
                    }
                }
            }
        }
    }

    //an einai curves
    else{
        vector<Curve> Curves_dataset;
        int max_points = 0;
        double max_coord = 0;

        int c = Initialize_Curve_Dataset(INfile, &Curves_dataset, &max_points, &max_coord);
        int buckets = c/8;
        cout << "Dataset with "<< c << " curves" << endl;

        //kanoume tis 8 periptwseis
        for(int i=1; i<3; i++){
            for(int a=1; a<3; a++){
                for(int u=1; u<3; u++){
                    cout << "I"<< i << "-A" << a << "-U" << u << endl;

                    vector<Curve> centroids;  //pinakas gia na krataw ta kentra
                    vector<Cluster> clusters;
                    vector<Bucket>** HT = NULL;
                    if(a==2)
                        HT = Curves_HT_initialize(numof_grids, buckets, c, W, M, m, numofV_hashfuncts, max_coord , max_points, Curves_dataset);

                    std::clock_t start;
                    start = std::clock();

                    //Initialization
                    if(i == 1)
                        Curve_Random_Vector_Cetroids_Selection(&centroids, Curves_dataset, numof_clusters);
                    else if(i == 2)
                        Curve_K_means_pp(&centroids, Curves_dataset, numof_clusters);

                    int flag = 0;
                    int it = 0;

                    while(1){
                        vector<Cluster> temp_clusters;
                        vector<Curve> new_centroids;

                        //Assignment
                        if(a == 1)
                            temp_clusters = Curves_Lloyds_Assignment(numof_clusters, centroids, Curves_dataset);
                        else if(a == 2)
                            temp_clusters = Curve_Assignment_By_Range_Search(centroids, Curves_dataset, HT, numof_clusters, numof_grids, numofV_hashfuncts, buckets, W, m, M, max_coord, max_points);

                        //Update
                        if(u == 1)
                            new_centroids = Curves_PAM_alaLloyds(centroids, temp_clusters, Curves_dataset);
                        else if(u == 2)
                            new_centroids = Curve_Mean_Vector_Update(temp_clusters, Curves_dataset);

                        if(Equal_Curve_centroids(centroids, new_centroids, numof_clusters)) flag = 1;

                        if((flag == 1) || (it == 20)){
                            clusters = temp_clusters;
                            break;
                        }
                        centroids = new_centroids;
                        it++;
                    }

                    double time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

                    if(clusters.size() != 0){
                        double stotal = 0.0;
                        vector<double> s = Curve_Silhouette(clusters, Curves_dataset, centroids, &stotal);

                        write_curve_results(OUTfile, i, a, u, numof_clusters, clusters, centroids, time, complete, s, stotal);
                    }

                    centroids.clear();
                    clusters.clear();
                    if(HT != NULL){
                        for(int l=0; l<numof_grids; l++){
                            delete HT[l];
                        }
                        delete[] HT;
                    }
                }
            }
        }
    }

    return 0;
}
