#include "SatSolver.h"

bool SatSolver::solve() {
    for (const auto& variable : variables) {
        if (variable->truth == TruthValue::Unknown) {
            if (!variable->assign(TruthValue::True)) {
                return false;
            }
        }
    }
    return true;
}

void SatSolver::createVariables(const Graph& graph) {
    int levelCount = static_cast<int>(graph.levels.size());
    levelIndexToVariable.resize(levelCount);
    
    for (int levelIndex = 0; levelIndex < levelCount; ++levelIndex) {
        int vertexCount = static_cast<int>(graph.levels[levelIndex]->vertices.size());
        levelIndexToVariable[levelIndex].resize(vertexCount, 
            std::vector<std::shared_ptr<SatVariable>>(vertexCount));
        
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < vertexCount; ++j) {
                if (i == j) continue;
                
                auto vertex1 = graph.levels[levelIndex]->vertices[i];
                auto vertex2 = graph.levels[levelIndex]->vertices[j];
                auto variable = std::make_shared<SatVariable>(vertex1, vertex2);
                
                variables.push_back(variable);
                levelIndexToVariable[levelIndex][i][j] = variable;
                
                // Link with inverse variable
                if (i > j) {
                    auto inverseVar = levelIndexToVariable[levelIndex][j][i];
                    variable->inverse = inverseVar;
                    inverseVar->inverse = variable;
                }
            }
        }
    }
}

void SatSolver::createDependencies(const Graph& graph) {
    int levelCount = static_cast<int>(graph.levels.size());
    
    for (int levelIndex = 0; levelIndex < levelCount - 1; ++levelIndex) {
        int vertexCount = static_cast<int>(graph.levels[levelIndex]->vertices.size());
        
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < vertexCount; ++j) {
                if (i == j) continue;
                
                auto vertex1 = graph.levels[levelIndex]->vertices[i];
                auto vertex2 = graph.levels[levelIndex]->vertices[j];
                auto currentVariable = levelIndexToVariable[levelIndex][i][j];
                
                for (const auto& neighbor1Weak : vertex1->down_neighbors) {
                    for (const auto& neighbor2Weak : vertex2->down_neighbors) {
                        auto neighbor1 = neighbor1Weak.lock();
                        auto neighbor2 = neighbor2Weak.lock();
                        
                        if (!neighbor1 || !neighbor2 || neighbor1 == neighbor2) continue;
                        
                        auto neighborVariable = levelIndexToVariable[levelIndex + 1]
                            [neighbor1->index][neighbor2->index];
                        
                        currentVariable->equivalent.push_back(neighborVariable);
                        neighborVariable->equivalent.push_back(currentVariable);
                    }
                }
            }
        }
    }
}

void SatSolver::create(const Graph& graph) {
    createVariables(graph);
    createDependencies(graph);
}

bool SatSolver::checkPlanarity(const Graph& graph) {
    SatSolver solver;
    solver.create(graph);
    return solver.solve();
}
