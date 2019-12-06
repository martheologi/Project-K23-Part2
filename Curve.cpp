#include <iostream>
#include <vector>
#include <string>

#include "structs.h"

using namespace std;

Curve::Curve(){}

Curve::~Curve(){}

void Curve::set_id(string curve_id){
    id = curve_id;
}

void Curve::set_m(int mi){
    m = mi;
}

void Curve::push_point(Point p){
    points.push_back(p);
}

string Curve::get_id(){
    return id;
}

int Curve::get_m(){
    return m;
}

vector<Point> Curve::get_points(){
    return points;
}

int Curve::equal(Curve curve){
    if(m != curve.get_m()) return 0;
    //cout << m << " " <<curve.get_m() << endl;

    for(int i=0; i<m; i++){
        if((points.at(i).get_x() != curve.get_points().at(i).get_x()) || (points.at(i).get_y() != curve.get_points().at(i).get_y())) return 0;
    }
    return 1;
}

void Curve::print(){
    std::cout.precision(16);
    cout << "Curve " << id << " with " << m << " points ------> ";
    for(int i=0; i<points.size(); i++){
        cout << "(" << points.at(i).get_x() << " , " << points.at(i).get_y() << ") ";
    }
    cout << endl << endl;
}
