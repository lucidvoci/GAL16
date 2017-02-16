//
//  main.cpp
//  GAL_projekt
//
//  Created by Lucie Dvorakova on 28.10.16.
//
//

#include <iostream>
#include <queue>
#include <fstream>
#include <omp.h>
#include <climits>
#include <cstring>
#include "main.hpp"
#include "structures.hpp"
#include "generate.hpp"

using namespace std;


void printShortestPaths(int* distance, int num_vertex) {
    for (int j = 0; j < num_vertex; ++j) {
        if (distance[j] == INT_MAX) {
            cout << j << " infinity\n";
        }
        else {
            cout << j << " " << distance[j] << "\n";
        }
    }
}

// Sekvencni implementace algoritmu s frontou podezrelych
double runAlgorithmSeq(Graph* g, bool test) {
    double beginTime = omp_get_wtime();
    
    int num_vertex = g->all_vertex.size();
    int num_path = g->num_path;
    int distance[num_vertex];
    bool isInQueue[num_vertex];
    
    // Inicializace vzdalenosti vsech uzulu
    distance[g->all_vertex[0]->nid] = 0;
    isInQueue[g->all_vertex[0]->nid] = true;
    for (int i = 1; i < num_vertex; ++i)
    {
        distance[i] = INT_MAX;
        isInQueue[i] = false;
    }
    
    // Inicializace fronty podezrelych
    queue<vertex*> q;
    q.push(g->all_vertex[0]);
    
    int test_path = 0;
    while(!q.empty()) {
        vertex* u = q.front();
        q.pop();
        isInQueue[u->nid] = false;
        // prochazeni vsech prilehlych uzlu
        for (TVertexPricePair::iterator adjIt = u->adj.begin(); adjIt != u->adj.end(); ++adjIt)
        {
            // trojuhelnikove pravidlo (relax)
            if (distance[adjIt->first->nid] > distance[u->nid] + adjIt->second)
            {
                distance[adjIt->first->nid] = distance[u->nid] + adjIt->second;
                // uzel se nenachazi ve fronte
                if(!isInQueue[adjIt->first->nid])
                {
                    q.push(adjIt->first);
                    isInQueue[adjIt->first->nid] = true;
                }
            }
            // kontrola na negativni cykli pri generovani
            if(test) {
                test_path++;
                if (test_path > (num_path*num_vertex*2)) {
                    return 1;
                }
            }
        }
    }
    if (test) {
        return 0;
    }
    cout << "Num nodes: " << num_vertex << " num paths: " << num_path <<endl;
    cout << "Elapsed time is :  " <<  omp_get_wtime() - beginTime << endl;
    // Hustota grafu - tisk
    //printf("s %f %f\n",double(double(g->num_path) / double(num_vertex)), omp_get_wtime() - beginTime);
    // Nejkratsi cesty - tisk
    //printShortestPaths(distance, num_vertex);
    return 0;
}

// Paralelni implementace algoritmu s frontou podezrelych
int runAlgorithmPar(Graph* g)
{
    double beginTime = omp_get_wtime();
    
    int num_vertex = g->all_vertex.size();
    int num_path = g->num_path;
    int distance[num_vertex];
    bool isInQueue[num_vertex];
    queue<vertex*> q;
    
    #pragma omp parallel shared(distance, isInQueue, q)
    {
        
        #pragma omp for
        for (int i = 1; i < num_vertex; ++i)
        {
            distance[i] = INT_MAX;
            isInQueue[i] = false;
        }
        
        #pragma omp master
        {
            distance[g->all_vertex[0]->nid] = 0;
            isInQueue[g->all_vertex[0]->nid] = true;
            q.push(g->all_vertex[0]);
        }
        #pragma omp barrier
        
        while(!q.empty())
        {
            #pragma omp for
            for(int i = 0; i < q.size(); i++)
            {
                vertex* u;
                #pragma omp critical
                {
                    u = q.front();
                    q.pop();
                }
                isInQueue[u->nid] = false;
                // prochazeni vsech prilehlych uzlu
                for (TVertexPricePair::iterator adjIt = u->adj.begin(); adjIt != u->adj.end(); ++adjIt)
                {
                    // trojuhelnikove pravidlo (relax)
                    if (distance[adjIt->first->nid] > distance[u->nid] + adjIt->second)
                    {
                        distance[adjIt->first->nid] = distance[u->nid] + adjIt->second;
                        // uzel se nenachazi ve fronte
                        #pragma omp critical
                        {
                            if(!isInQueue[adjIt->first->nid])
                            {
                                q.push(adjIt->first);
                                isInQueue[adjIt->first->nid] = true;
                            }
                        }
                   }
                }
            }
            #pragma omp barrier
        }
    }
    cout << "Num nodes: " << num_vertex << " num paths: " << num_path <<endl;
    cout<< "Elapsed time is :  "<<  omp_get_wtime() - beginTime <<endl;
    // Hustota grafu - tisk
    //printf("p %f %f\n",double( double(g->num_path) / double(num_vertex)), omp_get_wtime() - beginTime);
    // Nejkratsi cesty - tisk
    //printShortestPaths(distance, num_vertex);
    return 0;
}


