#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};

//Plan: constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
plan_id(plan_id), settlement(settlement), status(PlanStatus::AVALIABLE), selectionPolicy(selectionPolicy), life_quality_score(0), economy_score(0), environment_score(0), facilities{}, underConstruction{}, facilityOptions{} {}
//Plan: copy constructor WHAT ABOUT UNDERCONSTRUCTION AND FACILTYOPTIONS?????????????????????
Plan::Plan(const Plan& other):plan_id(other.plan_id), settlement(other.settlement), status(other.status), life_quality_score(other.getlifeQualityScore), economy_score(other.getEconomyScore), environment_score(other.getEnvironmentScore), facilities(other.getFacilities), underConstruction{}, facilityOptions{} {
    this->selectionPolicy = other.selectionPolicy->clone();
}
//Plan: copy assignment constructor
//Plan: destructor



//Plan: methods
const int Plan::getlifeQualityScore() const{return life_quality_score;}

const int Plan::getEconomyScore() const{return economy_score;}

const int Plan::getEnvironmentScore() const{return environment_score;}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){this->selectionPolicy = selectionPolicy->clone();}

void Plan::step(){
    for (int i = underConstruction.size() -1; i>=0;i--){
        Facility* f = underConstruction.at(i);
        f.step();
        if (f.getStatus()==FacilityStatus::OPERATIONAL){
            facilities.push_back(f);
            underConstruction.erase(underConstruction.begin()+i);}
    }

}

void Plan::printStatus(){
    std::cout << this->toString << std::endl;
    std::cout << "SettlementName:" << settlement.getName()<< std::endl;
    switch(status){
        case PlanStatus::AVALIABLE:
            std::cout << "PlanStatus:AVALIABLE" << std::endl;
        case PlanStatus::BUSY:
            std::cout << "PlanStatus:BUSY" << std::endl;
    }
    std::cout << "SelectionPolicy:" << selectionPolicy->toString() <<std::endl;
    std::cout << "LifeQualityScore:" << this->getlifeQualityScore()<< std::endl;
    std::cout << "EconomyScore:" << this->getEconomyScore()<< std::endl;
    std::cout << "EnvrionmentScore:" << this->getEnvironmentScore()<< std::endl;
    for (Facility f: facilities){
        std::cout << f.toString()<< std::endl;
        std::cout << f.toStringStatus()<< std::endl;
    }

}

const vector<Facility*>& Plan::getFacilities() const{return facilities;}

//what should we do here?
void Plan::addFacility(Facility* facility){
    facilityOptions.push_back(facility);
}

const string Plan::toString() const{return "PlanID:" + plan_id;}
