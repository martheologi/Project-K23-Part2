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

void Random_Vector_Cetroids_Selection(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int num_of_centr){
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(0,Items.size());

    for(int i=0; i<num_of_centr; i++){
        int random_centr_pos = distribution(generator);
        centroids->push_back(Items.at(random_centr_pos));
    }
}

vector<Cluster> Lloyds_Assignment(int num_of_centr, int d, vector<Vector_Item> centroids, vector<Vector_Item> Items){
    vector<Cluster> clusters;
    for(int i=0; i<num_of_centr; i++){
        Cluster cl;
        cl.set_center_id(centroids.at(i).get_item_id());
        clusters.push_back(cl);
    }

    for(int i=0; i<Items.size(); i++){
        double min_dist = 1000000.00;
        int nearest_centre_pos = -1;

        for(int j=0; j<num_of_centr; j++){
            double dist = distance_l1(Items.at(i).get_vector(), centroids.at(j).get_vector(), d);
            //cout << dist << "\t";
            if(dist < min_dist){
                min_dist = dist;
                nearest_centre_pos = j;
            }
        }
        clusters.at(nearest_centre_pos).push_position(i);
        //cout<< endl << Items.at(i).get_item_id() << " to center " << centroids.at(nearest_centre_pos).get_item_id() << endl;
    }

    return clusters;
}

void K_means_pp(){
    
}
