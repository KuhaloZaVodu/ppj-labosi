#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
// #include "../analizator/analizator.h" dok se ne napiše potpuna implementacija analizator.h
// Epsilon NKA instanca
class ENFA {
private:
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> acceptStates;
    std::string startState;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions;
    void epsilonTransitions(std::set<std::string>& currentStates) const;
public:
    // TODO: Probably write ENFAFactory class that takes in Regex and creates ENFA so change
    // the constructor to take states, alphabet, acceptStates, startState and transitions rather
    // than it taking the regex and then generating it itself
    ENFA(std::string regex) { /* TODO: Implement */ }

    // SAMO U SVRHE TESTIRANJA:
    ENFA(std::set<std::string> states, std::set<std::string> alphabet, std::set<std::string> acceptStates, 
         std::string startState, std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions)
        : states(std::move(states)), alphabet(std::move(alphabet)), acceptStates(std::move(acceptStates)), 
          startState(std::move(startState)), transitions(std::move(transitions)) {}

    bool run(std::string& inputStream);
};

int main() {
    std::string row;
    std::string section;

    std::vector<std::string> inputRows;
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> acceptStates;
    std::string startState;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions;

    int rowNumber = 1;

    while(std::getline(std::cin, row)) {
        std::istringstream iss(row);

        if (rowNumber == 1) {
            while (std::getline(iss, row, '|')) {
                inputRows.push_back(row);
            }
        }  else if (rowNumber == 2) {
            // Read the states
            while (std::getline(iss, section, ',')) {
                states.insert(section);
            }
        } else if (rowNumber == 3) {
            // Read the alphabet
            while (std::getline(iss, section, ',')) {
                alphabet.insert(section);
            }
        } else if (rowNumber == 4) {
            // Read the accept states
            while (std::getline(iss, section, ',')) {
                acceptStates.insert(section);
            }
        } else if (rowNumber == 5) {
            // Read the start state
            startState = row;
        } else if (rowNumber >= 6) {
            // Read the transitions
            std::string fromState, symbol, toState;
            std::vector<std::string> destinations;

            // Parse the transition in the format `fromState,symbol -> destination1,destination2`
            std::istringstream transStream(row);
            std::string leftSide, rightSide;

            std::getline(transStream, leftSide, '-');  // Parse the left side (fromState,symbol)
            std::getline(transStream, rightSide);      // Parse the right side (destination states)

            std::istringstream leftStream(leftSide);
            std::getline(leftStream, fromState, ',');  // Get fromState
            std::getline(leftStream, symbol);          // Get symbol

            std::istringstream rightStream(rightSide.substr(1));  // Skip the ">" and parse the destinations
            while (std::getline(rightStream, toState, ',')) {
                destinations.push_back(toState);
            }

            // Insert the transition into the map
            transitions[{fromState, symbol}] = std::set<std::string>(destinations.begin(), destinations.end());
        }

        rowNumber++;
    }

    ENFA enfa(states, alphabet, acceptStates, startState, transitions);

    for (std::string& inputStream : inputRows) {
        // std::cout << "Testing input stream: " << inputStream << " -> ";

        bool result = enfa.run(inputStream);
        if (result) {
            std::cout << "Accepted" << std::endl;
        } else {
            std::cout << "Rejected" << std::endl;
        }
    }

    return 0;
}

void ENFA::epsilonTransitions(std::set<std::string>& currentStates) const {
    std::set<std::string> visitedStates = currentStates;
    bool updated = true;
    
    while (updated) {
        updated = false;
        std::set<std::string> newStates;
        
        for (const std::string& state : currentStates) {
            auto epsilonTrans = transitions.find({state, "$"});
            
            if (epsilonTrans != transitions.end()) {
                for (const std::string& newState : epsilonTrans->second) {
                    if (visitedStates.find(newState) == visitedStates.end()) {
                        newStates.insert(newState);
                        visitedStates.insert(newState);
                        updated = true;
                    }
                }
            }
        }
        
        currentStates.insert(newStates.begin(), newStates.end());
    }
};

bool ENFA::run(std::string& inputStream) {
    std::set<std::string> currentStates = { startState };

    epsilonTransitions(currentStates);

    for (char inputSymbol : inputStream) {
        std::string letter(1, inputSymbol); // pretvori char u string (možda malo nepotrebno)
        std::set<std::string> nextStates;

        if (letter.compare(",") == 0) continue;

        for (const std::string& state : currentStates) {
            auto it = transitions.find({state, letter});
            if (it != transitions.end()) {
                nextStates.insert(it->second.begin(), it->second.end());
            }
        }

        if (nextStates.empty()) {    
            return false;
        }

        currentStates = nextStates;

        epsilonTransitions(currentStates);
    }

    for (const std::string& state : currentStates) {
        if (acceptStates.find(state) != acceptStates.end()) {
            return true;
        }        
    }

    return false;
}