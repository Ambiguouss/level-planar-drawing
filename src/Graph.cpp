#include <bits/stdc++.h>
#pragma once
#include "Level.cpp"
using namespace std;
struct Graph{
    vector<std::shared_ptr<Level>> levels;
    Graph(){};
    Graph(int level_number){
        for (int i = 0; i < level_number; ++i) {
            levels.push_back(make_shared<Level>());
        }
    }
    Graph(int level_number,vector<int> vertex_size){
        levels.resize(level_number);
        for(int i=0;i<level_number;i++){
            levels[i]=make_shared<Level>(vertex_size[i]);
        }
    }
    
    void add_vertex(std::shared_ptr<Vertex> v,int level){
        levels[level]->vertices.push_back(v);
    }
    void add_edge(int level,int ind_up,int ind_down){
        shared_ptr<Vertex> v1 = levels[level]->vertices[ind_up];
        shared_ptr<Vertex> v2 = levels[level+1]->vertices[ind_down];
        v1->add_down_neighbor(v2);
        v2->add_up_neighbor(v1);
    }

    

    static Graph input_graph(){
        int levels;
        cin>>levels;
        vector<int> vertex_vector(levels);
        for(auto& x : vertex_vector){
            cin>>x;
        }
        Graph g=Graph(levels,vertex_vector); 
        for(int i=0;i<levels-1;i++){
            int k;
            cin>>k;
            for(int j=0;j<k;j++){
                int a,b;
                cin>>a>>b;
                g.add_edge(i,a,b);
            }
        }
        return g;
    }

    void draw(string filename){
        ofstream outFile(filename);

        if (!outFile) {
            cerr << "Error opening file!" << endl;
            return;
        }

        outFile << "\\documentclass{article}\n";
        outFile << "\\usepackage{tikz}\n";
        outFile << "\\usepackage{adjustbox}\n";
        outFile << "\\begin{document}\n";
        outFile << "\\adjustbox{max size={\\textwidth}{\\textheight}}{%\n";
        outFile << "\\begin{tikzpicture}[]\n";
        

        for(int lvl=0;lvl<levels.size();lvl++){
            shared_ptr<Level> x = levels[lvl];
            for(int i=0;i<x->vertices.size();i++){
                shared_ptr<Vertex> v = x->vertices[i];
                outFile << "\\node[draw, circle] (" <<v  << ") at (" 
                    << i << "," << -lvl
                    << ") {" << v->label << "};\n";
            }
        }

        for(int lvl=0;lvl<levels.size();lvl++){
            shared_ptr<Level> x = levels[lvl];
            for(int i=0;i<x->vertices.size();i++){
                shared_ptr<Vertex> v = x->vertices[i];
                for(auto other:v->down_neighbors){
                    outFile << "\\draw (" << v<< ") -- (" 
                    << other.lock()<< ");\n";
                }
            }
        }

        outFile << "\\end{tikzpicture}%\n}\n";
        outFile << "\\end{document}\n";
        
        outFile.close();
        cout << "TikZ file generated: " << filename << endl;


    }
};
