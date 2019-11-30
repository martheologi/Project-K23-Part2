#include <iostream>
#include <vector>

#include "structs.h"

using namespace std;

Vector_Item::Vector_Item(){}

Vector_Item::~Vector_Item(){}

void Vector_Item::set_id(string id){
    item_id = id;
}

void Vector_Item::push(double xi){
    x.push_back(xi);
}

string Vector_Item::get_item_id(){
    return item_id;
}

vector<double> Vector_Item::get_vector(){
    return x;
}

void Vector_Item::print_item(){
    cout << "Item's id: " << item_id << endl;

    for(int i=0; i < x.size(); i++)
        cout << x.at(i) << ' ';

    cout << endl;
}

int Vector_Item::equal(Vector_Item item){
    for(int i=0; i<x.size(); i++){
        if(x.at(i) != item.get_vector().at(i)) return 0;
    }
    return 1;
}
