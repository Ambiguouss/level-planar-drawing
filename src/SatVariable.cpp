#include "SatVariable.h"
#include <stdexcept>

SatVariable::SatVariable(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2)
    : first(v1), second(v2) {}

bool SatVariable::assign(TruthValue value) {
    if (value == TruthValue::Unknown) {
        throw std::invalid_argument("Cannot assign Unknown truth value");
    }
    
    if (truth != TruthValue::Unknown && truth != value) {
        return false;
    }
    
    if (truth == value) {
        return true;
    }
    
    truth = value;
    return propagateAssignment(value);
}

bool SatVariable::propagateAssignment(TruthValue value) {
    if (auto inversePtr = inverse.lock()) {
        TruthValue inverseValue = (value == TruthValue::True) ? TruthValue::False : TruthValue::True;
        
        if (inversePtr->truth != TruthValue::Unknown && inversePtr->truth != inverseValue) {
            return false;
        }
        
        if (inversePtr->truth == TruthValue::Unknown) {
            if (!inversePtr->assign(inverseValue)) {
                return false;
            }
        }
    }
    
    for (const auto& equivWeak : equivalent) {
        if (auto equivPtr = equivWeak.lock()) {
            if (equivPtr->truth != TruthValue::Unknown && equivPtr->truth != value) {
                return false;
            }
            
            if (equivPtr->truth == TruthValue::Unknown) {
                if (!equivPtr->assign(value)) {
                    return false;
                }
            }
        }
    }
    
    return true;
}
