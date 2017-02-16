//
//  generate.cpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 04.12.16.
//
//

#include <sys/time.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "generate.hpp"
#include "main.hpp"
#include "structures.hpp"

using namespace std;


int random(int min, int max){
    return min + (rand() % (int)(max - min + 1));
}

vector<string> createGraph(Graph* g, int max_node, int min_node, int path_coef) {
    // vypocet mnozstvi uzlu
    int num_node = random(min_node, max_node);
    
    // vypocet mnozstvi hran
    int max_path;
    int min_weight =  MIN_WEIGHT;
    if (path_coef == FULL_DENS_COEF) {
        max_path = num_node * num_node - num_node;
    }
    else {
        max_path = num_node * path_coef;
    }
    if (path_coef == LOW_DENS_COEF) {
        min_weight = MIN_WEIGHT_NEG;
    }
    int min_path = max_path / 2;
    int num_path = random(min_path, max_path);
    
    // struktura uchovavajici zda hrana jiz existuje
    int** exists = new int*[num_node];
    for (int i = 0; i < num_node; i++) {
        exists[i] = new int[num_node];
    }
    for (int i = 0; i < num_node; i++) {
        for (int j = 0; j < num_node; j++) {
            exists[i][j] = 0;
        }
    }
    
    vector<string> graph;
    
    // vytvoreni stuktury grafu
    graph.push_back(to_string(num_node) + " "  + to_string(num_path));
    initGraph(g, num_node);
    g->num_path = num_path;
    
    int weight;
    int from_node;
    int to_node;
    int avalible_index;
    bool restart = true;
    while (num_path != 0)
    {
        vector<int> availables;
        weight = random(min_weight, MAX_WEIGHT);
        from_node = random(0, num_node - 1);
        // zjistiv vsechny uzlu ktere jsou dostupne
        for(int i = 0; i < num_node; ++i) {
            if (exists[from_node][i] == 0) {
                availables.push_back(i);
                restart = false;
            }
        }
        
        // jestlize uz z uzlu neni mozna dalsi hrana
        // generuje se jiny vystupni uzel
        if (restart) { continue; }
        
        // vybrani uzel z moznych
        avalible_index = random(0, availables.size() - 1);
        to_node = availables[avalible_index];
        
        // zamezeni smycek (snizi pravdepodobnost negativnich cyklu)
        if (from_node == to_node) { continue; }
        
        // zamezeni negativnich zpetnych hran (snizi pravdepodobnost negativnich cyklu)
        if (from_node > to_node) {
            weight = abs(weight);
        }
        exists[from_node][to_node] = 1;
        
        // vytvareni textove podoby grafu
        graph.push_back(to_string(from_node) + " " + to_string(to_node) + " " + to_string(weight));
        
        // vytvareni struktury grafu
        createGraph(g, to_string(from_node) + " " + to_string(to_node) + " " + to_string(weight));
        num_path--;
        
    }
    return graph;
}

// spusteni nahodne vygenerovaneho grafu
// probiha kontrola zda neobsahuje negativni cyklus
bool checkGraph(Graph* g) {
    bool result = !runAlgorithmSeq(g, true);
    return result;
}

// tisk grafu do souboru
void printGraph(vector<string>& lines, ofstream& out, bool to_file) {
    for (string& line : lines) {
        if(to_file) {
            out << line << endl;
        }
    }
}



int Generate::generate(int version) {
    int max_node;
    int min_node;
    int path_coef = LOW_DENS_COEF;
    
    // prideleni hodnot na zaklade zvolene verze
    if (version == VERSION1) {
        max_node = SMALL_MAX;
        min_node = SMALL_MIN;
    }
    else if (version == VERSION2) {
        max_node = MED_MAX;
        min_node = MED_MIN;
    }
    else if (version == VERSION3) {
        max_node = LARGE_MAX;
        min_node = LARGE_MIN;
    }
    else if (version == VERSION4 || version == VERSION5 || version == VERSION6)
    {
        max_node = LARGE_MAX;
        min_node = LARGE_MIN;
        if (version == VERSION4) {
            path_coef = MID_DENS_COEF;
        }
        else if (version == VERSION5) {
            path_coef = HIGH_DENS_COEF;
        }
        else {
            path_coef = FULL_DENS_COEF;
        }
    }
    else {
        cerr << "Error: Generating version: 1, 2, 3, 4, 5 or 6." << endl;
        return 1;
    }
    
    ofstream out;
    out.open("input.txt");
    struct timeval tv;
    
    gettimeofday(&tv, 0);
    srand(tv.tv_usec); // inicializace seed random generatoru
    // vytvareni grafu dokud neni jeden bez negativniho cyklu
    while(true) {
        vector<string> graph;
        Graph g;
        graph = createGraph(&g, max_node, min_node, path_coef);
        if (checkGraph(&g))
        {
            printGraph(graph, out, true);
            cout << "Graph successfully printed." << endl;
            break;
        }
    }
    out.close();
    
    return 0;
}
