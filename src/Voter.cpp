#include "Voter.h"

Voter::Voter(const std::string &name, const std::string &dob) : name(name), dob(dob) {}

Voter::Voter() = default;

std::string Voter::getName() const
{
    return name;
}

std::string Voter::getDOB() const
{
    return dob;
}
