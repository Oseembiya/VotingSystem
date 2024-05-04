#ifndef ELECTION_H
#define ELECTION_H

#include <regex>
#include <string>
#include <vector>
#include "Voter.h"
#include "Candidate.h"
#include "Ballot.h"
#include "../src/Map.cpp"

class Election
{
private:
  Vector<Candidate> *candidates;
  Vector<Voter> *voters;
  Vector<Ballot> *ballots;
  Map<std::string, int> voteCounts; // For tracking voters
  Map<std::string, std::string> voterAuthDetails;
  std::string userName;

public:
  Election();
  bool loadData(const std::string &candidateFile, const std::string &voterFile);
  bool authenticateVoter(const std::string &name, const std::string &dob);

  // Declare these methods as static and with correct return types bool
  static bool isValidName(const std::string &name)
  {
    return std::regex_match(name, std::regex("^[A-Za-z ]+$"));
  }

  static bool isValidDate(const std::string &dob)
  {
    return std::regex_match(dob, std::regex("^(0?[1-9]|[12][0-9]|3[01])/(0?[1-9]|1[0-2])/([0-9]{4})$"));
  }
  void registerVoter(const std::string &name, const std::string &dob);
  bool castVote(const std::string &candidateIdentifier); // Ensure unique signature
  bool updateCSV(const std::string &candidateName, int newVotes);
  bool updateVotersCSV();
  bool hasVoted(const std::string& userName);
  Vector<Candidate> &getCandidates() const;
  Vector<Map<std::string, int>> calculateResults() const;
};

#endif // ELECTION_H
