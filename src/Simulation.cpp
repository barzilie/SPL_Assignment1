#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
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
        return 1;
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
    isRunning = true;
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
  
    bool settExists = any_of(settlements.begin(), settlements.end(), [settlementName](const Settlement& sett) {
        return sett.getName() == settlementName;
    });
}

bool Simulation::isFacilityExists(const string &facilityName){

    bool facExists = any_of(facilitiesOptions.begin(), facilitiesOptions.end(), [facilityName](const FacilityType& fac) {
    return fac.getName() == facilityName;
    });
}

 bool Simulation::isPlanExists(const int planId){

    bool planExists = any_of(plans.begin(), plans.end(), [planId](const Plan& p) {
    return p.getID() == planId;
    });
 }

Settlement& Simulation::getSettlement(const string &settlementName){
    return *std::find_if(settlements.begin(), settlements.end(), [settlementName](const Settlement& sett){
        return sett.getName() == settlementName
        });
}

Plan& Simulation::getPlan(const int planId){
    return *std::find_if(plans.begin(), plans.end(), [planId](const Plan& p){
    return p.getID() == planId
    });
 }

 bool Simulation::changePlanPolicy(const int planId, const string &newPolicy){
    if (!isPlanExists(planId)){
        return false;
    }
    if(newPolicy != "nve" & )
    string planSP = this->getPlan(planId).getSelectionPolicy();
    if (planSP == newPolicy){
        return false;
    }

    switch(newPolicy){
        Plan p = Plan(getPlan(planId));
        case "bal":
            int LifeQualityScore = p.getlifeQualityScore() + p.getLQS_underConstruction();
            int EconomyScore = p.getEconomyScore() + p.getECS_underConstruction();
            int EnvironmentScore = p.getEnvironmentScore() + p.getENS_underConstruction();
            this->getPlan(planId).setSelectionPolicy(new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore));
        case "eco":
            this->getPlan(planId).setSelectionPolicy(new EconomySelection());
        case "nve":
            this->getPlan(planId).setSelectionPolicy(new NaiveSelection());
        case "env":
            this->getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
    }

 }


//rule of 3 additions
 Simulation(const Simulation& other);
 Simulation& operator=(const Simulation& other);

 Simulation:: ~Simulation (){
    for(BaseAction* ba: actionsLog){
        delete ba;
    }
    for(Settlement* s: settlements){
        delete s;
    }
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


