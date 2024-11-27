#pragma once
#include <vector>
#include <algorithm>
#include <climits>
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
    int current_index = lastSelectedIndex;
    lastSelectedIndex = (lastSelectedIndex+1)%(facilitiesOptions.size());
    return facilitiesOptions[current_index];
}

const string NaiveSelection::toString() const {return "nve";}


NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}



//************* BalancedSelection implementation: ************* 

//constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

//added copy constructor (for clone)
BalancedSelection::BalancedSelection(const BalancedSelection &bs):LifeQualityScore(bs.LifeQualityScore), EconomyScore(bs.EconomyScore), EnvironmentScore(bs.EnvironmentScore){}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    vector<int> diffs;  
    diffs.reserve(facilitiesOptions.size());
    int indexOfMin = 0;
    int minDiff = INT_MAX;
    for(int i = 0; i<facilitiesOptions.size(); i++){
        FacilityType current = facilitiesOptions[i];
        vector<int> scores = {this->EconomyScore + current.getEconomyScore(), this->EnvironmentScore + current.getEnvironmentScore(), this->LifeQualityScore + current.getLifeQualityScore()};
        auto maxi = max_element(scores.begin(), scores.end());
        auto mini = min_element(scores.begin(), scores.end());
        int maximum = *maxi;
        int minimum = *mini;
        diffs[i] = maximum - minimum;
        if (diffs[i] < minDiff) {
            minDiff = diffs[i];
            indexOfMin = i;
        }
    }
    return facilitiesOptions[indexOfMin];
}


const string BalancedSelection::toString() const {return "bal";}
    

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}





//************* EconomySelection implementation: ************* 

//constructor
EconomySelection::EconomySelection():lastSelectedIndex(0){}

//added copy constructor (for clone)
EconomySelection::EconomySelection(const EconomySelection &ecs):lastSelectedIndex(ecs.lastSelectedIndex){}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool found = false;
    int index = lastSelectedIndex;
    while(!found){
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY){
            found = true;
            int current_index = index;
            lastSelectedIndex = (index+1)%(facilitiesOptions.size()); 
            FacilityType& output = facilitiesOptions[current_index];
        }
        else{
            index = (index+1)%(facilitiesOptions.size());
        }
    }
    return output;
}


const string EconomySelection::toString() const {return "eco";}
    

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}





//************* SustainabilitySelection implementation: ************* 

//constructor
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0){}

//added copy constructor (for clone)
SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &ss):lastSelectedIndex(ss.lastSelectedIndex){}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    bool found = false;
    int index = lastSelectedIndex;
    while(!found){
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT){
            found = true;
            int current_index = index;
            lastSelectedIndex = (index+1)%(facilitiesOptions.size()); 
            FacilityType& output = facilitiesOptions[current_index];
        }
        else{
            index = (index+1)%(facilitiesOptions.size());
        }
    }
    return output;

}


const string SustainabilitySelection::toString() const {return "env";}
    

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