void initGraph(Graph* g, int num_vertex)
{
    for (int i = 0; i < num_vertex; ++i)
    {
        vertex* ver = new vertex();
        ver->nid = i;
        g->addVertex(ver);
    }
}
void createGraph(Graph* g, const string& input)
{
    int from_vertex_index;
    int to_vertex_index;
    int weight;
    sscanf(input.c_str(), "%d %d %d", &from_vertex_index, &to_vertex_index, &weight);

    // pridat hranu do grafu
    vertex* from_vertex = g->all_vertex[from_vertex_index];
    vertex* to_vertex = g->all_vertex[to_vertex_index];
    from_vertex->adj.push_back(make_pair(to_vertex,weight));
    
}

void deleteGraph(Graph* g)
{
    for (int i = 0; i < g->all_vertex.size(); ++i)
    {
        delete g->all_vertex[i];
    }
    
}

void printHelp(string name)
{
    cerr << "Usage: " << name << " [-p|-s|-g] [<input.txt>|1|2|3|4|5|6]" << std::endl;
    cerr << "\t -p <input.txt>\t- calculates algorithm parallely with <input.txt>" << std::endl;
    cerr << "\t -s <input.txt>\t- calculates algorithm sequentially with <input.txt>" << std::endl;
    cerr << "\t -g 1\t- generates graph nodes < 100, output input.txt" << std::endl;
    cerr << "\t -g 2\t- generates graph nodes < 500, output input.txt" << std::endl;
    cerr << "\t -g 3\t- generates graph nodes < 2500, output input.txt" << std::endl;
    cerr << "\t -g 4\t- generates graph paths < 20*nodes, output input.txt" << std::endl;
    cerr << "\t -g 5\t- generates graph paths < 100*nodes output input.txt" << std::endl;
    cerr << "\t -g 6\t- generates graph paths < nodes*nodes, output input.txt" << std::endl;
}

int main (int argc, char* argv[]) {
    // testovani argumentu
    if (argc < 3)
    {
        printHelp(argv[0]);
        return 1;
    }
    // generovani nahodnych grafu
    if (!strcmp(argv[1], "-g"))
    {
        Generate gen;
        return gen.generate(stoi(argv[2]));
    }
    
    // zpracovani argumentu
    bool parallel;
    if (!strcmp(argv[1], "-p"))
    {
        parallel = true;
    }
    else if (!strcmp(argv[1], "-s"))
    {
        parallel = false;
    }
    else
    {
        printHelp(argv[0]);
        return 1;
    }
    
    ifstream myFile;
    myFile.open(argv[2]);
    if (myFile.is_open())
    {
        int num_vertex;
        int num_path;
        string line;
        while (getline(myFile,line))
        {
            // parsovani jednotlivych grafu
            Graph g;
            sscanf(line.c_str(), "%d %d", &num_vertex, &num_path);
            g.num_path = num_path;
            initGraph(&g, num_vertex);
            for (int i = 0; i < num_path; ++i)
            {
                getline(myFile,line);
                createGraph(&g, line);
            }
            // spusteni seq/par algoritmu na graf
            if (parallel)
            {
                runAlgorithmPar(&g);
            }
            else
            {
                runAlgorithmSeq(&g, false);
            }
            deleteGraph(&g);
        }
        
        myFile.close();
    }
    else
    {
        cerr << "Error: Unable to open file" << std::endl;
        return 1;
    }
    return 0;
}


