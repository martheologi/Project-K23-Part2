vector<Cluster> Empty_Curve_cluster_check(int numof_clusters, vector<Cluster> temp_clusters, vector<Curve> centroids, vector<Curve> Curves_dataset);

int Equal_Curve_centroids(vector<Curve> c1, vector<Curve> c2, int numof_clusters);

void Curve_Random_Vector_Cetroids_Selection(vector<Curve>* centroids, vector<Curve> Curves_dataset, int numof_clusters);

void Curve_K_means_pp(vector<Curve>* centroids, vector<Curve> Curves_dataset, int numof_clusters);

vector<Cluster> Curves_Lloyds_Assignment(int numof_clusters, vector<Curve> centroids, vector<Curve> Curves_dataset);

vector<Curve> Curves_PAM_alaLloyds(vector<Curve> centroids, vector<Cluster> temp_clusters, vector<Curve> Curves_dataset);
