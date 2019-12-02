#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <bits/stdc++.h>

#include "structs.h"
#include "funct.h"
#include "hash.h"
#include "curve_funct.h"

Curve Initialize_Curve(string line, double* max_coord){
    Curve curve;
    int id_flag = 1;
    int m_flag = 2;
    string digits = "";

    for (auto x : line){
        Point p;
        if (x == '\t'){
            if(id_flag == 1){
              curve.set_id(digits);
              id_flag = 0;
              m_flag--;
            }
            if(m_flag == 1){
                curve.set_m(stoi(digits));
            }
            digits = "";
        }
        else if(x == '('){
            digits = "";
        }
        else if(x == ','){
            p.set_x(stod(digits));
            if(*max_coord < p.get_x())
                *max_coord = p.get_x();
            digits = "";
        }
        else if(x == ')'){
            p.set_y(stod(digits));
            if(*max_coord < p.get_y())
                *max_coord = p.get_y();
            curve.push_point(p);
            digits = "";
        }
        else{
            digits = digits + x;
        }
    }
    return curve;
}

int Initialize_Curve_Dataset(string filename, vector<Curve>* Curves_dataset, int* max_points, double* max_coord){
    string digits = "";
    string line;
    int c = 0;
    fstream file;

    file.open(filename);

    // extracting from the file
    while (file.good())
    //for(int i=0; i<100; i++)
    {
        if (!getline (file, line)) break;
        c++;

        Curve curve = Initialize_Curve(line, max_coord);

        Curves_dataset->push_back(curve);

        if(*max_points < curve.get_m())
            *max_points = curve.get_m();
    }
    file.close();

    return c;
}

// Return the sum of distance between all
// the pair of points.
double point_distance(Point x, Point y)
{
    double sum = 0;
    sum = abs(x.get_x() - y.get_x()) + abs(x.get_y() - y.get_y());

    return sum;
}

double minimum(double t1, double t2, double t3){
    double min = t1;
    if(t2 < min)
        min = t2;
    if(t3 < min)
        min = t3;

    return min;
}

void a1_a2(double* a1, double* a2, Point t, Point point, double delta){
    *a1 = (point.get_x()-t.get_x())/delta;
    //cout << *a1 <<endl;
    *a2 = (point.get_y()-t.get_y())/delta;
    int floor_a1 = floor(*a1);
    int floor_a2 = floor(*a2);
    int ceiling_a1 = ceil(*a1);
    int ceiling_a2 = ceil(*a2);
    double manh1 = abs(floor_a1 + t.get_x() - point.get_x()) + abs(floor_a2 + t.get_y() - point.get_y());
    int min = manh1;
    *a1 = floor_a1;
    *a2 = floor_a2;
    double manh2 = abs(floor_a1 + t.get_x() - point.get_x()) + abs(ceiling_a2 + t.get_y() - point.get_y());
    if(min > manh2) {
        min = manh2;
        *a1 = floor_a1;
        *a2 = ceiling_a2;
    }
    double manh3 = abs(ceiling_a1 + t.get_x() - point.get_x()) + abs(floor_a2 + t.get_y() - point.get_y());
    if(min > manh3){
        min = manh2;
        *a1 = ceiling_a1;
        *a2 = floor_a2;
    }
    double manh4 = abs(ceiling_a1 + t.get_x() - point.get_x()) + abs(ceiling_a2 + t.get_y() - point.get_y());
    if(min > manh4){
        min = manh2;
        *a1 = ceiling_a1;
        *a2 = ceiling_a2;
    }
    return;
}

double** DTW(Curve p, Curve q){
    int m1 = p.get_points().size();
    int m2 = q.get_points().size();
    //cout << m1 << " " << m2 << endl;
    double** table;
    table = new double* [m1+1];
    for(int i=0; i<m1+1; i++){
        table[i] = new double [m2+1];
    }

    table[0][0] = 0;
    for(int i=1; i<m1; i++){
        table[i][0] = 10000; //std::numeric_limits<double>::max();
    }
    for(int i=1; i<m2; i++){
        table[0][i] = 10000; //std::numeric_limits<double>::max();
    }
    for(int i=1; i<m1; i++){
        for(int j=1; j<m2; j++){
            table[i][j] = point_distance(p.get_points().at(i), q.get_points().at(j)) + minimum(table[i-1][j-1], table[i][j-1], table[i-1][j]);
        }
    }
    return table;
}

void print_table(double** table, int m1, int m2){
    for(int i=0; i<m1; i++){
        for(int j=0; j<m2; j++){
            cout << table[i][j] << "\t";
        }
        cout << endl;
    }
}

