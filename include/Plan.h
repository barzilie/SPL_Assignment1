#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;
using namespace std;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &other); //added function
        Plan(const Plan& other, const Settlement &settlement); //added function
        Plan& operator=(const Plan& other) = delete; 
        Plan(Plan &&other); //added function
        Plan& operator=(Plan&& other) = delete;
        virtual ~Plan(); //added function
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        void printForClose(); //added function
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getID() const; //added function
        const string getSelectionPolicy() const; //added function
        const string getSelectionPolicyFN() const; //added function

        //new getters that return the scores of the under construction facilities
        const int getlifeQualityScore_UC() const;
        const int getEconomyScore_UC() const;
        const int getEnvironmentScore_UC() const;
        const string getSettlementName() const; //added function

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; 
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};