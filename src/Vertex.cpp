#include <bits/stdc++.h>
#pragma once
using namespace std;
struct Vertex{
    string label;
    int index;
    vector<weak_ptr<Vertex>> down_neighbors;
    vector<weak_ptr<Vertex>> up_neighbors;
    Vertex(string _label,int index):label(_label),index(index){}
    void add_down_neighbor(weak_ptr<Vertex> v){
        down_neighbors.push_back(v);
    }

    void add_up_neighbor(weak_ptr<Vertex> v){
        up_neighbors.push_back(v);
    }

};
