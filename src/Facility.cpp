#pragma once
#include <string>
#include <vector>
#include "Facility.h"
using std::string;
using std::vector;

//***************** FacilityType *****************

//constructor and copy constructor

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}

FacilityType::FacilityType(const FacilityType &type):
name(type.name), category(type.category), price(type.price), lifeQuality_score(type.lifeQuality_score),economy_score(type.economy_score), environment_score(type.environment_score){}

//FacilityType: methods

const string& FacilityType::getName() const {return name;}

int FacilityType::getCost() const {return price;}

int FacilityType::getLifeQualityScore() const{return lifeQuality_score;}

int FacilityType::getEnvironmentScore() const{return environment_score;}

int FacilityType::getEconomyScore() const{return economy_score;}

FacilityCategory FacilityType::getCategory() const{return category;}

//***************** Facility *****************

// constructors
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

Facility::Facility(const FacilityType &type, const string &settlementName): 
FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){}

//Facility: methods
const string& Facility::getSettlementName() const{return settlementName;}

const int Facility::getTimeLeft() const{return timeLeft;}

FacilityStatus Facility::step(){
    timeLeft = getTimeLeft()-1;
    if (timeLeft == 0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return getStatus();
}

void Facility::setStatus(FacilityStatus status){this->status = status;}

const FacilityStatus& Facility::getStatus() const{return status;}

const string Facility::toString() const{return "FacilityName: "+ name;}

const string Facility::toStringStatus() const{
    FacilityStatus currentStatus = this->getStatus();
    switch(currentStatus){
        case FacilityStatus::OPERATIONAL:
            return "Facilitystatus: OPERATIONAL";
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            return "Facilitystatus: UNDER_CONSTRUCTIONS";}
    return "";
}

Facility* Facility::clone() const{
    return new Facility(*this);
}

