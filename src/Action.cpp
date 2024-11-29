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
{
}

ActionStatus BaseAction::getStatus() const{
    return this->status;
}

void BaseAction::complete(){
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    cout << errorMsg << endl;}

const string& BaseAction::getErrorMsg() const{return this->errorMsg;}

//******************* SimulateStep *******************

//SimulateStep: constructor
SimulateStep::SimulateStep(const int numOfSteps):BaseAction(), numOfSteps(numOfSteps){}

//SimulateStep: copy constructor
SimulateStep::SimulateStep(const SimulateStep &other):errorMsg(other.errorMsg),status(other.status), numOfSteps(other.numOfSteps){}

//SimulateStep: methods
void SimulateStep::act(Simulation &simulation) {
    while(this->numOfSteps>0){
        simulation->step();
        this->numOfSteps--;
    }  
    complete(); 
}

const string SimulateStep::toString() const {return "step " << this->numOfSteps << " " << this->status;}

 SimulateStep* SimulateStep::clone() const {return new SimulateStep(*this);}

//******************* AddPlan *******************


//AddPlan: constructor
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):errorMsg("Cannot create this plan"), status(ActionStatus::ERROR), settlementName(settlementName), selectionPolicy(selectionPolicy){}

//AddPlan: copy constructor
AddPlan::AddPlan(const AddPlan &other):errorMsg(other.errorMsg),status(other.status), settlementName(other.settlementName), selectionPolicy(other.selectionPolicy){}

//AddPlan: methods
void AddPlan::act(Simulation &simulation){
    if (!Simulation.isSettlementExists(this->settlementName)){
        error(this->errorMsg);
    }
    else if(this->selectionpolicy == "bal"){
        simulation.addPlan(Simulation.getSettlement(this->settlementName),new BalancedSelection(0,0,0));
        complete();
    }
    else if(this->selectionpolicy == "eco"){
        simulation.addPlan(Simulation.getSettlement(this->settlementName),new EconomySelection());
        complete();
    }
    else if(this->selectionpolicy == "nve"){
        simulation.addPlan(Simulation.getSettlement(this->settlementName),new NaiveSelection());
        complete();
    }
    else if(this->selectionpolicy == "env"){
        simulation.addPlan(Simulation.getSettlement(this->settlementName),new SustainabilitySelection());
        complete();
    }
    else{
        error(this->errorMsg);
    }
}

AddPlan::toString(){return "Plan " << this->settlementName << " " << this->selectionPolicy << " " << this->status;}

AddPlan::clone(){return new AddPlan(*this);}


//******************* AddSettlement *******************

//AddSettlement: constructor
Addsettlement::Addsettlement(const string &settlementName,SettlementType settlementType):errorMsg("Settlement already exists"),status(ActionStatus::ERROR), settlementName(settlementName),settlementType(settlementType){}

//AddSettlement: copy constructor
AddSettlement::AddSettlement(const AddSettlement &other):errorMsg(other.errorMsg),status(other.status), settlementName(other.settlementName),settlementType(other.settlementType){}

//AddSettlement: methods
void AddSettlement::act(Simulation &simulation) {
    if (Simulation->addSettlement(new Settlement(this->settlementName, this->SettlementType))){
        complete();
    }
    else{error(this->errorMsg);}
}

const string AddSettlement::toString() const {return "settlement " << this->settlementName << " " << static_cast<int>(this->selectionPolicy)<< " " <<this->status;}

 AddSettlement* AddSettlement::clone() const {return new AddSettlement(*this);}


//******************* AddFacility *******************

//AddFacility: constructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
(errorMsg("Facility already exists"),status(ActionStatus::ERROR), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)){}

//AddFacility: copy constructor
AddFacility::AddFacility(const AddFacility &other):errorMsg(other.errorMsg),status(other.status), facilityName(other.facilityName), facilityCategory(other.facilityCategory), price(other.price), lifeQualityScore(other.lifeQualityScore), economyScore(other.economyScore), environmentScore(other.environmentScore){}

//AddFacility: methods
void AddFacility::act(Simulation &simulation) {
    if (Simulation->addFacility(FacilityType(this->facilityName, this->facilityCategory, this->price, this->lifeQualityScore, this->economyScore, this->environmentScore))){
        complete();
    }
    else{
        error(this->errorMsg);
    }
}

const string AddFacility::toString() const {return "facility " << this->facilityName << " " << static_cast<int>(this->facilityCategory)<< " " << this->price << " " << this->lifeQualityScore << " " << this-> economyScore<< " " << this->environmentScore << " " << this->status;}

 AddFacility* AddFacility::clone() const {return new AddFacility(*this);}

