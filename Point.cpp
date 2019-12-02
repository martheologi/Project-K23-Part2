#include <iostream>
#include <vector>
#include <string>

#include "structs.h"

using namespace std;

Point::Point(){}

Point::~Point(){}

void Point::set_x(double xi){
    x = xi;
}

void Point::set_y(double yi){
    y = yi;
}

double Point::get_x(){
    return x;
}

double Point::get_y(){
    return y;
}
