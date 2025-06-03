#include "Graph.cpp"
#include "Sat_solver.cpp"
#include "Planarer.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    int seed;
    if(argc<4){
        cout<<"Provide: seed, file to draw non planar, file to draw planar.";
    }
    if (argc > 1) {
        // Convert first argument to an integer seed
        seed = std::atoi(argv[1]);
    } else {
        // Use current time as default seed
        seed = 0;
    }
    std::srand(seed);
    int lvl=5;
    int n=10;
    int p=0;
    vector<int> v(lvl,n);
    Graph g(lvl,v);

    for(int i=0;i<lvl-1;i++){
        for(int a=0;a<n;a++){
            for(int b=0;b<n;b++){
                int k=rand()%100;
                if(k<p)g.add_edge(i,a,b);
            }
        }
    }
    for(int l=0;l<lvl-1;l++){
        for(int i=0;i<3*n;i++){
            int a=rand()%n;
            int b=rand()%n;
            g.add_edge(l,a,b);
            if(!Sat_system::check_planarity(g)){
                g.levels[l]->vertices[a]->down_neighbors.pop_back();
                g.levels[l+1]->vertices[b]->up_neighbors.pop_back();
            }
        }
    }
    string filename1 =argv[2];
    g.draw(filename1);
    if(Sat_system::check_planarity(g)){
        Graph g2=Planarer::planarize(g);
        string filename2 =argv[3];
        g2.draw(filename2);
    }else{
        cout<<"Not planar\n";
    }
}