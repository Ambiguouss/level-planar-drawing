#include <bits/stdc++.h>
#pragma once
#include "Vertex.cpp"
using namespace std;
struct Level{
    vector<std::shared_ptr<Vertex>> vertices;
    Level(){}
    Level(int vert_number){
        vertices.resize(vert_number);
        for(int i=0;i<vert_number;i++){
            vertices[i] = make_shared<Vertex>(to_string(i),i);
        }
    }
    void add_vertex(std::shared_ptr<Vertex> v){
        vertices.push_back(v);
    }
};
