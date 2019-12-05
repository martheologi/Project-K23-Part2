#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Vector_Item{
    private:
        string item_id;
        vector<double> x;

    public:
        Vector_Item();
        ~Vector_Item();
        void set_id(string id);
        void push(double xi);
        string get_item_id();
        vector<double> get_vector();
        void print_item();
        int equal(Vector_Item item);
};

class Bucket{
    private:
        int key;
        vector<int> point_pos; //krataw th 8esh tou dianusmatos ston pinaka Items
    public:
        Bucket();
        ~Bucket();
        void set_key(int k);
        void push_pos(int pos);
        int get_key();
        vector<int> get_point_pos();
        void print();
};

class Hypercube_vertices{
    private:
        string code;
        vector<int> p_vector;

    public:
        Hypercube_vertices();
        ~Hypercube_vertices();
        void set_code(string HCcode);
        string get_code();
        vector<int> get_p_vector();
        void add_point(int point_pos);
};

int find_vertice(vector<Hypercube_vertices> HyperCube, string vertice);

void print_HyperCube(vector<Hypercube_vertices> HyperCube);

class Point{
    private:
        double x;
        double y;

    public:
        Point();
        ~Point();
        void set_x(double xi);
        void set_y(double yi);
        double get_x();
        double get_y();
};

class Curve{
    private:
        string id;
        int m;
        vector<Point> points;
    public:
        Curve();
        ~Curve();
        void set_id(string curve_id);
        void set_m(int mi);
        void push_point(Point p);
        string get_id();
        int get_m();
        vector<Point> get_points();
        int equal(Curve curve);
        void print();
};

class Cluster{
    private:
        string center_id;
        vector<int> positions;
    public:
        Cluster();
        ~Cluster();
        void set_center_id(string id);
        void push_position(int pos);
        void print_cluster();
        string get_center_id();
        vector<int> get_positions();
};
