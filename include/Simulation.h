#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName); //added function
        bool isPlanExists(const int planId); //added function
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planId);
        bool changePlanPolicy(const int planId, const string &newPolicy); //added fuction
        void clearSettlements(); //added fuction
        void clearActionsLog(); //added fuction

        //rule of 3 additions
        Simulation(const Simulation& other);
        Simulation& operator=(const Simulation& other);
        ~Simulation(); 

        //rule of 5 additions
        Simulation(Simulation&& other);
        Simulation& operator=(Simulation&& other);

        void step();
        void close();
        void open();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};