#pragma once
#include <string>
#include <vector>
#include "Simulation.h"
#include "Action.h"
#include <iostream>
using namespace std;
enum class SettlementType;
enum class FacilityCategory;
extern Simulation* backup;

//******************* BaseAction *******************

BaseAction::BaseAction():errorMsg(""), status(ActionStatus::ERROR){}

BaseAction::BaseAction(const string msg):errorMsg(msg), status(ActionStatus::ERROR){}

BaseAction::BaseAction(const BaseAction &other): errorMsg(other.errorMsg), status(other.status){}

ActionStatus BaseAction::getStatus() const{
    return this->status;
}

void BaseAction::complete(){
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    cout << errorMsg << endl;
}

const string& BaseAction::getErrorMsg() const{
    return this->errorMsg;
}

const string BaseAction::getStringStatus() const{
    if(getStatus() == ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERROR"; 
    }
}


//******************* SimulateStep *******************

//SimulateStep: constructor
SimulateStep::SimulateStep(const int numOfSteps):BaseAction(), numOfSteps(numOfSteps){}

//SimulateStep: copy constructor
SimulateStep::SimulateStep(const SimulateStep &other):BaseAction(other), numOfSteps(other.numOfSteps){}

//SimulateStep: methods
void SimulateStep::act(Simulation &simulation) {
    int stepCount = this->numOfSteps;
    while(stepCount>0){
        simulation.step();
        stepCount--;
    }  
    complete(); 
}

const string SimulateStep::toString() const {
    string result =  "step " + to_string(this->numOfSteps) + " " + this->getStringStatus();
return result;
}

 SimulateStep* SimulateStep::clone() const {return new SimulateStep(*this);}

//******************* AddPlan *******************

//AddPlan: constructor
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):BaseAction("Cannot create this plan"), settlementName(settlementName), selectionPolicy(selectionPolicy){}

//AddPlan: copy constructor
AddPlan::AddPlan(const AddPlan &other):BaseAction(other), settlementName(other.settlementName), selectionPolicy(other.selectionPolicy){}

//AddPlan: methods
void AddPlan::act(Simulation &simulation){
    if (!simulation.isSettlementExists(this->settlementName)){
       error(this->getErrorMsg());
    }
    else if(this->selectionPolicy == "bal"){
        simulation.addPlan(simulation.getSettlement(this->settlementName),new BalancedSelection(0,0,0));
        complete();
    }
    else if(this->selectionPolicy == "eco"){
        simulation.addPlan(simulation.getSettlement(this->settlementName),new EconomySelection());
        complete();
    }
    else if(this->selectionPolicy == "nve"){
        simulation.addPlan(simulation.getSettlement(this->settlementName),new NaiveSelection());
        complete();
    }
    else if(this->selectionPolicy == "env"){
        simulation.addPlan(simulation.getSettlement(this->settlementName),new SustainabilitySelection());
        complete();
    }
    else{
       error(this->getErrorMsg());
    }
}

const string AddPlan::toString() const{
    string result =  "plan " + this->settlementName + " " + this->selectionPolicy + " " + this->getStringStatus();
    return result;
}

AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}


//******************* AddSettlement *******************

//AddSettlement: constructor
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType): BaseAction("Settlement already exists"), settlementName(settlementName),settlementType(settlementType){}

//AddSettlement: copy constructor
AddSettlement::AddSettlement(const AddSettlement &other):BaseAction(other), settlementName(other.settlementName),settlementType(other.settlementType){}

//AddSettlement: methods
void AddSettlement::act(Simulation &simulation) {
    if (simulation.addSettlement(new Settlement(this->settlementName, this->settlementType))){
        complete();
    }
    else{error(this->getErrorMsg());}
}

const string AddSettlement::toString() const {
    string result =  "settlement " + this->settlementName + " " + to_string(static_cast<int>(this->settlementType)) + " " + this->getStringStatus();
    return result;
}

 AddSettlement* AddSettlement::clone() const {return new AddSettlement(*this);}


//******************* AddFacility *******************

//AddFacility: constructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
BaseAction("Facility already exists"), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

//AddFacility: copy constructor
AddFacility::AddFacility(const AddFacility &other):BaseAction(other), facilityName(other.facilityName), facilityCategory(other.facilityCategory), price(other.price), lifeQualityScore(other.lifeQualityScore), economyScore(other.economyScore), environmentScore(other.environmentScore){}

//AddFacility: methods
void AddFacility::act(Simulation &simulation) {
    if (simulation.addFacility(FacilityType(this->facilityName, this->facilityCategory, this->price, this->lifeQualityScore, this->economyScore, this->environmentScore))){
        complete();
    }
    else{
       error(this->getErrorMsg());
    }
}

