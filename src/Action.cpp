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
void BaseAction::complete(){this->status = "COMPLETED"}
void BaseAction::error(this->errorMsg){this->status = "ERROR";
    std::cout << errorMsg << std::endl;}

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
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):errorMsg("”Cannot create this plan”"), status("RUNNING"), settlementName(settlementName), selectionPolicy(selectionPolicy){}

//AddPlan: copy constructor
AddPlan::AddPlan(const AddPlan &other):errorMsg(other.errorMsg),status(other.status), settlementName(other.settlementName), selectionPolicy(other.selectionPolicy){}

//AddPlan: methods
AddPlan::act(Simulation &simulation){
    if (!Simulation.isSettlementExists(this->settlementName)){
        error();
    }
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

AddPlan::toString()

AddPlan::clone()


class AddSettlement : public BaseAction {
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};

class PrintPlanStatus: public BaseAction {
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
        const int planId;
        const string newPolicy;
};


class PrintActionsLog : public BaseAction {

};

class Close : public BaseAction {

};

class BackupSimulation : public BaseAction {

};


class RestoreSimulation : public BaseAction {

};