#include <bits/stdc++.h>
#pragma once
using namespace std;
#include "Vertex.cpp"

struct Sat_var{
    shared_ptr<Vertex> first;
    shared_ptr<Vertex> second;
    int truth=0;//-1 for false, 0 for unknown, 1 for truth
    weak_ptr<Sat_var> inv;
    vector<weak_ptr<Sat_var>> equiv;
    Sat_var(shared_ptr<Vertex> v1,shared_ptr<Vertex> v2):first(v1),second(v2){};

    bool process(int val){
        if(val==0) throw std::invalid_argument("Don't assign truth as 0");
        if(truth==-val)return false;
        truth=val;
        bool ok=1;
        if(inv.lock()->truth==val)return false;
        if(inv.lock()->truth==0){
            if(!(inv.lock()->process(-val)))return false;
        }
        for(auto nei:equiv){
            if(nei.lock()->truth==-val)return false;
            if(nei.lock()->truth==0){
                if(!(nei.lock()->process(val))) return false;
            }
        }
        return true;
    }
};