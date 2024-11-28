#pragma once
#include <string>
#include <vector>
#include "Simulation.h"
enum class SettlementType;
enum class FacilityCategory;

enum class ActionStatus{
    COMPLETED, ERROR
};

class BaseAction{
    public:
        BaseAction();
        //HAS TO ADD GET STATUS FOR ALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ActionStatus getStatus() const;
        virtual void act(Simulation& simulation)=0;
        virtual const string toString() const=0;
        virtual BaseAction* clone() const = 0;
        virtual ~BaseAction() = default;

    protected:
        void complete();
        void error(string errorMsg);
        const string &getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};
//BaseAction: protected methods
void BaseAction::complete(){
    //add to changelog here
    this->status = "COMPLETED"
    }
//ERROR method may not require a change if error is the default status, may just call for print
void BaseAction::error(
    this->errorMsg){this->status = "ERROR";
    //add to changelog here
    std::cout << errorMsg << std::endl;}
    //DONT FORGET: create add to changelog method in simulation

//SimulateStep: constructor
SimulateStep::SimulateStep(const int numOfSteps):errorMsg(""),status("RUNNING"), numOfSteps(numOfSteps){}

//SimulateStep: copy constructor
SimulateStep::SimulateStep(const SimulateStep &other):errorMsg(other.errorMsg),status(other.status), numOfSteps(other.numOfSteps){}

//SimulateStep: methods
void SimulateStep::act(Simulation &simulation) {
    while(this->numOfSteps>0){
        simulation->step();
        this->numOfSteps--;
    }  
    this->complete(); 
}

const string SimulateStep::toString() const {return "step " << this->numOfSteps << " " << this->status;}

 SimulateStep* SimulateStep::clone() const {return new SimulateStep(*this);}

//AddPlan: constructor
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):errorMsg("Cannot create this plan"), status("RUNNING"), settlementName(settlementName), selectionPolicy(selectionPolicy){}

//AddPlan: copy constructor
AddPlan::AddPlan(const AddPlan &other):errorMsg(other.errorMsg),status(other.status), settlementName(other.settlementName), selectionPolicy(other.selectionPolicy){}

//AddPlan: methods
void AddPlan::act(Simulation &simulation){
    if (!Simulation.isSettlementExists(this->settlementName)){error();}
    switch(this->selectionPolicy){
        case "bal":
            simulation.addPlan(Simulation.getSettlement(this->settlementName),new BalancedSelection());
        case "eco":
            simulation.addPlan(Simulation.getSettlement(this->settlementName),new EconomySelection());
        case "nve":
            simulation.addPlan(Simulation.getSettlement(this->settlementName),new NaiveSelection());
        case "env":
            simulation.addPlan(Simulation.getSettlement(this->settlementName),new SustainabilitySelection());
        default:
            error();
    }
    complete();
}

AddPlan::toString(){return "Plan " << this->settlementName << this->selectionPolicy << this->status;}

AddPlan::clone(){return new AddPlan(*this);}


class AddSettlement : public BaseAction {
        const string settlementName;
        const SettlementType settlementType;
};
//AddSettlement: constructor
Addsettlement::Addsettlement(const string &settlementName,SettlementType settlementType):errorMsg("Settlement already exists"),status("RUNNING"), settlementName(settlementName),settlementType(settlementType){}

//AddSettlement: copy constructor
AddSettlement::AddSettlement(const AddSettlement &other):errorMsg(other.errorMsg),status(other.status), settlementName(other.settlementName),settlementType(other.settlementType){}

//AddSettlement: methods
void AddSettlement::act(Simulation &simulation) {
    if (Simulation->addSettlement(this->settlementName, this->SettlementType)){complete();}
    else{error();}
}

const string AddSettlement::toString() const {return "settlement " << this->settlementName << " " << static_cast<int>(this->selectionPolicy)<< " " <<this->status;}

 AddSettlement* AddSettlement::clone() const {return new AddSettlement(*this);}


class AddFacility : public BaseAction {
    public:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};
//AddFacility: constructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
(errorMsg("Facility already exists"),status("RUNNING"), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)){}

//AddFacility: copy constructor
AddFacility::AddFacility(const AddFacility &other):errorMsg(other.errorMsg),status(other.status), facilityName(other.facilityName), facilityCategory(other.facilityCategory), price(other.price), lifeQualityScore(other.lifeQualityScore), economyScore(other.economyScore), environmentScore(other.environmentScore){}

