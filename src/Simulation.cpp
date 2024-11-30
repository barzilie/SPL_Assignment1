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
#include "Action.h"
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
    string line;
    while(getline(file, line)) {
        if(line[0]!='#'){
            vector<string> args = Auxiliary::parseArguments(line);
                //# settlement <settlement_name> <settlement_type> 
                if(args[0] == "settlement"){
                    settlements.push_back(new Settlement(args[1],static_cast<SettlementType>(stoi(args[2]))));

                }
                //# facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
                if(args[0] == "facility"){
                    FacilityType ft = FacilityType(args[1], static_cast<FacilityCategory>(stoi(args[2])),stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
                    facilitiesOptions.push_back(ft);
                }
                //# plan <settlement_name> <selection_policy>
                if(args[0] == "plan"){
                        if(args[2] == "bal"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new BalancedSelection(0,0,0), this->facilitiesOptions));
                            this->planCounter++;
                        }
                        else if(args[2] == "eco"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new EconomySelection(), this->facilitiesOptions));
                            this->planCounter++;
                        }
                        else if(args[2] == "nve"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new NaiveSelection(), this->facilitiesOptions));
                            this->planCounter++;
                        }
                        else if(args[2] == "env"){
                            plans.push_back(Plan(this->planCounter, this->getSettlement(args[1]), new SustainabilitySelection(), this->facilitiesOptions));
                            this->planCounter++;
                        }     
                    }
                }
        } 
}
//seems like we should use some implementation of the rule of 5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


