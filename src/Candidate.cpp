#include "Candidate.h"

// Constructor initialization with default votes set to zero
Candidate::Candidate(std::string name, std::string party, std::string position, int num)
    : name(name), party(party), position(position), NumberOfVotes(num) {
}

// Default constructor initialization
Candidate::Candidate() : NumberOfVotes(0) {
}

// Method to get the candidate's name
std::string Candidate::getName() const {
    return name;
}

// Method to get the candidate's party
std::string Candidate::getParty() const {
    return party;
}

// Method to get the candidate's position
std::string Candidate::getPosition() const {
    return position;
}

// Method to get the number of votes
int Candidate::getVotes() const {
  return NumberOfVotes;
}

void Candidate::setVotes(int num) {
  NumberOfVotes = num;
}
