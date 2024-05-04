#include "Ballot.h"

// Constructor implementation
Ballot::Ballot(Voter* voter, Candidate* candidate) : voter(voter), candidate(candidate) {}

Ballot::Ballot() {}

// Getter for the voter
Voter* Ballot::getVoter() const {                      // written by Osee Mbiya 
    return voter;        
}

// Getter for the candidate
Candidate* Ballot::getCandidate() const {
    return candidate;
}