//******************* PrintPlanStatus *******************

//PrintPlanStatus: constructor
PrintPlanStatus::PrintPlanStatus(int planId):errorMsg(""), status(ActionStatus::ERROR), planId(planId) {}

//PrintPlanStatus: copy constructor
PrintPlanStatus::PrintPlanStatus(const PrintPlanStatus &other):errorMsg(other.errorMsg),status(other.status), planId(other.planId){}

//PrintPlanStatus: methods
void PrintPlanStatus::act(Simulation &simulation){
    if(isPlanExists(this->planId)){
        error();
    }
    else{
        simulation.getPlan(this->planId).printStatus();
        complete(); 
    }
    

}
PrintPlanStatus::toString(){return "PrintPlanStatus" << this->planId << " " << this->status ;}

PrintPlanStatus::clone(){return new PrintPlanStatus(*this);}

//******************* ChangePlanPolicy *******************

//ChangePlanPolicy: constructor
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):errorMsg("Cannot change selection policy"), status(ActionStatus::ERROR), planId(planId), newPolicy(newPolicy){}

//ChangePlanPolicy: copy constructor
ChangePlanPolicy::ChangePlanPolicy(const ChangePlanPolicy &other):errorMsg(other.errorMsg),status(other.status), planId(other.planId), newPolicy(other.newPolicy){}

//ChangePlanPolicy: methods
void ChangePlanPolicy::act(Simulation &simulation){
    if (isPlanExists(planId)){
        Plan& p = simulation.getPlan(planId);
        string prevPolicy = p.selectionPolicy->toStringFullName();
        if (simulation.changePlanPolicy(this->planId, this->newPolicy)){
            complete();
            cout << "planID: " << this->planId << endl;
            cout << "previouspolicy: " << prevPolicy << endl;
            cout << "newPolicy: " << p.selectionPolicy->toStringFullName() << endl;
        }
        else{
        error(this->errorMsg);
        }
    }
    else{
        error(this->errorMsg);
    }
}

ChangePlanPolicy::toString(){return "changePolicy:" << this->planId << " " << this->newPolicy << " " << this->status ;}

ChangePlanPolicy::clone(){return new ChangePlanPolicy(*this);}


//******************* PrintActionsLog *******************

//PrintActionsLog: constructor
PrintActionsLog::PrintActionsLog():errorMsg(""), status(ActionStatus::ERROR){}

//PrintActionsLog: copy constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):errorMsg(other.errorMsg),status(other.status){}

//PrintActionsLog: methods
void PrintActionsLog::act(Simulation &simulation){
    int log_size = simulation.actionsLog.size();
    for (i = 0; i < log_size; i++){
        cout << actionsLog.at(i).toString() << endl;
    }
    complete();
}
PrintActionsLog::toString(){return "log" << this->status ;}

PrintActionsLog::clone(){return new PrintActionsLog(*this);}



//******************* Close *******************

//close: constructor
Close::Close():errorMsg(""), status(ActionStatus::ERROR){}

//Close: copy constructor
Close::Close(const Close &other):errorMsg(other.errorMsg),status(other.status){}

//Close: methods
void Close::act(Simulation &simulation){
    simulation.close();
    complete();
}

Close::toString(){return "close " << this->status;}

Close::clone(){return new Close(*this);}

//******************* BackupSimulation *******************

//backupsimulation: constructor
BackupSimulation::BackupSimulation():errorMsg(""), status(ActionStatus::ERROR){}

//BackupSimulation: copy constructor
BackupSimulation::BackupSimulation(const BackupSimulation &other):errorMsg(other.errorMsg),status(other.status){}

//BackupSimulation: methods
void BackupSimulation::act(Simulation &simulation){
    if(backup){delete backup;}
    backup = Simulation.clone();
    complete();
    

}
BackupSimulation::toString(){return "BackupSimulation" << this->status ;}

BackupSimulation::clone(){return new BackupSimulation(*this);}


//******************* RestoreSimulation *******************

//restoresimulation: constructor
//building an assignment operator here for simulation
RestoreSimulation::RestoreSimulation():errorMsg("No backup available"), status(ActionStatus::ERROR){}

//RestoreSimulation: copy constructor
RestoreSimulation::RestoreSimulation(const RestoreSimulation &other):errorMsg(other.errorMsg),status(other.status){}

//RestoreSimulation: methods
void RestoreSimulation::act(Simulation &simulation){
    if(backup){
        simulation = *backup;
        complete();
    }
    else{
        error(this->errorMsg);
    }
    
    

}
RestoreSimulation::toString(){return "RestoreSimulation" << this->status ;}

RestoreSimulation::clone(){return new RestoreSimulation(*this);}