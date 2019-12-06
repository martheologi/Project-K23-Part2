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
#include "cluster_curve_funct.h"

//elegxw an kapoio cluster einai adeio kai an einai pairnw kainourio kentro tuxaia kai ksanakanw assign
vector<Cluster> Empty_Curve_cluster_check(int numof_clusters, vector<Cluster> temp_clusters, vector<Curve> centroids, vector<Curve> Curves_dataset){
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
        Curve_Random_Vector_Cetroids_Selection(&centroids, Curves_dataset, numof_clusters);
        // for(int j=0; j<numof_clusters; j++)
        //     cout << centroids.at(j).get_id() << endl;
        temp_clusters = Curves_Lloyds_Assignment(numof_clusters, centroids, Curves_dataset);
    }

    return temp_clusters;
}

int Equal_Curve_centroids(vector<Curve> c1, vector<Curve> c2, int numof_clusters){
    for(int i=0; i<numof_clusters; i++){
        if(!c1.at(i).equal(c2.at(i))) return 0;
    }
    return 1;
}

void Curve_Random_Vector_Cetroids_Selection(vector<Curve>* centroids, vector<Curve> Curves_dataset, int numof_clusters){
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(0,Curves_dataset.size()-1);

    for(int i=0; i<numof_clusters; i++){
        int random_centr_pos = distribution(generator);
        centroids->push_back(Curves_dataset.at(random_centr_pos));
    }
}

void Curve_K_means_pp(vector<Curve>* centroids, vector<Curve> Curves_dataset, int numof_clusters){
    int n = Curves_dataset.size();
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
            int m1 = Curves_dataset.at(i).get_points().size();
            int m2 = Curves_dataset.at(j).get_points().size();
            double **table = DTW(Curves_dataset.at(i),Curves_dataset.at(j));
            distances_array[i][j] = table[m1-1][m2-1];
            for(int t=0; t<m1+1; t++)
                delete[] table[t];
            delete[] table;
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
        centroids->push_back(Curves_dataset.at(min_pos));
    }

    for(int i=0; i<n; i++){
        delete[] distances_array[i];
    }
    delete[] distances_array;

    return;
}

vector<Cluster> Curves_Lloyds_Assignment(int numof_clusters, vector<Curve> centroids, vector<Curve> Curves_dataset){
    vector<Cluster> clusters;
    for(int i=0; i<numof_clusters; i++){
        Cluster cl;
        cl.set_center_id(centroids.at(i).get_id());
        clusters.push_back(cl);
    }

    for(int i=0; i<Curves_dataset.size(); i++){
        double min_dist = 1000000.00;
        int nearest_centre_pos = -1;

        for(int j=0; j<numof_clusters; j++){
            //an einai to kentro den to vazw
            if(centroids.at(j).equal(Curves_dataset.at(i))) continue;
            int m1 = Curves_dataset.at(i).get_points().size();
            int m2 = centroids.at(j).get_points().size();
            double **table = DTW(Curves_dataset.at(i),centroids.at(j));
            double dist = table[m1-1][m2-1];
            //double dist = distance_l1(Items.at(i).get_vector(), centroids.at(j).get_vector(), d);
            //cout << dist << "\t";
            if(dist < min_dist){
                min_dist = dist;
                nearest_centre_pos = j;
            }
            for(int t=0; t<m1+1; t++)
                delete[] table[t];
            delete[] table;
        }
        clusters.at(nearest_centre_pos).push_position(i);
        //cout<< endl << Items.at(i).get_item_id() << " to center " << centroids.at(nearest_centre_pos).get_item_id() << endl;
    }
    //checking if we have an empty cluster
    clusters = Empty_Curve_cluster_check(numof_clusters, clusters, centroids, Curves_dataset);

    return clusters;
}

