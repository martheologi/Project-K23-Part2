int distance_l1(vector<double>, vector<double>, int);

long long int modular_pow(long long int, long long int, int);

int a_generator(double, double, int);

int h_generator(vector<int>, int, long int, int);

int64_t g_generator(vector<int>, int);

int f_generator(int, unordered_map<int, int>*);

int hash_key(Vector_Item, int, int, int, int, int, long int);

string get_vertice(Vector_Item, int, int, int, int, unordered_map<int, int> *);

vector<Bucket>** HT_initialize(int numofV_hashtables, int buckets, int c, vector<Vector_Item> Items, int numofV_hashfuncts, int W, int M, int m);

vector<Bucket>** Curves_HT_initialize(int L_grid, int buckets, int c, int W, int M, int m, int k_vec, int max_coord , int max_points, vector<Curve> Curves_dataset);
