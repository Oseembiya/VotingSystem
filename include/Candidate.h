#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <iostream>

class Candidate {
private:
  std::string name;     // Candidate's name
  std::string party;    // Candidate's party affiliation
  std::string position; // Position candidate is running for
  int NumberOfVotes;  

public:
    // Constructor
  Candidate(std::string name, std::string party, std::string position, int num);
  Candidate();
    // Accessor methods
    std::string getName() const; // Gets the candidate's name
    std::string getParty() const; // Gets the candidate's party
    std::string getPosition() const; // Gets the candidate's position
  int getVotes() const;
  void setVotes(int num);
};

#endif 

