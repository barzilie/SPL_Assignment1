#pragma once
#include <vector>
#include <iostream>
#include <ostream>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
using std::vector;
using namespace std;

//Plan: constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities{}, underConstruction{}, facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {
}

//Plan: copy constructor
Plan::Plan(const Plan& other):plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), status(other.status), facilities{}, underConstruction{}, facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    int facilities_size = static_cast<int>(other.facilities.size()); //casting size to int (otherwise can't compare i to size)
    int underConstruction_size = static_cast<int>(other.underConstruction.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i<facilities_size; i++){
        facilities.push_back(other.facilities.at(i)->clone());
    }
    for(int i=0; i<underConstruction_size; i++){
        underConstruction.push_back(other.underConstruction.at(i)->clone());
    }
}

//special copy constructor for passing reference to settlement
Plan::Plan(const Plan& other, const Settlement &settlement):plan_id(other.plan_id), settlement(settlement), selectionPolicy(other.selectionPolicy->clone()), status(other.status), facilities{}, underConstruction{}, facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    int facilities_size = static_cast<int>(other.facilities.size()); //casting size to int (otherwise can't compare i to size)
    int underConstruction_size = static_cast<int>(other.underConstruction.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i<facilities_size; i++){
        facilities.push_back(other.facilities.at(i)->clone());
    }
    for(int i=0; i<underConstruction_size; i++){
        underConstruction.push_back(other.underConstruction.at(i)->clone());
    }
}

// move copy constructor
Plan::Plan(Plan &&other):plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), status(other.status), facilities{}, underConstruction{}, facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    other.selectionPolicy = nullptr;
    int facilities_size = static_cast<int>(other.facilities.size()); 
    int underConstruction_size = static_cast<int>(other.underConstruction.size()); 
    for(int i=0; i<facilities_size; i++){
        facilities.push_back(other.facilities.at(i));
        other.facilities.at(i) = nullptr;
    }
    for(int i=0; i<underConstruction_size; i++){
        underConstruction.push_back(other.underConstruction.at(i));
        other.underConstruction.at(i) = nullptr;
    }
}

// Plan: destructor
Plan::~Plan(){
    delete selectionPolicy;
    for (Facility* f: facilities){
        delete f;
    }
    for (Facility* f: underConstruction){
        delete f;
    }}

//Plan: methods
const int Plan::getlifeQualityScore() const{return life_quality_score;}

const int Plan::getEconomyScore() const{return economy_score;}

const int Plan::getEnvironmentScore() const{return environment_score;}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    if(this->selectionPolicy){
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

void Plan::step(){
    if(this->status == PlanStatus::AVALIABLE){
        int availabeSpots = static_cast<int>(this->settlement.getType()) + 1 - this->underConstruction.size();
        while(availabeSpots > 0){
            FacilityType nextType = this->selectionPolicy->selectFacility(this->facilityOptions);
            Facility* f = new Facility(nextType, settlement.getName());
            this->addFacility(f);
            availabeSpots--;
        }
        this->status = PlanStatus::BUSY;

    }
    int uc_size = static_cast<int>(underConstruction.size());
    int toRemove_size = uc_size;
    vector<bool> toRemove(uc_size,false);
    for (int i = 0; i < uc_size ;i++){
        Facility* f = underConstruction.at(i);
        if (f->step() == FacilityStatus::OPERATIONAL){
            facilities.push_back(f);
            this->economy_score = this->economy_score + f->getEconomyScore();
            this->environment_score = this->environment_score + f->getEnvironmentScore();
            this->life_quality_score = this->life_quality_score + f->getLifeQualityScore();
            toRemove[i] = true;
        }
    }
    for (int j = toRemove_size - 1; j >= 0 ; j--){
        if(toRemove[j]){
            underConstruction.erase(underConstruction.begin() + j);
            uc_size--;
        }

    }
    int constractionLimit = static_cast<int>(this->settlement.getType()) + 1;
    if(constractionLimit > uc_size){
        this->status = PlanStatus::AVALIABLE;
    }
}

void Plan::printStatus(){
    std::cout << this->toString() << std::endl;
    std::cout << settlement.toString()<< std::endl;
    //has to insert an if statement
    if(this->status == PlanStatus::AVALIABLE){
        std::cout << "PlanStatus:AVALIABLE" << std::endl;
    }
    else{
        std::cout << "PlanStatus:BUSY" << std::endl;
    }
    std::cout << "SelectionPolicy:" << selectionPolicy->toString() <<std::endl;
    std::cout << "LifeQualityScore:" << this->getlifeQualityScore()<< std::endl;
    std::cout << "EconomyScore:" << this->getEconomyScore()<< std::endl;
    std::cout << "EnvrionmentScore:" << this->getEnvironmentScore()<< std::endl;
    //for-loop to regular
    // for (Facility* f: this->underConstruction){
    //     std::cout << f->toString()<< std::endl;
    //     std::cout << f->toStringStatus()<< std::endl;
    // }

    int underConstruction_size = static_cast<int>(underConstruction.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i<underConstruction_size; i++){
        cout << underConstruction.at(i)->toString()<< endl;
        cout << underConstruction.at(i)->toStringStatus()<< endl;
    }
    int facilities_size = static_cast<int>(this->facilities.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i<facilities_size; i++){
        cout << (facilities.at(i))->toString()<< endl;
        cout << facilities.at(i)->toStringStatus()<< endl;
    }
}

void Plan::printForClose(){
    std::cout << this->toString() << std::endl;
    std::cout << settlement.toString()<< std::endl;
    //std::cout << "SelectionPolicy:" << selectionPolicy->toString() <<std::endl;
    std::cout << "LifeQualityScore:" << this->getlifeQualityScore()<< std::endl;
    std::cout << "EconomyScore:" << this->getEconomyScore()<< std::endl;
    std::cout << "EnvrionmentScore:" << this->getEnvironmentScore()<< std::endl;
}

const vector<Facility*>& Plan::getFacilities() const{
    return facilities;
}

void Plan::addFacility(Facility* facility){
    underConstruction.push_back(facility);
    if(this->getSelectionPolicy() == "bal"){
        this->selectionPolicy->incrementScores(facility->getLifeQualityScore(), facility->getEconomyScore(), facility->getEnvironmentScore());
    }
}

const string Plan::toString() const{return "PlanID:" + to_string(this->plan_id);}

const int Plan::getID() const{return this->plan_id;}

const string Plan::getSelectionPolicy() const{return this->selectionPolicy->toString();}

const string Plan::getSelectionPolicyFN() const
{
    return this->selectionPolicy->toStringFullName();
}

//under construction facilities scores

const int Plan::getlifeQualityScore_UC() const{
    int lifeQualityScore_UC = 0;
    for(Facility* f: underConstruction){
        lifeQualityScore_UC = lifeQualityScore_UC + f->getLifeQualityScore();
    }
    return lifeQualityScore_UC;
}

const int Plan::getEconomyScore_UC() const{
    int economyScore_UC = 0;
    for(Facility* f: underConstruction){
        economyScore_UC = economyScore_UC + f->getEconomyScore();
    }
    return economyScore_UC;
}

const int Plan::getEnvironmentScore_UC() const{
    int environmentScore_UC = 0;
    for(Facility* f: underConstruction){
        environmentScore_UC = environmentScore_UC + f->getEnvironmentScore();
    }
    return environmentScore_UC;
}

const string Plan::getSettlementName() const{
    return settlement.getName();
}
