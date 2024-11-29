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
                //# settlement <settlement_name> <settlement_type> 
                if(args[0] == "settlement"){
                    //change according to cast - 2 next lines 
                    const std::array<SettlementType, 3> types = {SettlementType::VILLAGE, SettlementType::CITY, SettlementType::METROPOLIS};
                    settlements.push_back(new Settlement(args[1],types[stoi(args[2])]));

                }
                //# facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
                if(args[0] == "facility"){
                    //change according to cast - 2 next lines 
                    const std::array<FacilityCategory, 3> category = {FacilityCategory::LIFE_QUALITY, FacilityCategory::ECONOMY, FacilityCategory::ENVIRONMENT};
                    facilitiesOptions.push_back(FacilityType(args[1], category[stoi(args[2])],stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6])));
                }
                //# plan <settlement_name> <selection_policy>
                if(args[0] == "plan"){
                        if(args[2] == "bal"){
                            //required to create get settlement that returns refernce to args[1]!!!!!!!!!!!!!!!!!!!!!!
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new BalancedSelection(0,0,0), facilitiesOptions));
                        }
                        if(args[2] == "eco"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new EconomySelection(), facilitiesOptions));
                        }
                        if(args[2] == "eco"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new NaiveSelection(), facilitiesOptions));
                        }
                        if(args[2] == "eco"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new SustainabilitySelection(), facilitiesOptions));
                        }     
                    }
                this->planCounter++;
                }
        } 
}
//seems like we should use some implementation of the rule of 5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


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
    /*
        bool settExists = any_of(settlements.begin(), settlements.end(), [settlementName](const Settlement& sett){
        return sett.getName() == settlementName;
    });
    */

}

bool Simulation::isFacilityExists(const string &facilityName){
    for(FacilityType f: facilitiesOptions){
        if(f.getName() == facilityName){
            return true;
        }
    }
    return false;
    /*
        bool facExists = any_of(facilitiesOptions.begin(), facilitiesOptions.end(), [facilityName](const FacilityType& fac) {
    return fac.getName() == facilityName;
    });
    */

}

 bool Simulation::isPlanExists(const int planId){
    for(Plan p: plans){
        if(p.getID() == planId){
            return true;
        }
    }
    return false;

    /*
       bool planExists = any_of(plans.begin(), plans.end(), [planId](const Plan& p) {
    return p.getID() == planId;
    });
    */
 
 }

Settlement& Simulation::getSettlement(const string &settlementName){

    for(Settlement* s: settlements){
        if(s->getName() == settlementName){
            return *s;
        }
    }
    Settlement* s = nullptr;
    return *s;
    // what can i do here?
    /*
        return *std::find_if(settlements.begin(), settlements.end(), [settlementName](const Settlement& sett){
        return sett.getName() == settlementName
        });
    */

}

Plan& Simulation::getPlan(const int planId){
    for(Plan& p: plans){
        if(p.getID() == planId){
            return p;
        }
    }
    Plan* p = nullptr;
    return *p;
    /*
        return *std::find_if(plans.begin(), plans.end(), [planId](const Plan& p){
    return p.getID() == planId
    });
    */

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

/*

    switch(newPolicy){
        Plan p = Plan(getPlan(planId));
        case "bal":
            int LifeQualityScore = p.getlifeQualityScore() + p.getlifeQualityScore_UC();
            int EconomyScore = p.getEconomyScore() + p.getEconomyScore_UC();
            int EnvironmentScore = p.getEnvironmentScore() + p.getEnvironmentScore_UC();
            this->getPlan(planId).setSelectionPolicy(new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore));
        case "eco":
            this->getPlan(planId).setSelectionPolicy(new EconomySelection());
        case "nve":
            this->getPlan(planId).setSelectionPolicy(new NaiveSelection());
        case "env":
            this->getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
    }

*/



 }


//rule of 3 additions
 Simulation(const Simulation& other);
 Simulation& operator=(const Simulation& other);

 Simulation:: ~Simulation (){
    //consider writing with a for loop
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


