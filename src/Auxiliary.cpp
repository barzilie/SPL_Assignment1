#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>


static std::vector<std::string> Auxiliary::parseArguments(const std::string& line){
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;
    while (getline (ss, argu, ' ')) {
        result.push_back (argu);
    }

    return result;
}