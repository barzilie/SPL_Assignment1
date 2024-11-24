#pragma once
#include <vector>
#include "Facility.h"
#include "SelectionPolicy.h"
using std::vector;
using namespace std;

// ************* NaiveSelection implementation: *************

//constructor
NaiveSelection::NaiveSelection():lastSelectedIndex(0){}

//added copy constructor (for clone)
NaiveSelection::NaiveSelection(const NaiveSelection &ns):lastSelectedIndex(ns.lastSelectedIndex){}


const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex++;
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive Selection: last index that was selected is "+lastSelectedIndex;
}


NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

//do i need to implement? 
NaiveSelection::~NaiveSelection(){}




//************* BalancedSelection implementation: ************* 

//constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

//added copy constructor (for clone)
BalancedSelection::BalancedSelection(const BalancedSelection &bs):LifeQualityScore(bs.LifeQualityScore), EconomyScore(bs.EconomyScore), EnvironmentScore(bs.EnvironmentScore){}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    vector<int> diffs;  
    diffs.reserve(facilitiesOptions.size);
    int indexOfMin = 0;
    int minDiff = INT_MAX;
    for(int i = 0; i<facilitiesOptions.size; i++){
        FacilityType current = facilitiesOptions[i];
        vector<int> scores = {this->EconomyScore + current.getEconomyScore, this->EnvironmentScore + current.getEnvironmentScore, this->LifeQualityScore + current.getLifeQualityScore};
        int maximum = max(scores);
        int minimum = min(scores);
        diffs[i] = maximum - minimum;
        if (diffs[i] < minDiff) {
            minDiff = diffs[i];
            indexOfMin = i;
        }
    }
    return facilitiesOptions[indexOfMin];
}


const string BalancedSelection::toString() const {
    return "Balanced Selection: LifeQualityScore is "+ LifeQualityScore +", EconomyScore is "+ EconomyScore +", EnvironmentScore is "+ EnvironmentScore; 
}
    

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

//destructor -what to do here
BalancedSelection::~BalancedSelection(){}




//************* EconomySelection implementation: ************* 

//constructor
EconomySelection::EconomySelection():lastSelectedIndex(0){}

//added copy constructor (for clone)
EconomySelection::EconomySelection(const EconomySelection &ecs):lastSelectedIndex(ecs.lastSelectedIndex){}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool found = false;
    int index = lastSelectedIndex+1;
    while(!found){
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY){
            found = true;
            lastSelectedIndex = index;
            return facilitiesOptions[lastSelectedIndex];
        }
        else{
            index = (index+1)%(facilitiesOptions.size);
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}


const string EconomySelection::toString() const {
    return "Economy Selection: last index that was selected is "+lastSelectedIndex;
}
    

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

//destructor -what to do here
EconomySelection::~EconomySelection(){}





//************* SustainabilitySelection implementation: ************* 

//constructor
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0){}

//added copy constructor (for clone)
SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &ss):lastSelectedIndex(ss.lastSelectedIndex){}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool found = false;
    int index = lastSelectedIndex+1;
    while(!found){
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT){
            found = true;
            lastSelectedIndex = index;
            return facilitiesOptions[lastSelectedIndex];
        }
        else{
            index = (index+1)%(facilitiesOptions.size());
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}


const string SustainabilitySelection::toString() const {
    return "Sustainability Selection: last index that was selected is "+lastSelectedIndex;
}
    

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}

//destructor -what to do here
SustainabilitySelection::~SustainabilitySelection(){}
