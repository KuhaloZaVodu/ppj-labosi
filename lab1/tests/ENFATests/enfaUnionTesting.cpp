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
    ENFA* concat(ENFA* other);
    void logDescription();
};

int main() {
    // ENFA 1: accepts "ab"
    std::set<std::string> states1 = { "q0", "q1", "q2" };
    std::set<std::string> alphabet1 = { "a", "b" };
    std::set<std::string> acceptStates1 = { "q2" };
    std::string startState1 = "q0";
    std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions1 = {
        { {"q0", "a"}, {"q1"} },
        { {"q1", "b"}, {"q2"} }
    };
    ENFA enfa1(states1, alphabet1, acceptStates1, startState1, transitions1);

    // ENFA 2: does NOT accept "ab" (requires "ac" instead)
    std::set<std::string> states2 = { "q0", "q1", "q3" };
    std::set<std::string> alphabet2 = { "a", "c" };
    std::set<std::string> acceptStates2 = { "q3" };
    std::string startState2 = "q0";
    std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions2 = {
        { {"q0", "a"}, {"q1"} },
        { {"q1", "c"}, {"q3"} }
    };
    ENFA enfa2(states2, alphabet2, acceptStates2, startState2, transitions2);

    // Test input
    std::string input = "ab";

    /*if (enfa1.run(input)) {
        std::cout << "ENFA 1 alone accepted!" << std::endl;
    } else {
        std::cout << "ENFA 1 alone declined!" << std::endl;
    }

    if (enfa2.run(input)) {
        std::cout << "ENFA 2 alone accepted!" << std::endl;
    } else {
        std::cout << "ENFA 2 alone declined!" << std::endl;
    }*/

    ENFA* enfa3 = enfa1.concat(&enfa2);
    ENFA* enfa4 = enfa2.concat(&enfa1);

    std::cout << "LOGGING ENFA 3 DESCRIPTION:" << std::endl;
    enfa3->logDescription();

    if (enfa3->run(input)) {
        std::cout << "ENFA 3 accepted!" << std::endl;
    } else {
        std::cout << "ENFA 3 declined!" << std::endl;
    }

    if (enfa4->run(input)) {
        std::cout << "ENFA 4 accepted!" << std::endl;
    } else {
        std::cout << "ENFA 4 declined!" << std::endl;
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

        std::cout << "testing input " << letter << std::endl;

        for (const std::string& state : currentStates) {
            auto it = transitions.find({state, letter});
            if (it != transitions.end()) {
                nextStates.insert(it->second.begin(), it->second.end());
            }
        }

        if (nextStates.empty()) {    
            std::cout << "EMPTY QUITTING!!" << std::endl;
            return false;
        }

        currentStates = nextStates;

        epsilonTransitions(currentStates);
    }

    std::cout << "Printing end states" << std::endl;
    for (const std::string& endState : currentStates) {
        std::cout << endState << std::endl;
    }

    for (const std::string& state : currentStates) {
        if (acceptStates.find(state) != acceptStates.end()) {
            return true;
        }        
    }

    return false;
}

ENFA* ENFA::concat(ENFA* other) {
    std::set<std::string> newStates;
    std::set<std::string> newAlphabet;
    std::set<std::string> newAcceptStates;
    std::string newStartState;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> newTransitions;

    for (auto state : this->states) {
        newStates.insert("1" + state);
    }

    for (auto state : other->states) {
        newStates.insert("2" + state);
    }

    for (auto acceptState : this->acceptStates) {
        newAcceptStates.insert("1" + acceptState);
    }

    for (auto acceptState : other->acceptStates) {
        newAcceptStates.insert("2" + acceptState);
    }

    newAlphabet = this->alphabet;
    newAlphabet.insert(other->alphabet.begin(), other->alphabet.end());

    for (const auto& transition : this->transitions) {
        auto newKey = std::make_pair("1" + transition.first.first, transition.first.second);
        std::set<std::string> newTargets;
        for (const auto& target : transition.second) {
            newTargets.insert("1" + target);
        }
        newTransitions[newKey] = newTargets;
    }

    for (const auto& transition : other->transitions) {
        auto newKey = std::make_pair("2" + transition.first.first, transition.first.second);
        std::set<std::string> newTargets;
        for (const auto& target : transition.second) {
            newTargets.insert("2" + target);
        }
        newTransitions[newKey] = newTargets;
    }

    newStartState = "u" + this->startState + other->startState;

    auto startEpsilonKey = std::make_pair(newStartState, "$");
    std::set<std::string> oldStartStates;
    oldStartStates.insert("1" + this->startState);
    oldStartStates.insert("2" + other->startState);

    newTransitions.insert(std::make_pair(startEpsilonKey, oldStartStates));

    return new ENFA(newStates, newAlphabet, newAcceptStates, newStartState, newTransitions);
}

void ENFA::logDescription() {
    std::cout << "STATES:" << std::endl;
    for (auto state : this->states) {
        std::cout << state << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ACCCEPT STATES:" << std::endl;
    for (auto state : this->acceptStates) {
        std::cout << state << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ALPHABET:" << std::endl;
    for (auto symbol : this->alphabet) {
        std::cout << symbol << std::endl;
    }
    std::cout << std::endl;

    for (auto mapEntry : this->transitions) {
        std::cout << mapEntry.first.first << "+" << mapEntry.first.second << " = ";
        for (auto next : mapEntry.second) {
            std::cout << next << ",";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}