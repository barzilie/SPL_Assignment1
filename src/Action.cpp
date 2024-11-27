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

class SimulateStep : public BaseAction {

    public:
        const int numOfSteps;
};

class AddPlan : public BaseAction {
        const string settlementName;
        const string selectionPolicy;
};


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