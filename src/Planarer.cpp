#pragma once
#include <bits/stdc++.h>
#include "Sat_solver.cpp"
#include "Graph.cpp"

struct Planarer{

    static Graph doubled(Graph& g){
        int l = g.levels.size();
        Graph double_graph(2*l);
        for(int i=0;i<l;i++){
            int k= g.levels[i]->vertices.size();
            double_graph.levels[2*i]=make_shared<Level>(k);
            double_graph.levels[2*i+1]=make_shared<Level>(k);
            for(int j=0;j<k;j++){
                double_graph.add_edge(2*i,j,j);
            }
        }
        for(int i=0;i<l;i++){
            int k= g.levels[i]->vertices.size();

            for(int j=0;j<k;j++){
                for(auto x:g.levels[i]->vertices[j]->up_neighbors){
                    double_graph.add_edge(2*i-1,x.lock()->index,j);
                }
            }
            for(int j=0;j<k;j++){
                for(auto x:g.levels[i]->vertices[j]->down_neighbors){
                    double_graph.add_edge(2*i+1,j,x.lock()->index);
                }
            }
        }
        return double_graph;
    }

    static void add_max_edges(Graph& g){
        for(int l=0;l<g.levels.size();l+=2){
            for(int i=0;i<g.levels[l]->vertices.size();i++){
                for(int j=0;j<g.levels[l+1]->vertices.size();j++){
                    g.add_edge(l,i,j);
                    if(!Sat_system::check_planarity(g)){
                        g.levels[l]->vertices[i]->down_neighbors.pop_back();
                        g.levels[l+1]->vertices[j]->up_neighbors.pop_back();
                    }
                }
            }
        }
    }

    static Graph planarize(Graph& g){
        Graph double_graph = doubled(g);
        add_max_edges(double_graph);
        vector<vector<int>>permutations(g.levels.size());
        for(int l=0;l<g.levels.size();l++){
            int k = g.levels[l]->vertices.size();
            vector<vector<int>> path(k);
            for(int i=0;i<k;i++){
                for(auto x:double_graph.levels[2*l]->vertices[i]->down_neighbors){
                    if(x.lock()->index!=i){
                        path[i].push_back(x.lock()->index);
                    }
                }
                for(auto x:double_graph.levels[2*l+1]->vertices[i]->up_neighbors){
                    if(x.lock()->index!=i){
                        path[i].push_back(x.lock()->index);
                    }
                }
            }
            for(int i=0;i<k;i++){
                if(path[i].size()<=1){
                    permutations[l].push_back(i);
                    int prev=i;
                    int curr=path[i][0];
                    while(path[curr].size()==2){
                        permutations[l].push_back(curr);
                        int next=path[curr][0];
                        if(next==prev){
                            next=path[curr][1];
                        }
                        prev=curr;
                        curr=next;
                    }
                    permutations[l].push_back(curr);
                    break;
                }
            }
        }


        //check orientation for every level
        vector<vector<int>>rev_perm;
        
        rev_perm.push_back(vector<int>(permutations[0].size()));
        for(int i=0;i<permutations[0].size();i++){
            rev_perm[0][permutations[0][i]]=i;
        }

        for(int l=1;l<permutations.size();l++){
            vector<int>x=permutations[l];
            int i=0;
            while(i<x.size()&&g.levels[l]->vertices[x[i]]->up_neighbors.size()==0)i++;
            int j=x.size()-1;
            while(j>=0&&g.levels[l]->vertices[x[j]]->up_neighbors.size()==0)j--;
            if(i==x.size()) continue;

            auto i_up_neighbors = g.levels[l]->vertices[x[i]]->up_neighbors; 
            auto j_up_neighbors = g.levels[l]->vertices[x[j]]->up_neighbors; 
            int up_i = i_up_neighbors[0].lock()->index;
            int up_j = j_up_neighbors[0].lock()->index;

            if (rev_perm[l - 1][up_j] < rev_perm[l - 1][up_i]) {
                reverse(permutations[l].begin(),permutations[l].end());
            }else if(rev_perm[l - 1][up_j] == rev_perm[l - 1][up_i]){
                if(j_up_neighbors.size()>1 && rev_perm[l-1][j_up_neighbors[1].lock()->index] <rev_perm[l - 1][up_i]){
                    reverse(permutations[l].begin(),permutations[l].end());
                }
            }

            

            rev_perm.push_back(vector<int>(permutations[l].size()));
            for(int i=0;i<permutations[l].size();i++){
                rev_perm[l][permutations[l][i]]=i;
            }
        }
        
        Graph res(g.levels.size());
        for(int i=0;i<g.levels.size();i++){
            for(int j=0;j<permutations[i].size();j++){
                res.add_vertex(make_shared<Vertex>(g.levels[i]->vertices[permutations[i][j]]->label,j),i);
            }
        }
        for(int l=0;l<res.levels.size()-1;l++){
            for(int i=0;i<res.levels[l]->vertices.size();i++){
                for(auto x:g.levels[l]->vertices[permutations[l][i]]->down_neighbors){
                    int mapped_index = rev_perm[l+1][x.lock()->index];
                    res.add_edge(l,i,mapped_index);
                }
            }
        }
        return res;
    }
    
};