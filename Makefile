OBJS = cluster.o VectorItem.o hash.o funct.o bucket.o cluster_funct.o Cluster.o Curve.o Point.o curve_funct.o cluster_curve_funct.o
SOURCE = cluster.cpp VectorItem.cpp hash.cpp funct.cpp bucket.cpp cluster_funct.cpp Cluster.cpp Curve.cpp Point.cpp curve_funct.cpp cluster_curve_funct.cpp
HEADER = structs.h hash.h funct.h cluster_funct.h curve_funct.h cluster_curve_funct.h
OUT = cluster

CC = g++
FLAGS = -g3 -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

cluster.o: cluster.cpp
	$(CC) $(FLAGS) cluster.cpp

bucket.o: bucket.cpp
	$(CC) $(FLAGS) bucket.cpp

funct.o: funct.cpp
	$(CC) $(FLAGS) funct.cpp

cluster_funct.o: cluster_funct.cpp
	$(CC) $(FLAGS) cluster_funct.cpp

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp

Curve.o: Curve.cpp
	$(CC) $(FLAGS) Curve.cpp

Point.o: Point.cpp
	$(CC) $(FLAGS) Point.cpp

hash.o: hash.cpp
	$(CC) $(FLAGS) hash.cpp

curve_funct.o: curve_funct.cpp
	$(CC) $(FLAGS) curve_funct.cpp

cluster_curve_funct.o: cluster_curve_funct.cpp
	$(CC) $(FLAGS) cluster_curve_funct.cpp

clean:
	rm -f $(OBJS) $(OUT) output*
