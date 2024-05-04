#ifndef VOTER_H
#define VOTER_H

#include <string>

class Voter
{
private:
    std::string name;            // Written by Osee mbiya 
    std::string dob;

public:
    Voter(const std::string &name, const std::string &dob);
  Voter();
    std::string getName() const;
    std::string getDOB() const;
};

#endif // VOTER_H