const string AddFacility::toString() const {
    string result = "facility " + this->facilityName + " " + to_string(static_cast<int>(this->facilityCategory)) + " " + to_string(this->price) + " " + to_string(this->lifeQualityScore) + " " + to_string(this-> economyScore) + " " + to_string(this->environmentScore) + " " + this->getStringStatus();
    return result;
}
 AddFacility* AddFacility::clone() const {return new AddFacility(*this);}

//******************* PrintPlanStatus *******************

//PrintPlanStatus: constructor
PrintPlanStatus::PrintPlanStatus(int planId):BaseAction("Plan doesn't exist"), planId(planId) {}

//PrintPlanStatus: copy constructor
PrintPlanStatus::PrintPlanStatus(const PrintPlanStatus &other):BaseAction(other), planId(other.planId){}

//PrintPlanStatus: methods
void PrintPlanStatus::act(Simulation &simulation){
    if(!simulation.isPlanExists(this->planId)){
        error(this->getErrorMsg());
    }
    else{
        simulation.getPlan(this->planId).printStatus();
        complete(); 
    }
}

const string PrintPlanStatus::toString() const {
    string result = "PrintPlanStatus " + to_string(this->planId) + " " + this->getStringStatus();
    return result;
}

PrintPlanStatus* PrintPlanStatus::clone() const {return new PrintPlanStatus(*this);}

//******************* ChangePlanPolicy *******************

//ChangePlanPolicy: constructor
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):BaseAction("Cannot change selection policy"), planId(planId), newPolicy(newPolicy){}

//ChangePlanPolicy: copy constructor
ChangePlanPolicy::ChangePlanPolicy(const ChangePlanPolicy &other):BaseAction(other), planId(other.planId), newPolicy(other.newPolicy){}

//ChangePlanPolicy: methods
void ChangePlanPolicy::act(Simulation &simulation){
    if (simulation.isPlanExists(planId)){
        Plan& p = simulation.getPlan(planId);
        string prevPolicy = p.getSelectionPolicyFN();
        if (simulation.changePlanPolicy(this->planId, this->newPolicy)){
            complete();
            cout << "planID: " << this->planId << endl;
            cout << "previouspolicy: " << prevPolicy << endl;
            cout << "newPolicy: " << p.getSelectionPolicyFN() << endl;
        }
        else{
       error(this->getErrorMsg());
        }
    }
    else{
       error(this->getErrorMsg());
    }
}

const string ChangePlanPolicy::toString() const {
    string result =  "changePolicy:" + to_string(this->planId) + " " + this->newPolicy + " " + this->getStringStatus() ;
    return result;
}

ChangePlanPolicy* ChangePlanPolicy::clone() const{return new ChangePlanPolicy(*this);}


//******************* PrintActionsLog *******************

//PrintActionsLog: constructor
PrintActionsLog::PrintActionsLog():BaseAction(){}

//PrintActionsLog: copy constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other){}

//PrintActionsLog: methods
void PrintActionsLog::act(Simulation &simulation){
    simulation.printActionsLog();
    complete();
}

const string PrintActionsLog::toString() const{
    string result =  "log" + this->getStringStatus();
    return result;
}

PrintActionsLog* PrintActionsLog::clone() const {return new PrintActionsLog(*this);}


//******************* Close *******************

//close: constructor
Close::Close():BaseAction(){}

//Close: copy constructor
Close::Close(const Close &other):BaseAction(other){}

//Close: methods
void Close::act(Simulation &simulation){
    simulation.close();
    complete();
}

const string Close::toString() const {
    string result = "close " + this->getStringStatus();
    return result;
}

Close* Close::clone() const{return new Close(*this);}

//******************* BackupSimulation *******************

//backupsimulation: constructor
BackupSimulation::BackupSimulation():BaseAction(){}

//BackupSimulation: copy constructor
BackupSimulation::BackupSimulation(const BackupSimulation &other):BaseAction(other){}

//BackupSimulation: methods
void BackupSimulation::act(Simulation &simulation){
    if(backup){delete backup;}
    backup = simulation.clone();
    complete();
}

const string BackupSimulation::toString() const {
    string result =  "Backup " + this->getStringStatus();
    return result;
}

BackupSimulation* BackupSimulation::clone() const{return new BackupSimulation(*this);}


//******************* RestoreSimulation *******************

//RestoreSimulation: constructor
RestoreSimulation::RestoreSimulation():BaseAction("No backup available"){}

//RestoreSimulation: copy constructor
RestoreSimulation::RestoreSimulation(const RestoreSimulation &other):BaseAction(other){}

//RestoreSimulation: methods
void RestoreSimulation::act(Simulation &simulation){
    if(backup){
        simulation = *backup;
        complete();
    }
    else{
       error(this->getErrorMsg());
    }
}

const string RestoreSimulation::toString() const {
    string result =  "Restore " + this->getStringStatus();
    return result;
}

RestoreSimulation* RestoreSimulation::clone() const {return new RestoreSimulation(*this);}