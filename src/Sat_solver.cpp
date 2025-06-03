#pragma once
#include <bits/stdc++.h>
#include "Sat_var.cpp"

struct Sat_system{
    vector<shared_ptr<Sat_var>> variables;
    
    vector<vector<vector<shared_ptr<Sat_var>>>> lvl_ind_ind_to_var;
    

    //bool less(Vertex* a, Vertex* b);

    bool solve(){
        for(auto var : variables){
            if(var->truth!=0)continue;
            if(!var->process(1))return false;
        }
        return true;
    }
    
    void create_variables(Graph& g){
        int l = g.levels.size();
        for(int i=0;i<l;i++){
            int k = g.levels[i]->vertices.size();
            lvl_ind_ind_to_var.push_back(vector<vector<shared_ptr<Sat_var>>>(k,vector<shared_ptr<Sat_var>>(k)));
            for(int j=0;j<k;j++){
                for(int jj=0;jj<k;jj++){
                    if(j==jj)continue;
                    shared_ptr<Vertex> v1 = g.levels[i]->vertices[j];
                    shared_ptr<Vertex> v2 = g.levels[i]->vertices[jj];
                    shared_ptr<Sat_var> sat_variable =  make_shared<Sat_var>(v1,v2);
                    variables.push_back(sat_variable);
                    lvl_ind_ind_to_var[i][j][jj]=sat_variable;
                    if(j>jj){
                        shared_ptr<Sat_var> anti_var=lvl_ind_ind_to_var[i][jj][j];
                        sat_variable->inv = anti_var;
                        anti_var->inv = sat_variable;
                    }
                }
            }

        }
    }

    void fill_dependencies(Graph& g){
        int l = g.levels.size();
        for(int i=0;i<l-1;i++){
            int k=g.levels[i]->vertices.size();
            for(int j=0;j<k;j++){
                for(int jj=0;jj<k;jj++){
                    if(j==jj)continue;
                    shared_ptr<Vertex> v1 = g.levels[i]->vertices[j];
                    shared_ptr<Vertex> v2 = g.levels[i]->vertices[jj];
                    shared_ptr<Sat_var> v_var = lvl_ind_ind_to_var[i][j][jj];
                    for(auto x1:v1->down_neighbors){
                        for(auto x2:v2->down_neighbors){
                            if(x1.lock()==x2.lock())continue;
                            shared_ptr<Sat_var> x_var = lvl_ind_ind_to_var[i+1][x1.lock()->index][x2.lock()->index];
                            v_var->equiv.push_back(x_var);
                            x_var->equiv.push_back(v_var);
                        }    
                    }

                }
            }

        }
    }

    void create(Graph& g){
        create_variables(g);
        fill_dependencies(g);
    }

    static bool check_planarity(Graph& g){
        Sat_system s;
        s.create(g);
        return s.solve();
    }
    
};