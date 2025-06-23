#include "Graph.cpp"
#include "Sat_solver.cpp"
#include "Planarer.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    Graph g = Graph::input_graph();
    g.draw("res.tex");
    Sat_system s;
    s.create(g);
    cout<<s.solve()<<'\n';
    Graph g2=Planarer::planarize(g);
    g2.draw("res_planar.tex");
}