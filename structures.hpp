//
//  structures.hpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 28.10.16.
//
//

#ifndef structures_hpp
#define structures_hpp

#include <string>
#include <vector>
#include <stdio.h>

struct vertex {
    int nid;
    typedef std::pair<vertex*, int> vertex_adj;
    std::vector<vertex_adj> adj;
};

typedef std::vector<std::pair<vertex*,int> > TVertexPricePair;

class Graph {
public:
    int num_path;
    std::vector<vertex*> all_vertex;
    void addVertex(vertex* new_vertex);
    void printGraph();
};

#endif /* structures_hpp */
