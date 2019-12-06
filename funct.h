Vector_Item initialize_item(string);

int Initialize_Dataset_Vector(string, vector<Vector_Item>*);

void configuration(string conf_file, int* numof_clusters, int* numof_grids, int* numofV_hashtables, int* numofV_hashfuncts);

Vector_Item AproximateNN(vector<Vector_Item>, Vector_Item, vector<Bucket>**, int, int, int, long int, int, int, int*);

Vector_Item HyperCubeNN(vector<Vector_Item> Items, Vector_Item, vector<Hypercube_vertices>, unordered_map<int, int> *, int, int, int, long int, int, int, int *);

Vector_Item ExactNN(vector<Vector_Item>, Vector_Item, int, int*);

void write_results(string outfile, int I, int A, int U, int k, vector<Cluster> clusters, vector<Vector_Item> centroids, double cltime, int complete, vector<double> s, double stotal);
