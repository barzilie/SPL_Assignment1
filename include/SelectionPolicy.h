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
        virtual const string toStringFullName() const = 0; //added function to all
        virtual SelectionPolicy *clone() const = 0;
        virtual void incrementScores(int LQ, int EC, int EN) = 0; //added function to all (relevant only for bal)
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        NaiveSelection(const NaiveSelection &ns); //new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override;
        NaiveSelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override;
        ~NaiveSelection() override = default;
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection(const BalancedSelection &bs); //new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override;
        BalancedSelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override;
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;  
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        EconomySelection(const EconomySelection &ecs); //new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override;
        EconomySelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override;
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        SustainabilitySelection(const SustainabilitySelection &ss); //new copy constructor
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        const string toStringFullName() const override;
        SustainabilitySelection *clone() const override;
        void incrementScores(int LQ, int EC, int EN) override;
        ~SustainabilitySelection() override = default;
    private:
        int lastSelectedIndex;
};