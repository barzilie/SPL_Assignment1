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
plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities{}, underConstruction{}, facilityOptions{}, life_quality_score(0), economy_score(0), environment_score(0) {
}
// Plan: copy constructor and copy assignment operator - NOT REQUIRED
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

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){this->selectionPolicy = selectionPolicy->clone();}

void Plan::step(){
    if(this->status == PlanStatus::AVALIABLE){
        int availabeSpots = static_cast<int>(this->settlement.getType()) + 1 - this->underConstruction.size();
        while(availabeSpots>0){
            FacilityType nextType = this->selectionPolicy->selectFacility(this->facilityOptions);
            Facility* f = new Facility(nextType, nextType.getName());
            this->addFacility(f);
            availabeSpots--;
        }
        this->status = PlanStatus::BUSY;

    }
    for (int i = underConstruction.size() -1; i>=0;i--){
        Facility* f = underConstruction.at(i);
        if (f->step()==FacilityStatus::OPERATIONAL){
            facilities.push_back(f);
            this->economy_score = this->economy_score + f->getEconomyScore();
            this->environment_score = this->economy_score + f->getEnvironmentScore();
            this->life_quality_score = this->economy_score + f->getLifeQualityScore();
            underConstruction.erase(underConstruction.begin()+i);}
    }

}

void Plan::printStatus(){
    std::cout << this->toString() << std::endl;
    std::cout << settlement.toString()<< std::endl;
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
    for (Facility* f: facilities){
        std::cout << f->toString()<< std::endl;
        std::cout << f->toStringStatus()<< std::endl;
    }

}

const vector<Facility*>& Plan::getFacilities() const{return facilities;}

void Plan::addFacility(Facility* facility){
    underConstruction.push_back(facility);
    //do we need to increment the score of the balanced selection (if using it)?
    
    /*
       if(this->getSelectionPolicy == "bal"){
        this->s
    */

}

const string Plan::toString() const{return "PlanID:" + this->plan_id;}

const int Plan::getID() const{return this->plan_id;}

const string Plan::getSelectionPolicy() const{return this->selectionPolicy->toString();}

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
        environmentScore_UC = environmentScore_UC + f->getEconomyScore();
    }
    return environmentScore_UC;
}