//AddFacility: methods
void AddFacility::act(Simulation &simulation) {
    if (Simulation->AddFacility(this->settlementName, this->SettlementType)){complete();}
    else{error();}
}

const string AddFacility::toString() const {return "facility " << this->facilityName << " " << static_cast<int>(this->facilityCategory)<< " " << this->price << " " << this->lifeQualityScore << " " << this-> economyScore<< " " << this->environmentScore << " " << this->status;}

 AddFacility* AddFacility::clone() const {return new AddFacility(*this);}

class PrintPlanStatus: public BaseAction {
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
        const int planId;
        const string newPolicy;
        const string prevPolicy;
};

//ChangePlanPolicy: constructor
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):errorMsg("Cannot change selection policy"), status("RUNNING"), planId(planId), newPolicy(newPolicy), prevPolicy(""){}

//ChangePlanPolicy: copy constructor
ChangePlanPolicy::ChangePlanPolicy(const ChangePlanPolicy &other):errorMsg(other.errorMsg),status(other.status), planId(other.planId), newPolicy(other.newPolicy), prevPolicy(other.prevPolicy){}

//ChangePlanPolicy: methods
void ChangePlanPolicy::act(Simulation &simulation){
    if (simulation.changePlanPolicy(this->planId, this->newPolicy)){complete();}
    else{error();}}
    //ASK FROM AMIT TO RETURN PREV POLICY AND CHANGE ACT ACCORFINGLY

ChangePlanPolicy::toString(){return "planID:" << this->planId << "/n" << "previousPolicy:" << this->prevPolicy << "/n" << "newPolicy: " << this->newPolicy ;}
//CONSIDER CHANGING THE RETURN

ChangePlanPolicy::clone(){return new ChangePlanPolicy(*this);}




class PrintActionsLog : public BaseAction {
};
//PrintActionsLog: constructor
PrintActionsLog::PrintActionsLog():errorMsg(""), status(""){}

//PrintActionsLog: copy constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):errorMsg(other.errorMsg),status(other.status){}

//PrintActionsLog: methods
void PrintActionsLog::act(Simulation &simulation){
    //loop through actions log simulations, print every to string of action. 
    complete();

}
PrintActionsLog::toString(){return "Log" << this->status ;}

PrintActionsLog::clone(){return new PrintActionsLog(*this);}

class Close : public BaseAction {

};
//close: constructor
Close::Close():errorMsg(""), status(""){}

//Close: copy constructor
Close::Close(const Close &other):errorMsg(other.errorMsg),status(other.status){}

//Close: methods
void Close::act(Simulation &simulation){
    //loops through simulation.plan.printStatus()
    simulation.close();
    complete();
    

}
Close::toString(){return "close" << this->status ;}

Close::clone(){return new Close(*this);}

class BackupSimulation : public BaseAction {

};
//backupsimulation: constructor
BackupSimulation::BackupSimulation():errorMsg(""), status(""){}

//BackupSimulation: copy constructor
BackupSimulation::BackupSimulation(const BackupSimulation &other):errorMsg(other.errorMsg),status(other.status){}

//BackupSimulation: methods
void BackupSimulation::act(Simulation &simulation){
    extern Simulation* backup = Simulation.clone();
    complete();
    

}
BackupSimulation::toString(){return "BackupSimulation" << this->status ;}

BackupSimulation::clone(){return new BackupSimulation(*this);}


class RestoreSimulation : public BaseAction {

};
//restoresimulation: constructor
//building an assignment operator here for simulation
RestoreSimulation::RestoreSimulation():errorMsg("No backup available"), status(""){}

//RestoreSimulation: copy constructor
RestoreSimulation::RestoreSimulation(const RestoreSimulation &other):errorMsg(other.errorMsg),status(other.status){}

//RestoreSimulation: methods
void RestoreSimulation::act(Simulation &simulation){
    if(extern Simulation* backup){
        simulation = *backup;
        complete();
    }
    error();
    

}
RestoreSimulation::toString(){return "RestoreSimulation" << this->status ;}

RestoreSimulation::clone(){return new RestoreSimulation(*this);}