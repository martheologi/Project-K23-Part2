int range_finder(vector<Vector_Item> centroids);

int Equal_centroids(vector<Vector_Item> c1, vector<Vector_Item> c2, int numof_clusters);

void Random_Vector_Cetroids_Selection(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters);

vector<Cluster> Lloyds_Assignment(int numof_clusters, int d, vector<Vector_Item> centroids, vector<Vector_Item> Items);

void K_means_pp(vector<Vector_Item>* centroids, vector<Vector_Item> Items, int numof_clusters);

vector<Cluster> Assignment_By_Range_Search(vector<Vector_Item> centroids, vector<Vector_Item> Items,vector<Bucket>** HT, int numof_clusters, int numofV_hashtables, int numofV_hashfuncts, int buckets, int W, int m, int M);

vector<Cluster> Empty_cluster_check(int numof_clusters, vector<Cluster> temp_clusters, vector<Vector_Item> centroids, int d, vector<Vector_Item> Items);

vector<Vector_Item> Mean_Vector_Update(vector<Cluster> temp_clusters, vector<Vector_Item> Items);

vector<Vector_Item> PAM_alaLloyds(vector<Vector_Item> centroids, vector<Cluster> temp_clusters, vector<Vector_Item> Items);

double average_distance(Vector_Item item, int cluster_id, vector<Cluster> clusters, vector<Vector_Item> Items, vector<Vector_Item> centroids);

vector<double> Silhouette(vector<Cluster> clusters, vector<Vector_Item> Items, vector<Vector_Item> centroids, double* stotal);

vector<double> Silhouette(vector<Cluster> clusters, vector<Curve> Curves_dataset, vector<Curve> centroids, double* stotal);
