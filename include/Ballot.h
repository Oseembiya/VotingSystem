#ifndef BALLOT_H
#define BALLOT_H

#include "Voter.h"
#include "Candidate.h"

class Ballot
{                         // Written by Osee Mbiya M00781715
private:
    Voter *voter;         // Pointer to the voter who cast the ballot
    Candidate *candidate; // Pointer to the candidate the voter voted for

public:
    Ballot(Voter *voter, Candidate *candidate);
  Ballot();
    Voter *getVoter() const;
    Candidate *getCandidate() const;
};

#endif
