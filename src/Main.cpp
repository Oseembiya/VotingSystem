#include <iostream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <string>
#include <regex>
#include "Election.h"

bool authenticateVoter(Election *election);
void displayMainMenu();
void castVote(Election *election);
void viewCandidateInfo(Election *election);
void viewElectionResults(Election *election);

int main()
{
    Election *election = new Election();
    std::string candidateFile = "candidates.csv";
    std::string voterFile = "voters.csv";

    if (!election->loadData(candidateFile, voterFile))
    {
        std::cerr << "Failed to load data files." << std::endl;
        delete election; // Clean up memory
        return 1;
    }
    bool isAuthenticated = false;
    while (!isAuthenticated)
    {
        isAuthenticated = authenticateVoter(election);
        if (!isAuthenticated)
        {
            std::cout << "Returning to start...\n";
        }
    }

    int choice;
    while (true)
    {
        displayMainMenu();

        // Input validation loop
        while (!(std::cin >> choice))
        {
            std::cout << "Please enter a number: ";
            std::cin.clear();                                                   // Clear the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the input
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the rest of the line

        switch (choice)
        {
        case 1:
            castVote(election);
            break;
        case 2:
            viewCandidateInfo(election);
            break;
        case 3:
            viewElectionResults(election);
            break;
        case 4:
            std::cout << "Exiting Voting System." << std::endl;
	    return 0;
	    break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }
    }

    return 0;
}

bool authenticateVoter(Election *election)
{
    std::string name, dob;
    bool validInput = false;

    while (!validInput)
    {
        std::cout << "===================================================\n"
                  << "|              Voter Authentication               |\n"
                  << "===================================================\n"
                  << "| Please enter your details to start:             |\n"
                  << "| Name: ";
        std::getline(std::cin, name);
        if (!Election::isValidName(name))
        {
            std::cout << "| Invalid format. Letters and spaces only.\n";
            continue;
        }
        std::cout << "| DOB (DD/MM/YYYY): ";
        std::getline(std::cin, dob);
        if (!Election::isValidDate(dob))
        {
            std::cout << "| Invalid format. Please enter correct format.\n";
            continue;
        }
        validInput = true;
    }

    if (election->authenticateVoter(name, dob))
    {
        std::cout << "===================================================\n"
                  << "| Authentication successful.                      |\n"
                  << "===================================================\n";
        return true;
    }
    else
    {
        char choice;
        std::cout << "===================================================\n"
                  << "| Authentication failed. You're not registered.   |\n"
                  << "| Do you want to register to vote? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'y' || choice == 'Y')
        {
            election->registerVoter(name, dob);
            return true;
        }
        else
        {
            std::cout << "===================================================\n"
                      << "| Registration declined.                          |\n"
                      << "===================================================\n";
                      // User declined registration
            return false;
        }
    }
}

void displayMainMenu()
{
    std::cout << "===================================================\n"
              << "|     Welcome to the Voting System Dashboard.     |\n"
              << "===================================================\n"
              << "| 1. Cast Vote                                    |\n"
              << "| 2. View candidate Info                          |\n"
              << "| 3. View election results                        |\n"
              << "| 4. Exit                                         |\n"
              << "===================================================\n"
              << "| Select an option => ";
}

// Utility function to trim whitespace from the start and end of a string
std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

// Utility function to convert a string to lowercase
std::string toLower(const std::string &str)
{
    std::string lowerCase;
    std::transform(str.begin(), str.end(), std::back_inserter(lowerCase),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return lowerCase;
}

void castVote(Election *election)

{
  Vector<Candidate> candidates = election->getCandidates();
  std::cout << candidates[0].getName() << " " << candidates[0].getParty() << std::endl;
      for (size_t i = 0; i < candidates.getSize(); ++i) {
        std::cout << "- " << candidates[i].getName() << " (" << candidates[i].getParty() << ")\n";
    }
    
    std::string candidateIdentifier;
    std::cout << "Enter Candidate Name or Party: ";
    std::getline(std::cin, candidateIdentifier);
    std::string processedInput = toLower(trim(candidateIdentifier));
    election->castVote(processedInput);
}


void viewCandidateInfo(Election *election) {
    Vector<Candidate> candidates = election->getCandidates();

    std::cout << "+======+======================+==================+=================+\n"
              << "| ID   | Candidate Name       | Party            | Vote Count      |\n"
              << "+======+======================+==================+=================+\n";

    for (size_t i = 0; i < candidates.getSize(); ++i) {
      std::cout << "| " << std::left << std::setw(4) << (i + 1)
		<< " | " << std::left << std::setw(20) << candidates[i].getName()
		<< " | " << std::left << std::setw(16) << candidates[i].getParty()
		<< " | " << std::left << std::setw(15) << candidates[i].getVotes() << " |\n";
    }

    std::cout << "+======+======================+==================+=================+\n";
}


void viewElectionResults(Election *election) // Written By Dumitru Nirca
{
  // Displaying the results in table
  std::cout << "====================================================================================\n"
              << "| ID | Candidate Name          | Party            | Vote Count | Percentage        |\n"
	    << "====================================================================================\n";
    Vector<Candidate> candidates = election->getCandidates();
    auto compareByVotes = [](const Candidate &a, const Candidate &b) {
    return a.getVotes() > b.getVotes(); // Sort in descending order of votes
};

// Sort the candidates vector using the custom comparator
    std::sort(candidates.begin(), candidates.end(), compareByVotes);
    
    // Display the sorted candidates
    int totalVotes = 0;
    for (const auto &cand : candidates) {
      totalVotes +=cand.getVotes();
    }
    int i = 0;
    for (const auto &cand : candidates)
   {
     double percentage = (static_cast<double>(cand.getVotes()) / totalVotes) * 100.0;

     std::cout << "| " << std::setw(2) << ++i
	       << "| " << std::setw(25) << cand.getName()
	       << "| " << std::setw(17) << cand.getParty()
	       << "| " << std::setw(11) << cand.getVotes()
		<< "| " << std::setw(16) << std::fixed << std::setprecision(2) << percentage << "% |" << std::endl;
     std::cout << "====================================================================================\n";
    }
    

 // Displaying the winner
    std::cout << "\nThe winner of the election is: "
	      << candidates[0].getName()
	      << " from " << candidates[0].getParty()
	      << " with " << candidates[0].getVotes() << " votes.\n\n";
}
