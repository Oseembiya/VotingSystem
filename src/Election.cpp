#include "Election.h"
#include "Voter.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <regex>

Election::Election() : candidates(new Vector<Candidate>()), voters(new Vector<Voter>()), ballots(new Vector<Ballot>()), voteCounts(), voterAuthDetails()
{
  voteCounts = Map<std::string, int>();
  voterAuthDetails = Map<std::string, std::string>();
} // Written by Osee Mbiya M00871715

bool Election::loadData(const std::string &candidateFile, const std::string &voterFile)
{
  std::ifstream candidateStream(candidateFile);
  if (!candidateStream.is_open())
    {
      std::cerr << "Error: Unable to open candidate file: " << candidateFile << std::endl;
      return false;
    }
  std::string line;
  std::getline(candidateStream, line);
  while (std::getline(candidateStream, line))
    {
      std::stringstream ss(line);
      std::string id, name, party, position, numberOfVotes;
      std::getline(ss, id, ',');
      std::getline(ss, name, ',');
      std::getline(ss, party, ',');
      std::getline(ss, position, ',');
      std::getline(ss, numberOfVotes);
      int numOfVotes = std::stoi(numberOfVotes);
      candidates->push_back(Candidate(name, party, position, numOfVotes));
    }
  candidateStream.close();

  std::ifstream voterStream(voterFile);
  if (!voterStream.is_open())
    {
      std::cerr << "Error: Unable to open voter file: " << voterFile << std::endl;
      return false;
    }

  std::getline(voterStream, line);
  while (getline(voterStream, line))
    {
      std::stringstream ss(line);
      std::string id, name, email, phone, address, dob, eligibility, registrationDate, votingStatus;
      getline(ss, id, ',');
      getline(ss, name, ',');
      getline(ss, email, ',');
      getline(ss, phone, ','); // Written By Taf
      getline(ss, address, ',');
      getline(ss, dob, ',');
      getline(ss, eligibility, ',');
      getline(ss, registrationDate, ',');
      getline(ss, votingStatus);
      voters->push_back(Voter(name, dob));
      voterAuthDetails[name] = dob;
    }
  voterStream.close();
  return true;
}

bool Election::hasVoted(const std::string& userName) { // Written by Dumitru Nirca
  // Open the voters.csv file
  std::ifstream voterStream("voters.csv");
  if (!voterStream.is_open()) {
    std::cerr << "Error: Unable to open voter file." << std::endl;
    return true; // Assume error, disallow voting
  }

  std::string line;
  while (getline(voterStream, line)) {
    std::stringstream ss(line);
    std::string id, name, email, phone, address, dob, eligibility, registrationDate, votingStatus;
    getline(ss, id, ','); // ID
    getline(ss, name, ','); // Name
    getline(ss, email, ',');
    getline(ss, phone, ',');
    getline(ss, address, ',');
    getline(ss, dob, ',');
    getline(ss, eligibility, ',');
    getline(ss, registrationDate, ',');
    getline(ss, votingStatus); // Voting Status

    // Check if the current line corresponds to the user
    if (name == userName) {
      voterStream.close();
      return (votingStatus == "Voted");
    }
  }
    
  voterStream.close();
  return true; // Assume error, disallow voting
}

bool Election::authenticateVoter(const std::string &name, const std::string &dob)
{
  try
    {
      const std::string &storedDob = voterAuthDetails.find(name);
      userName = name;
      return storedDob == dob; // Return true if DOB matches, false otherwise
    }
  catch (const std::out_of_range &)
    {
      return false; // Return false if the name is not found
    }
}

void Election::registerVoter(const std::string &name, const std::string &dob)
{
  std::ofstream voterFile("voters.csv", std::ios::app);
  if (voterFile.is_open())
    {
      int id = voters->getSize() + 1;
      std::string email = "unknown@example.com";
      std::string phone = "000-000-0000";
      std::string address = "Unknown Address";
      std::string eligibility = "Eligible";
      std::string registrationDate = "01/01/2023";
      std::string votingStatus = "Not Voted";

      // Sanitize inputs to avoid breaking CSV format
      auto sanitize = [](const std::string &str)
      {
	std::string sanitized = str;
	std::replace(sanitized.begin(), sanitized.end(), ',', ';');
	return sanitized;
      };

      voterFile << id << ","
		<< sanitize(name) << ","
		<< email << ","
		<< phone << ","
		<< sanitize(address) << ","
		<< dob << ","
		<< eligibility << ","
		<< registrationDate << ","
		<< votingStatus << std::endl;

      voters->push_back(Voter(name, dob));
      voterAuthDetails[name] = dob;
      voterFile.close();

      std::cout << "===================================================\n"
		<< "| Registration successful.                        |\n"
		<< "| Name: " << name << "\n"
		<< "| DOB: " << dob << "\n"
		<< "| You are now eligible to vote.                   |\n"
		<< "===================================================\n";
    }
  else
    {
      std::cerr << "Failed to open the voter for registration." << std::endl;
    }
}

