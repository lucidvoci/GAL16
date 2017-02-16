//
//  main.hpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 28.10.16.
//
//

#ifndef main_hpp
#define main_hpp

#include <stdio.h>
#include "structures.hpp"

using namespace std;

void initGraph(Graph* g, int num_vertex);
void createGraph(Graph* g, const string& input);
double runAlgorithmSeq(Graph* g, bool test);


#endif /* main_hpp */
