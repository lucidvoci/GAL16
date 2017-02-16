//
//  structures.cpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 28.10.16.
//
//

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "structures.hpp"

using namespace std;

void Graph::addVertex(vertex* new_vertex){
    all_vertex.push_back(new_vertex);
}

void Graph::printGraph(){
    for(vector<vertex*>::iterator it = all_vertex.begin(); it != all_vertex.end(); ++it){
        for(TVertexPricePair::iterator itt = (*it)->adj.begin(); itt != (*it)->adj.end(); ++itt){
            cout << (*it)->nid << " -- " << (*itt).second << " -> " << (*itt).first->nid << "\n";
        }
    }
}