bool Election::castVote(const std::string &candidateIdentifier)
{
  std::string identifierLower = candidateIdentifier;
  std::transform(identifierLower.begin(), identifierLower.end(), identifierLower.begin(),
                 [](unsigned char c)
                 { return std::tolower(c); });

  auto caseInsensitiveMatchIt = std::find_if(candidates->begin(), candidates->end(),
					     [&identifierLower](const Candidate &c)
					     {
					       std::string nameLower = c.getName();
					       std::string partyLower = c.getParty();
					       std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
					       std::transform(partyLower.begin(), partyLower.end(), partyLower.begin(), ::tolower);

					       // Check if identifier matches either name or party
					       return nameLower == identifierLower || partyLower == identifierLower;
					     });

  if (caseInsensitiveMatchIt != candidates->end())
    {
      if(hasVoted(userName)) {
	std::cout << "You have already voted. Multiple votes are not allowed." << std::endl;
	return false;
      }
      // Since we found the candidate in a case-insensitive manner,
      // increment the vote count using the original name for consistency
      updateCSV(caseInsensitiveMatchIt->getName(), caseInsensitiveMatchIt->getVotes());
      updateVotersCSV();
      std::cout << "Vote successfully casted for " << caseInsensitiveMatchIt->getName() << " (" << caseInsensitiveMatchIt->getParty() << ")." << std::endl;
      return true;
    }
  else
    {
      std::cout << "Candidate or party not found." << std::endl;
      return false;
    }
}
bool Election::updateVotersCSV() { // Written by Dumitru Nirca
  std::ifstream inFile("voters.csv");
  if (!inFile.is_open()) {
    std::cerr << "Error: Unable to open voters.csv for reading." << std::endl;
    return false;
  }

  std::ofstream outFile("temp.csv");
  if (!outFile.is_open()) {
    std::cerr << "Error: Unable to open temp.csv for writing." << std::endl;
    return false;
  }
    
  std::string line;
  while (std::getline(inFile, line)) {
    std::istringstream iss(line);
    std::string id, nameFromFile, email, phone, address, dob, eligibility, registrationDate, oldVotingStatus;
    std::getline(iss, id, ','); // ID
    std::getline(iss, nameFromFile, ','); // Name
    std::getline(iss, email, ',');
    std::getline(iss, phone, ',');
    std::getline(iss, address, ',');
    std::getline(iss, dob, ',');
    std::getline(iss, eligibility, ',');
    std::getline(iss, registrationDate, ',');
    std::getline(iss, oldVotingStatus); // Voting Status
    if (userName == nameFromFile) {
      outFile << id << "," << nameFromFile << "," << email << "," << phone << "," << address << ","
	      << dob << "," << eligibility << "," << registrationDate << "," << "Voted" << std::endl;
    } else {
      outFile << line << std::endl;
    }
  }

  
  inFile.close();
  outFile.close();

  // Replace the original file with the temporary file
  std::remove("voters.csv");
  std::rename("temp.csv", "voters.csv");

  return true;
}


bool Election::updateCSV(const std::string &candidateName, int newVotes)
{

  newVotes++;
  std::ifstream inFile("candidates.csv");
  if (!inFile.is_open())
    {
      std::cerr << "Error: Unable to open candidates.csv for reading." << std::endl;
      return false;
    }

  std::ofstream outFile("temp.csv");
  if (!outFile.is_open())
    {
      std::cerr << "Error: Unable to open temp.csv for writing." << std::endl;
      return false;
    }

  std::string line;
  while (std::getline(inFile, line))
    {
      std::istringstream iss(line);
      std::string token;
      std::string id;
      std::getline(iss, id, ','); // ID
      std::getline(iss, token, ','); // Name
      if (token == candidateName)
	{
	  // Update the vote count in this line
	  outFile << id << ",";
	  outFile << token << ",";
	  std::getline(iss, token, ','); // Party
	  outFile << token << ",";
	  std::getline(iss, token, ','); // Position
	  outFile << token << ",";
	  outFile << newVotes << std::endl; // Update the votes
	}
      else
	{
	  // Write the line as it is
	  outFile << line << std::endl;
	}
    }

  inFile.close();
  outFile.close();

  // Replace the original file with the temporary file
  std::remove("candidates.csv");
  std::rename("temp.csv", "candidates.csv");
  voteCounts[candidateName] = newVotes;

    // Update candidates vector
    for (auto& candidate : *candidates) {
        if (candidate.getName() == candidateName) {
            candidate.setVotes(newVotes);
            break;
        }
    }

  return true;
}


 Vector<Candidate> &Election::getCandidates() const
{
  return *candidates;
}





