OBJS = cluster.o VectorItem.o hash.o funct.o bucket.o cluster_funct.o Cluster.o
SOURCE = cluster.cpp VectorItem.cpp hash.cpp funct.cpp bucket.cpp cluster_funct.cpp Cluster.cpp
HEADER = structs.h hash.h funct.h cluster_funct.h
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

hash.o: hash.cpp
	$(CC) $(FLAGS) hash.cpp

clean:
	rm -f $(OBJS) $(OUT) output*