void Simulation::start(){
    open();
    cout << "The simulation has started" << endl;
    while(isRunning){
        string userInput;
        getline(cin, userInput);
        vector<string> args = Auxiliary::parseArguments(userInput);
        if(args[0] == "step"){
            SimulateStep s = SimulateStep(stoi(args[1]));
            s.act(*this);
            addAction(s.clone());
        }

        else if(args[0] == "plan"){
            AddPlan s = AddPlan(args[1], args[2]);
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "settlement"){
            AddSettlement s = AddSettlement(args[1], static_cast<SettlementType>(stoi(args[2])));
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "facility"){
            AddFacility s = AddFacility(args[1], static_cast<FacilityCategory>(stoi(args[2])), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "planStatus"){
            PrintPlanStatus s = PrintPlanStatus(stoi(args[1]));
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "changePolicy"){
            ChangePlanPolicy s = ChangePlanPolicy(stoi(args[1]), args[2]);
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "log"){
            PrintActionsLog s = PrintActionsLog();
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "close"){
            Close s = Close();
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "backup"){
            BackupSimulation s = BackupSimulation();
            s.act(*this);
            addAction(s.clone());
        }
        else if(args[0] == "restore"){
            RestoreSimulation s = RestoreSimulation();
            s.act(*this);
            addAction(s.clone());
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan p = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
    plans.push_back(p);
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
    //if settlement is already in settlements vector: function returns false, otherwise add it to settlements and return true
    bool exists = isSettlementExists(settlement->getName());
    if (!exists){
        settlements.push_back(settlement);
    }
    return !exists; //if already exist, we will not add the sett and return false
    
}

bool Simulation::addFacility(FacilityType facility){

    //if facility is already in facilitiesOptions vector: returns false, otherwise add it to facilitiesOptions and return true
    bool exists = isFacilityExists(facility.getName());
    if(!exists){
        facilitiesOptions.push_back(facility);
    }
    return !exists; //if already exist, we will not add the fac and return false

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
    int list_size = static_cast<int>(plans.size()); //casting size to int (otherwise can't compare i to size)
    for(int i = 0; i<list_size; i++){
        if(plans.at(i).getID() == planId){
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
    Settlement* s = nullptr;
    return *s;
}

Plan& Simulation::getPlan(const int planId){
    for(Plan& p: plans){
        if(p.getID() == planId){
            return p;
        }
    }
    Plan* p = nullptr;
    return *p;
 }

 bool Simulation::changePlanPolicy(const int planId, const string &newPolicy){

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

    return true;

 }

void Simulation::clearSettlements(){
    int list_size = static_cast<int>(settlements.size()); //casting size to int (otherwise can't compare i to size)
    for(int i = 0; i<list_size; i++){
        delete settlements.at(i);
    }
    settlements.clear();
}

void Simulation::clearActionsLog(){
    int list_size = static_cast<int>(this->actionsLog.size()); //casting size to int (otherwise can't compare i to size)
    for(int i = 0; i<list_size; i++){
        delete actionsLog.at(i);
    }
    this->actionsLog.clear();
}

void Simulation::printActionsLog() const{
    int log_size = this->actionsLog.size();
    for (int i = 0; i < log_size; i++){
        cout << this->actionsLog.at(i)->toString() << endl;
    }
}

Simulation *Simulation::clone() const{
    return new Simulation(*this);
}

//rule of 3 additions

 Simulation::Simulation(const Simulation& other): isRunning(other.isRunning), planCounter(other.planCounter), actionsLog{}, plans{}, settlements{}, facilitiesOptions(other.facilitiesOptions){
    int actions_size = static_cast<int>(other.actionsLog.size()); //casting size to int (otherwise can't compare i to size)
    int settlements_size = static_cast<int>(other.settlements.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i<actions_size; i++){
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
    for(int i=0; i<settlements_size; i++){
        //maybe crete clone instead of making "new" statement
        settlements.push_back(new Settlement(*(other.settlements.at(i))));
    }

    int plans_size = static_cast<int>(other.plans.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i< plans_size; i++){
        Settlement& settle = getSettlement(other.plans.at(i).getSettlementName());
        plans.push_back(Plan(other.plans.at(i), settle));
    }    

}
Simulation& Simulation::operator=(const Simulation& other){
    if(&other != this){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans.clear();
        facilitiesOptions.clear();
        clearSettlements();
        clearActionsLog();

        int settlements_size = static_cast<int>(other.settlements.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i<settlements_size; i++){
            //maybe crete clone instead of making "new" statement
            settlements.push_back(new Settlement(*(other.settlements.at(i))));
        } 

        int plans_size = static_cast<int>(other.plans.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i< plans_size; i++){
            Settlement& settle = getSettlement(other.plans.at(i).getSettlementName());
            plans.push_back(Plan(other.plans.at(i), settle));
        }

        int facilities_size = static_cast<int>(other.facilitiesOptions.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i<facilities_size; i++){
            facilitiesOptions.push_back(FacilityType(other.facilitiesOptions.at(i)));
        }

        int actions_size = static_cast<int>(other.actionsLog.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i<actions_size; i++){
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }
  
    }
    return *this;
}

Simulation::~Simulation (){
    clearSettlements();
    clearActionsLog();
}

//rule of 5 additions

// move copy constructor
Simulation::Simulation(Simulation&& other): isRunning(other.isRunning), planCounter(other.planCounter), actionsLog{}, plans{}, settlements{}, facilitiesOptions(other.facilitiesOptions){
    int actions_size = static_cast<int>(other.actionsLog.size()); 
    int settlements_size = static_cast<int>(other.settlements.size()); 
    for(int i=0; i<actions_size; i++){
        actionsLog.push_back(other.actionsLog.at(i));
        other.actionsLog.at(i) = nullptr;
    }
    for(int i=0; i<settlements_size; i++){
        settlements.push_back(other.settlements.at(i));
        other.settlements.at(i) = nullptr;
    }
    int plans_size = static_cast<int>(other.plans.size()); //casting size to int (otherwise can't compare i to size)
    for(int i=0; i< plans_size; i++){
        Settlement& settle = getSettlement(other.plans.at(i).getSettlementName());
        plans.push_back(Plan(other.plans.at(i), settle));
    }
}

// move copy assignment operator
Simulation& Simulation::operator=(Simulation&& other){
        if(&other != this){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans.clear();
        facilitiesOptions.clear();
        clearSettlements();
        clearActionsLog();

        int settlements_size = static_cast<int>(other.settlements.size()); 
        for(int i=0; i<settlements_size; i++){
            settlements.push_back(other.settlements.at(i));
            other.settlements.at(i) = nullptr;

        } 

        int plans_size = static_cast<int>(other.plans.size()); 
        for(int i=0; i< plans_size; i++){
            Settlement& settle = getSettlement(other.plans.at(i).getSettlementName());
            plans.push_back(Plan(other.plans.at(i), settle));
        }

        int facilities_size = static_cast<int>(other.facilitiesOptions.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i<facilities_size; i++){
            facilitiesOptions.push_back(FacilityType(other.facilitiesOptions.at(i)));
        }        

        int actions_size = static_cast<int>(other.actionsLog.size()); //casting size to int (otherwise can't compare i to size)
        for(int i=0; i<actions_size; i++){
            actionsLog.push_back(other.actionsLog.at(i));
            other.actionsLog.at(i) = nullptr;
        }
  
    }
    return *this;

}


void Simulation::step(){
    for(Plan& p: plans){
        p.step();
    }
}

void Simulation::close(){
    for(Plan& p: plans){
        p.printForClose();
    }
    this->isRunning = false;
}

void Simulation::open(){
    this->isRunning = true;
}




