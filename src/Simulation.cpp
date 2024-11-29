#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <array>
#include "Auxiliary.h"
#include "Simulation.h"
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;
using namespace std;


Simulation::Simulation(const string &configFilePath):isRunning(false), planCounter(0), actionsLog{}, plans{}, settlements{}, facilitiesOptions{} {
    std::ifstream file(configFilePath);
    //
    // open config file
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        // return 1; //the compiler didnt like returning ftom constructor
    }
    //necessery?^
    std::string line;
    while (std::getline(file, line)) {
        if(line[0]!='#'){
            vector<string> args = Auxiliary::parseArguments(line);
            switch(args[0]){

                //# settlement <settlement_name> <settlement_type> 
                case "settlement":
                    //change according to cast - 2 next lines 
                    const std::array<SettlementType, 3> types = {SettlementType::VILLAGE, SettlementType::CITY, SettlementType::METROPOLIS};
                    settlements.push_back(new Settlement::Settlement(args[1],types[stoi(args[2])]));

                //# facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
                case "facility":
                    //change according to cast - 2 next lines 
                    const std::array<FacilityCategory, 3> category = {FacilityCategory::LIFE_QUALITY, FacilityCategory::ECONOMY, FacilityCategory::ENVIRONMENT};
                    facilitiesOptions.push_back(FacilityType::FacilityType(args[1], category[stoi(args[2])],stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6])));

                //# plan <settlement_name> <selection_policy>
                case "plan":
                    switch(args[2]){
                        case "bal":
                            plans.push_back(Plan::Plan(planCounter, args[1], new BalancedSelection::BalancedSelection(0,0,0), facilitiesOptions));
                        case "eco":
                            plans.push_back(Plan::Plan(planCounter, args[1], new EconomySelection::EconomySelection(), facilitiesOptions));
                        case "nve":
                            plans.push_back(Plan::Plan(planCounter, args[1], new NaiveSelection::NaiveSelection(), facilitiesOptions));
                        case "env":
                            plans.push_back(Plan::Plan(planCounter, args[1], new SustainabilitySelection::SustainabilitySelection(), facilitiesOptions));
                    }
                    planCounter++;
                    //seems like we should use some implementation of the rule of 5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }
        } 
    } 
}


void Simulation::start(){
    this->isRunning = true;
    cout << "The simulation has started" << endl;
    while(isRunning){
    //string userInput;
   // getline(cin, userInput);

    }
    //wait to input from user (action to execute)?  
    //string userInput;
   // getline(cin, userInput);

}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan p = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
    plans.push_back(p);
}

void Simulation::addAction(BaseAction *action){
    //after implementing action class
    
}

bool Simulation::addSettlement(Settlement *settlement){

    //if settlement is already in settlements vector: returns false, otherwise add it to settlements and return true
    bool exists = isSettlementExists(settlement->getName());
    if (!exists){
        settlements.push_back(settlement);
    }
    return exists;
    
}

bool Simulation::addFacility(FacilityType facility){

    //if facility is already in facilitiesOptions vector: returns false, otherwise add it to facilitiesOptions and return true
    bool exists = isFacilityExists(facility.getName());
    if(!exists){
        facilitiesOptions.push_back(facility);
    }
    return exists;

}

bool Simulation::isSettlementExists(const string &settlementName){
    for(Settlement* s: settlements){
        if(s->getName() == settlementName){
            return true;
        }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName){
    for(FacilityType f: facilitiesOptions){
        if(f.getName() == facilityName){
            return true;
        }
    }
    return false;
}

 bool Simulation::isPlanExists(const int planId){
    for(Plan p: plans){
        if(p.getID() == planId){
            return true;
        }
    }
    return false;
 }

Settlement& Simulation::getSettlement(const string &settlementName){
    for(Settlement* s: settlements){
        if(s->getName() == settlementName){
            return *s;
        }
    }
    return Settlement("invalid", SettlementType::VILLAGE);
}

Plan& Simulation::getPlan(const int planId){
    for(Plan p: plans){
        if(p.getID() == planId){
            return p;
        }
    }
    return p;
 }

 bool Simulation::changePlanPolicy(const int planId, const string &newPolicy){
    if (!isPlanExists(planId)){
        return false;
    }
    if(newPolicy != "nve" && newPolicy != "bal" && newPolicy != "eco" && newPolicy != "env"){
        return false;
    }
    string planSP = this->getPlan(planId).getSelectionPolicy();
    if (planSP == newPolicy){
        return false;
    }

    Plan p = Plan(getPlan(planId));

    if(newPolicy == "bal"){
        int LifeQualityScore = p.getlifeQualityScore() + p.getlifeQualityScore_UC();
        int EconomyScore = p.getEconomyScore() + p.getEconomyScore_UC();
        int EnvironmentScore = p.getEnvironmentScore() + p.getEnvironmentScore_UC();
        this->getPlan(planId).setSelectionPolicy(new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore));
    }
    else if(newPolicy == "eco"){
        this->getPlan(planId).setSelectionPolicy(new EconomySelection());
    }
    else if(newPolicy == "nve"){
        this->getPlan(planId).setSelectionPolicy(new NaiveSelection());
    }
    else if(newPolicy == "env"){
        this->getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
    }

 }

void clearSettlements(){
    int list_size = static_cast<int>(this->settlements.size()); //casting size to int (otherwise can't compare i to size)
    for(int i = 0; i<list_size; i++){
        delete settlements.at(i);
    }
    this->settlements.clear();
}

void clearActionsLog(){
    int list_size = static_cast<int>(this->actionsLog.size()); //casting size to int (otherwise can't compare i to size)
    for(int i = 0; i<list_size; i++){
        delete actionsLog.at(i);
    }
    this->actionsLog.clear();
}


//rule of 3 additions
 Simulation(const Simulation& other);
 Simulation& operator=(const Simulation& other);

 Simulation:: ~Simulation (){
    clearSettlements();
    clearActionsLog();
 }


//rule of 5 additions
Simulation(Simulation&& other);
Simulation& operator=(Simulation&& other);


void Simulation::step(){
    for(Plan p: plans){
        p.step();
    }
}

void Simulation::close(){
    for(Plan p: plans){
        p.printStatus();
    }
    isRunning = false;
}

void Simulation::open(){

}




