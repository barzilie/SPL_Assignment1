#pragma once
#include <vector>
#include <algorithm>
#include <climits>
#include "Facility.h"
using std::vector;
using namespace std;


class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual const string toStringFullName() const = 0; //added function
        virtual SelectionPolicy *clone() const = 0;
        virtual void incrementScores(int LQ, int EC, int EN) = 0; //added function (relevant only for bal)
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        NaiveSelection(const NaiveSelection &ns); //added function- copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override; //added function
        NaiveSelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override; //added function
        ~NaiveSelection() override = default;
    private:
        int lastSelectedIndex; //actually, this is the next index to be chosen (or at least tested), and not the last selected one
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection(const BalancedSelection &bs); //added function- new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override; //added function
        BalancedSelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override; //added function
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;  
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        EconomySelection(const EconomySelection &ecs); //added function- new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override; //added function
        EconomySelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override; //added function
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        SustainabilitySelection(const SustainabilitySelection &ss); //added function- new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override; //added function
        SustainabilitySelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override; //added function
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};