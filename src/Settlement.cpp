#pragma once
#include "Settlement.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

//constructor
Settlement::Settlement(const string &name, SettlementType type):name(name), type(type){}

//methods
const string &Settlement::getName() const{return name;}
SettlementType Settlement::getType() const{return type;}
const string Settlement::toString() const{return "SettlementName: "+ name;}
