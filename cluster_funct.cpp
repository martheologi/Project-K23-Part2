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

int range_finder(vector<Vector_Item> centroids){
    int dist;
    int d = centroids.at(0).get_vector().size();
    long int min = 1000000;

    for(int i=0; i<centroids.size(); i++){
        for(int j=i+1; j<centroids.size(); j++){
            dist = distance_l1(centroids.at(i).get_vector(), centroids.at(j).get_vector(), d);
            if(dist < min){
                min = dist;
            }
        }
    }
    return min/2;
}

int Equal_centroids(vector<Vector_Item> c1, vector<Vector_Item> c2, int numof_clusters){
    int d = c1.at(0).get_vector().size();
    for(int i=0; i<numof_clusters; i++){
        for(int x=0; x<d; x++){
            if(c1.at(i).get_vector().at(x) != c2.at(i).get_vector().at(x)) return 0;
        }
    }
    return 1;
}

void Random_Vector_Cetroids_Selection(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters){
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(0,Items.size());

    for(int i=0; i<numof_clusters; i++){
        int random_centr_pos = distribution(generator);
        centroids->push_back(Items.at(random_centr_pos));
    }
}

vector<Cluster> Lloyds_Assignment(int numof_clusters, int d, vector<Vector_Item> centroids, vector<Vector_Item> Items){
    vector<Cluster> clusters;
    for(int i=0; i<numof_clusters; i++){
        Cluster cl;
        cl.set_center_id(centroids.at(i).get_item_id());
        clusters.push_back(cl);
    }

    for(int i=0; i<Items.size(); i++){
        double min_dist = 1000000.00;
        int nearest_centre_pos = -1;

        for(int j=0; j<numof_clusters; j++){
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

void K_means_pp(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters){
    int n = Items.size();
    int d = Items.at(0).get_vector().size();
    int distances_array[n][n];

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i==j){
                distances_array[i][j] = 0;
                continue;
            }
            distances_array[i][j] = distance_l1(Items.at(i).get_vector(), Items.at(j).get_vector(), d);
        }
    }

    double v[n];

    for(int i=0; i<n; i++){
        double d1 = 0.0;
        for(int j=0; j<n; j++){
            double d2 = 0.0;
            if( i == j) continue;
            for(int t=0; t<n; t++){
                if( t == j) continue;
                d2 += distances_array[j][t];
            }
            d1 += (distances_array[i][j] / d2);
        }
        v[i] = d1;
    }
    for(int k=0; k<numof_clusters; k++){
        //cout << v[i] << endl;
        long int min = 1000000;
        int max = 0;
        int min_pos = -1;
        for(int i=0; i<n; i++){
            if(v[i] < min){
                min = v[i];
                min_pos = i;
            }
            if(v[i] > max) max = v[i];
        }
        v[min_pos] = max;
        centroids->push_back(Items.at(min_pos));
    }
    return;
}

vector<Cluster> Assignment_By_Range_Search(vector<Vector_Item> centroids, vector<Vector_Item> Items, int numof_clusters, int numofV_hashtables, int numofV_hashfuncts, int buckets, int W, int m, int M){
    int c = Items.size();
    vector<Cluster> clusters;
    vector<Bucket>** HT = new vector<Bucket>* [numofV_hashtables];

    for(int l=0; l<numofV_hashtables; l++){
        HT[l] = new vector<Bucket>;
        for(int i=0; i<buckets; i++){
            Bucket b;
            b.set_key(i);
            HT[l]->push_back(b);
        }
    }
    //HASHIIIIIING
    //gia ka8e dianusma tou dataset
    for(int n=0; n<c; n++){
        Vector_Item item = Items.at(n);
        int d = item.get_vector().size();

        for(int l=0; l<numofV_hashtables; l++){
            int key = hash_key(item, buckets, d, numofV_hashfuncts, W, M, m);
            //to vazw sto hash table
            if((key<0) || (key>=buckets)) continue;
            HT[l]->at(key).push_pos(n);
        }
    }

    int range = range_finder(centroids);
    cout << "range" << range << endl;

    for(int i=0; i<numof_clusters; i++){
        int AprNN_dist;
        Vector_Item NN_item = AproximateNN(Items, centroids.at(i), HT, buckets, numofV_hashfuncts, numofV_hashtables, m, M, W, &AprNN_dist);
        cout << AprNN_dist << endl;
    }

    return clusters;
}

vector<Vector_Item> Mean_Vector_Update(vector<Cluster> temp_clusters, vector<Vector_Item> Items){
    vector<Vector_Item> new_centroids;
    int d = Items.at(0).get_vector().size();

    //gia ka8e cluster
    for(int i=0; i<temp_clusters.size(); i++){
        int T = temp_clusters.at(i).get_positions().size(); //to plh8os twn dinusmatwn sto cluster
        Vector_Item new_c;
        new_c.set_id("new_c");
        //gia ka8e xi
        for(int x=0; x<d; x++){
            //gia ka8e xi tou ka8e dianusmatos tou cluster
            int V = 0;
            for(int j=0; j<T; j++){
                V += Items.at(temp_clusters.at(i).get_positions().at(j)).get_vector().at(x);  ///aparadektos kwdikas mh me rwtatex den katalavainw
            }
            new_c.push(V/T);
        }
        new_centroids.push_back(new_c);
    }
    return new_centroids;
}
