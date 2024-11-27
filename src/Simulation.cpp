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
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
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
Simulation::Simulation(const string &configFilePath):isRunning(true), planCounter(0), actionsLog{}, plans{}, settlements{}, facilitiesOptions{} {
    std::ifstream file("your_file.txt");
    //
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
                    const std::array<SettlementType, 3> types = {SettlementType::VILLAGE, SettlementType::CITY, SettlementType::METROPOLIS};
                    settlements.push_back(Settlement::Settlement(args[1],types[stoi(args[2])]));
                //# facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
                case "facility":
                    const std::array<FacilityCategory, 3> category = {FacilityCategory::LIFE_QUALITY, FacilityCategory::ECONOMY, FacilityCategory::ENVIRONMENT};
                    facilitiesOptions.push_back(FacilityType::FacilityType(args[1], category[stoi(args[2])],stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6])));
                //# plan <settlement_name> <selection_policy>
                case "plan":                    
                    switch(args[1]){
                        case "bal":
                            plans.push_back(Plan::Plan(planCounter, args[0], BalancedSelection::BalancedSelection(0,0,0), facilitiesOptions));
                        case "eco":
                            plans.push_back(Plan::Plan(planCounter, args[0], EconomySelection::EconomySelection(), facilitiesOptions));
                        case "nve":
                            plans.push_back(Plan::Plan(planCounter, args[0], NaiveSelection::NaiveSelection(), facilitiesOptions));
                        case "env":
                            plans.push_back(Plan::Plan(planCounter, args[0], SustainabilitySelection::SustainabilitySelection(), facilitiesOptions));
                    }
                    planCounter++;
                    //seems like we should use some implementation of the rule of 5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }
        } 
    } 
}