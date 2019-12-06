Curve Initialize_Curve(string line, double* max_coord);

int Initialize_Curve_Dataset(string filename, vector<Curve>* Curves_dataset, int* max_points, double* max_coord);

double point_distance(Point x, Point y);

double minimum(double t1, double t2, double t3);

void a1_a2(double* a1, double* a2, Point t, Point point, double delta);

double** DTW(Curve p, Curve q);

void print_table(double** table, int m1, int m2);

Vector_Item grid_curve_vector(Curve curve, int max_points, int max_coord);

Curve curve_ExactNN(vector<Curve> Curves_dataset, Curve curve, int c, double* ExactNN_dist);

Curve curve_AproximateNN(vector<Curve> Curves_dataset, Curve curve, vector<Bucket>** HT, int buckets, int k_vec, int L_grid, long int m, int M, int W, double* AprNN_dist, int max_points, int max_coord);

Curve curve_HyperCubeNN(vector<Curve> Curves_dataset, Curve curve, vector<Hypercube_vertices> HyperCube, unordered_map<int, int> *f_index, int k, int M, int Modulus, long int m, int probes, int W, double *HyperCubeNN_dist, int max_points, int max_coord);

void write_curve_results(string outfile, int I, int A, int U, int k, vector<Cluster> clusters, vector<Curve> centroids, double cltime, int complete, vector<double> s, double stotal);
