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

//elegxw an kapoio cluster einai adeio kai an einai pairnw kainourio kentro tuxaia kai ksanakanw assign
vector<Cluster> Empty_cluster_check(int numof_clusters, vector<Cluster> temp_clusters, vector<Vector_Item> centroids, int d, vector<Vector_Item> Items){
    int empty_pos = -1;
    for(int i=0; i<numof_clusters; i++){
        if(temp_clusters.at(i).get_positions().size()==0){
            //cout << "empty cluster" << endl;
            empty_pos = i;
            break;
        }
    }
    if(empty_pos != -1){
        centroids.clear();
        temp_clusters.clear();
        Random_Vector_Cetroids_Selection(&centroids, Items, numof_clusters);
        // for(int j=0; j<numof_clusters; j++)
        //     cout << centroids.at(j).get_item_id() << endl;
        temp_clusters = Lloyds_Assignment(numof_clusters, d, centroids, Items);
    }

    return temp_clusters;
}

void Random_Vector_Cetroids_Selection(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters){
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(0,Items.size()-1);

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
            //an einai to kentro den to vazw
            if(centroids.at(j).equal(Items.at(i))) continue;
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
    //checking if we have an empty cluster
    clusters = Empty_cluster_check(numof_clusters, clusters, centroids, d, Items);

    return clusters;
}

void K_means_pp(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters){
    int n = Items.size();
    int d = Items.at(0).get_vector().size();
    double** distances_array = new double* [n];

    for(int i=0; i<n; i++){
        distances_array[i] = new double [n];
    }

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

    for(int i=0; i<n; i++){
        delete[] distances_array[i];
    }
    delete[] distances_array;

    return;
}

vector<Cluster> Assignment_By_Range_Search(vector<Vector_Item> centroids, vector<Vector_Item> Items, vector<Bucket>** HT, int numof_clusters, int numofV_hashtables, int numofV_hashfuncts, int buckets, int W, int m, int M){

    int c = Items.size();
    int d = centroids.at(0).get_vector().size();

    vector<Cluster> clusters;
    for(int i=0; i<numof_clusters; i++){
        Cluster cl;
        cl.set_center_id(centroids.at(i).get_item_id());
        clusters.push_back(cl);
    }

    //pinakas gia na krataw to cluster pou mpainei to antistoixo dianusma
    int Items_cluster[Items.size()];
    for(int i=0; i<Items.size(); i++){
        Items_cluster[i] = -1;
    }
    //vriskw to key ka8e centroid
    int keys[numof_clusters];

    for(int i=0; i<numof_clusters; i++){
        Vector_Item center = centroids.at(i);
        //vriskw se poia bucket antistoixei to center
        for(int l=0; l<numofV_hashtables; l++){
            keys[i] = hash_key(center, buckets, d, numofV_hashfuncts, W, M, m);
            //vazw to bucket sto cluster tou center
            for(int b=0; b<HT[l]->at(keys[i]).get_point_pos().size(); b++){
                //elegxw an to centroid antistoixei sto idio bucket me kapoio allo centroid
                int key_flag = 0;
                for(int k=0; k<numof_clusters; k++){
                    if(i == k) continue;
                    if(keys[k] == keys[i])
                        key_flag = 1;
                }
                //an anhkei se idio bucket den kanw tipota kai 8a ta valw meta me brutforce
                if(key_flag == 1) {
                    continue;
                    cout << "idia bucketttt "<<endl;
                }
                //an to dianusma den exei mpei se kapoio allo cluster
                if(Items_cluster[HT[l]->at(keys[i]).get_point_pos().at(b)] == -1){
                    clusters.at(i).push_position(HT[l]->at(keys[i]).get_point_pos().at(b));
                    Items_cluster[HT[l]->at(keys[i]).get_point_pos().at(b)] = i;
                }
            }
        }
    }
    //gia ta upoloipa dianusmata
    for(int n=0; n<Items.size(); n++){
        double min_dist = 1000000.00;
        int nearest_centre_pos = -1;

        if(Items_cluster[n] != -1)
            continue;

        Vector_Item item = Items.at(n);

        for(int j=0; j<numof_clusters; j++){
            //an einai to kentro den to vazw
            if(centroids.at(j).equal(item)) continue;
            double dist = distance_l1(item.get_vector(), centroids.at(j).get_vector(), d);
            if(dist < min_dist){
                min_dist = dist;
                nearest_centre_pos = j;
            }
        }
        clusters.at(nearest_centre_pos).push_position(n);
        Items_cluster[n] = nearest_centre_pos;
    }

    clusters = Empty_cluster_check(numof_clusters, clusters, centroids, d, Items);
    //
    // for(int i=0; i<numof_clusters; i++){
    //     clusters.at(i).print_cluster();
    //     cout << endl;
    // }
    return clusters;
}

vector<Vector_Item> Mean_Vector_Update(vector<Cluster> temp_clusters, vector<Vector_Item> Items){
    vector<Vector_Item> new_centroids;
    int d = Items.at(0).get_vector().size();

    //gia ka8e cluster
    for(int i=0; i<temp_clusters.size(); i++){
        int T = temp_clusters.at(i).get_positions().size(); //to plh8os twn dianusmatwn sto cluster
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

vector<Vector_Item> PAM_alaLloyds(vector<Vector_Item> centroids, vector<Cluster> temp_clusters, vector<Vector_Item> Items){
    vector<Vector_Item> new_centroids;
    int d = Items.at(0).get_vector().size();

    //gia ka8e cluster
    for(int c=0; c<temp_clusters.size(); c++){
        int min = 0;
        int T = temp_clusters.at(c).get_positions().size(); //to plh8os twn dianusmatwn sto cluster

        //upologizw to a8roisma tou kentrou apo ta shmeia
        for(int t=0; t<T; t++){
            Vector_Item item = Items.at(temp_clusters.at(c).get_positions().at(t));
            min += distance_l1(centroids.at(c).get_vector(), item.get_vector(), d);
        }
        int min_pos = -1;
        //gia ka8e dianusma sto cluster
        for(int t=0; t<T; t++){
            int sum = 0;
            Vector_Item possibol_new_c = Items.at(temp_clusters.at(c).get_positions().at(t));
            //vriskw to a8roisma twn apostasewn tou dianusmatos apo ta upoloipa dianusmata tou cluster
            for(int i=0; i<T; i++){
                if(t == i) continue;
                Vector_Item item = Items.at(temp_clusters.at(c).get_positions().at(i));
                sum += distance_l1(possibol_new_c.get_vector(), item.get_vector(), d);
            }
            //pros8etw kai thn apostash apo to kentro pou exw twra kai den einai sto cluster
            sum += distance_l1(possibol_new_c.get_vector(), centroids.at(c).get_vector(), d);

            if(sum < min){
                min = sum;
                min_pos = t;
            }
            if(sum == min){
                continue;
            }
        }
        if(min_pos != -1){
            Vector_Item new_c = Items.at(temp_clusters.at(c).get_positions().at(min_pos));
            new_centroids.push_back(new_c);
        }
        else{
            Vector_Item new_c = centroids.at(c);
            new_centroids.push_back(new_c);
        }
    }

    return new_centroids;
}