vector<Cluster> Curve_Assignment_By_Range_Search(vector<Curve> centroids, vector<Curve> Curves_dataset, vector<Bucket>** HT, int numof_clusters, int numof_grids, int numofV_hashfuncts, int buckets, int W, int m, int M, int max_coord, int max_points){
    int c = Curves_dataset.size();

    vector<Cluster> clusters;
    for(int i=0; i<numof_clusters; i++){
        Cluster cl;
        cl.set_center_id(centroids.at(i).get_id());
        clusters.push_back(cl);
    }

    //pinakas gia na krataw to cluster pou mpainei to antistoixo dianusma
    int Curves_cluster[Curves_dataset.size()];
    for(int i=0; i<Curves_dataset.size(); i++){
        Curves_cluster[i] = -1;
    }
    //vriskw to key ka8e centroid
    int keys[numof_clusters];

    for(int i=0; i<numof_clusters; i++){
        Vector_Item center = grid_curve_vector(centroids.at(i), max_points, max_coord);
        int d = center.get_vector().size();
        //vriskw se poia bucket antistoixei to center
        for(int l=0; l<numof_grids; l++){
            keys[i] = hash_key(center, buckets, d, numofV_hashfuncts, W, M, m);
            if(HT[l]->at(keys[i]).get_point_pos().size() == 0) continue;

            int key_flag = 0;
            for(int k=0; k<i; k++){
              if(i == k) continue;
              if(keys[k] == keys[i])
              key_flag = 1;
            }
            //vazw to bucket sto cluster tou center
            for(int b=0; b<HT[l]->at(keys[i]).get_point_pos().size(); b++){
                //elegxw an to centroid antistoixei sto idio bucket me kapoio allo centroid
                //an anhkei se idio bucket den kanw tipota kai 8a ta valw meta me brutforce
                if(key_flag == 1) {
                    continue;
                    cout << "idia bucketttt "<<endl;
                }
                //an to dianusma den exei mpei se kapoio allo cluster
                if(Curves_cluster[HT[l]->at(keys[i]).get_point_pos().at(b)] == -1){
                    clusters.at(i).push_position(HT[l]->at(keys[i]).get_point_pos().at(b));
                    Curves_cluster[HT[l]->at(keys[i]).get_point_pos().at(b)] = i;
                }
            }
        }
    }
    //gia ta upoloipa dianusmata
    for(int n=0; n<Curves_dataset.size(); n++){
        double min_dist = 1000000.00;
        int nearest_centre_pos = -1;

        if(Curves_cluster[n] != -1)
            continue;

        Curve curve = Curves_dataset.at(n);

        for(int j=0; j<numof_clusters; j++){
            //an einai to kentro den to vazw
            if(centroids.at(j).equal(curve)) continue;
            int m1 = curve.get_points().size();
            int m2 = centroids.at(j).get_points().size();
            double **table = DTW(curve, centroids.at(j));
            double dist = table[m1-1][m2-1];
            //double dist = distance_l1(item.get_vector(), centroids.at(j).get_vector(), d);
            if(dist < min_dist){
                min_dist = dist;
                nearest_centre_pos = j;
            }
            for(int t=0; t<m1+1; t++)
                delete[] table[t];
            delete[] table;
        }
        clusters.at(nearest_centre_pos).push_position(n);
        Curves_cluster[n] = nearest_centre_pos;
    }

    clusters = Empty_Curve_cluster_check(numof_clusters, clusters, centroids, Curves_dataset);
    return clusters;
}

vector<Curve> Curves_PAM_alaLloyds(vector<Curve> centroids, vector<Cluster> temp_clusters, vector<Curve> Curves_dataset){
    vector<Curve> new_centroids;

    //gia ka8e cluster
    for(int c=0; c<temp_clusters.size(); c++){
        int min = 0;
        int T = temp_clusters.at(c).get_positions().size(); //to plh8os twn dianusmatwn sto cluster

        //upologizw to a8roisma tou kentrou apo ta shmeia
        for(int t=0; t<T; t++){
            Curve curve = Curves_dataset.at(temp_clusters.at(c).get_positions().at(t));
            int m1 = curve.get_points().size();
            int m2 = centroids.at(c).get_points().size();
            double **table = DTW(curve, centroids.at(c));
            min += table[m1-1][m2-1];
            for(int t=0; t<m1+1; t++)
                delete[] table[t];
            delete[] table;
        }
        int min_pos = -1;
        //gia ka8e dianusma sto cluster
        for(int t=0; t<T; t++){
            int sum = 0;
            Curve possibol_new_c = Curves_dataset.at(temp_clusters.at(c).get_positions().at(t));
            //vriskw to a8roisma twn apostasewn tou dianusmatos apo ta upoloipa dianusmata tou cluster
            for(int i=0; i<T; i++){
                if(t == i) continue;
                Curve curve = Curves_dataset.at(temp_clusters.at(c).get_positions().at(i));
                int m1 = curve.get_points().size();
                int m2 = possibol_new_c.get_points().size();
                double **table = DTW(curve, possibol_new_c);
                sum += table[m1-1][m2-1];
                for(int t=0; t<m1+1; t++)
                    delete[] table[t];
                delete[] table;
            }
            //pros8etw kai thn apostash apo to kentro pou exw twra kai den einai sto cluster
            int m1 = centroids.at(c).get_points().size();
            int m2 = possibol_new_c.get_points().size();
            double **table = DTW(centroids.at(c), possibol_new_c);
            sum += table[m1-1][m2-1];
            for(int t=0; t<m1+1; t++)
                delete[] table[t];
            delete[] table;
            //sum += distance_l1(possibol_new_c.get_vector(), centroids.at(c).get_vector(), d);

            if(sum < min){
                min = sum;
                min_pos = t;
            }
            if(sum == min){
                continue;
            }
        }
        if(min_pos != -1){
            Curve new_c = Curves_dataset.at(temp_clusters.at(c).get_positions().at(min_pos));
            new_centroids.push_back(new_c);
        }
        else{
            Curve new_c = centroids.at(c);
            new_centroids.push_back(new_c);
        }
    }

    return new_centroids;
}