//epistrefei dianusma ftiaxnontas thn grin kampulh
Vector_Item grid_curve_vector(Curve curve, int max_points, int max_coord){
    int d = 2;
    double delta = 0.000001;
    vector<Point> grid;

    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<double> distribution(0.0,d);

    //gia ka8e shmeio 8a ftiaksw to (a1*delta, a2*delta)
    for(int p=0; p<curve.get_m(); p++){
        Point t, point = curve.get_points().at(p);
        Point g;
        double a1, a2;

        t.set_x(distribution(generator));
        t.set_y(distribution(generator));
        a1_a2(&a1, &a2, t, point, delta);

        //cout << "a1 = " << a1 << " a2 = "<< a2 << endl;
        g.set_x((a1*delta)+t.get_x());
        g.set_y((a2*delta)+t.get_y());

        int found = 0;
        for(int i=0; i<grid.size(); i++){
            if((grid.at(i).get_x() == g.get_x()) && (grid.at(i).get_y() == g.get_y())){
                found = 1;
                break;
            }
        }
        if(found == 0) grid.push_back(g);
    }
    //ftixnw dianisma apo to grid
    Vector_Item item;
    //vazw se dianusma me th seira ta xi kai ta yi apo ta points ths curve
    for(int i=0; i<grid.size(); i++){
            item.push(grid.at(i).get_x());
            item.push(grid.at(i).get_y());
    }
    for(int i=grid.size(); i<max_points*2; i++){
        item.push(max_coord);
    }
    return item;
}

Curve curve_ExactNN(vector<Curve> Curves_dataset, Curve curve, int c, double* ExactNN_dist){
    int NN_pos = -1;
    //int d = curve.get_vector().size();
    int NN_dist;
    long int min = 10000000000000;
    int i;

    for(i=0; i<c; i++){
        int m1 = Curves_dataset.at(i).get_points().size();
        int m2 = curve.get_points().size();
        Curve neighbor_curve = Curves_dataset.at(i);
        double **table = DTW(Curves_dataset.at(i),curve);
        NN_dist = table[m1-1][m2-1];

        if(NN_dist<min){
            NN_pos= i;
            min=NN_dist;
        }
        for(int t=0; t<m1+1; t++)
            delete[] table[t];
        delete[] table;
    }

    *ExactNN_dist = min;
    return Curves_dataset.at(NN_pos);
}

Curve curve_AproximateNN(vector<Curve> Curves_dataset, Curve curve, vector<Bucket>** HT, int buckets, int k_vec, int L_grid, long int m, int M, int W, double* AprNN_dist, int max_points, int max_coord){
    int NN_position = -1;
    long int NN_dist = 10000000000000;

    for(int l=0; l<L_grid; l++){
        //ftiaxnw to grid ths kampulhs
        Vector_Item item = grid_curve_vector(curve, max_points, max_coord);

        //to vazw se hash table
        int key = hash_key(item, buckets, max_points*2, k_vec, W, M, m);
        if((key<0) || (key>=buckets)) continue;

        if(HT[l]->at(key).get_point_pos().size() != 0){
            //gia ka8e stoixeio me kleidei key
            int retrieved_items = 0;
            for(int p=0; p<HT[l]->at(key).get_point_pos().size(); p++){
                retrieved_items++;
                int pos = HT[l]->at(key).get_point_pos().at(p);
                Curve neighbor_curve = Curves_dataset.at(pos);

                if(retrieved_items > 3*L_grid) break;

                int m1 = neighbor_curve.get_points().size();
                int m2 = curve.get_points().size();
                double **table = DTW(neighbor_curve,curve);
                //NN_dist = table[m1-1][m2-1];

                if(table[m1-1][m2-1] < NN_dist){
                   NN_position = pos;//it->second;
                   NN_dist = table[m1-1][m2-1];

                   for(int t=0; t<m1+1; t++)
                       delete[] table[t];
                   delete[] table;

                }
            }
        }
    }
    *AprNN_dist = NN_dist;

    return Curves_dataset.at(NN_position);
}

void write_curve_results(string Method, string HashFunction, string OUTfile, string Q_curve, string ExactNN_curve, string NN_curve, double AprNN_dist, double ExactNN_dist){

    ofstream file;
    file.open (OUTfile, ios::app); // append mode

    if(file.is_open()){
        file << "Query: " << Q_curve << endl;
        file << "Method: " << Method << endl;
        file << "HashFunction: " << HashFunction << endl;
        file << "Found Nearest neighbor: " << NN_curve << endl;
        file << "True Nearest neighbor: " << ExactNN_curve << endl;
        file << "distanceFound: " << AprNN_dist << endl;
        file << "distanceTrue: " << ExactNN_dist << endl;
        file << endl;
    }
}