double average_curve_distance(Curve curve, int cluster_id, vector<Cluster> clusters, vector<Curve> Curves_dataset, vector<Curve> centroids){
    double a = 0.0;
    double b = 0.0;

    //vriskw to meso oro twn apostasewn tou shmeiou apo ta upoloipa shmeia tou cluster sto opoio anhkei
    for(int i=0; i<clusters.at(cluster_id).get_positions().size(); i++){
        Curve tmp_curve = Curves_dataset.at(clusters.at(cluster_id).get_positions().at(i));
        int m1 = curve.get_points().size();
        int m2 = tmp_curve.get_points().size();
        double **table = DTW(curve, tmp_curve);
        a += table[m1-1][m2-1];
        for(int t=0; t<m1+1; t++)
            delete[] table[t];
        delete[] table;
    }
    a = a/clusters.at(cluster_id).get_positions().size();

    //vriskw to epomeno kontinotero kentro
    long int min = 1000000;
    int second_centroid = -1;
    for(int i=0; i<centroids.size(); i++){
        if(i == cluster_id) continue;
        int m1 = curve.get_points().size();
        int m2 =  centroids.at(i).get_points().size();
        double **table = DTW(curve, centroids.at(i));

        int dist = table[m1-1][m2-1];//distance_l1(item.get_vector(), centroids.at(i).get_vector(), d);
        if(dist < min){
            min = dist;
            second_centroid = i;
        }
        for(int t=0; t<m1+1; t++)
            delete[] table[t];
        delete[] table;
    }
    //vriskw to meso oro twn apostasewn tou shmeiou apo ta upoloipa shmeia tou epomenou kontinoterou cluster
    for(int i=0; i<clusters.at(second_centroid).get_positions().size(); i++){
        Curve tmp_curve = Curves_dataset.at(clusters.at(second_centroid).get_positions().at(i));
        int m1 = curve.get_points().size();
        int m2 = tmp_curve.get_points().size();
        double **table = DTW(curve, tmp_curve);
        b += table[m1-1][m2-1];
        for(int t=0; t<m1+1; t++)
            delete[] table[t];
        delete[] table;
    }
    b = b/clusters.at(second_centroid).get_positions().size();

    double si = (b - a)/(max(a,b));

    return si;
}

vector<double> Curve_Silhouette(vector<Cluster> clusters, vector<Curve> Curves_dataset, vector<Curve> centroids, double* stotal){
    vector<double> s;

    //gia ka8e cluster vriskw to average s(p) of points
    for(int c=0; c<centroids.size(); c++){
        double sp = 0.0;

        for(int i=0; i<clusters.at(c).get_positions().size(); i++){
            Curve curve = Curves_dataset.at(clusters.at(c).get_positions().at(i));
            double si = average_curve_distance(curve, c, clusters, Curves_dataset, centroids);
            sp += si;
        }

        sp = sp/clusters.at(c).get_positions().size();
        *stotal += sp;
        s.push_back(sp);
    }

    *stotal = *stotal/centroids.size();

    return s;
}
