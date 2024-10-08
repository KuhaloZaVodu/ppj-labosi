#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

// Privremeni exception za funkcije čija implementacija nije napisana
class NotImplemented : public std::logic_error {
public:
    NotImplemented() : std::logic_error("Funkcija nije implementirana") {};
};

// Definicija akcije koju leksički analizator može poduzeti
struct LexerAction {
    std::pair<std::string, std::string> stateRegex;
    //std::string state;
    //std::string regex;
    std::vector<std::string> commands;
};


// Pristupačniji "format" definicije lexera
struct LexerDefinition {
    std::map<std::string, std::string> regDefs;
    std::set<std::string> lexerStates;
    std::set<std::string> lexicalUnits;
    std::vector<LexerAction> actions;
};

// Parser ulaza
class LexerDefinitionParser {
private:
    LexerDefinition parsedLexerDefinition;
public:
    LexerDefinitionParser() {};

    LexerDefinition getLexerDefintion() { return this->parsedLexerDefinition; };

    void parseInput(std::string input);
};

// Generator 'lexer.c' datoteke
class LexerGenerator {
public:
    LexerGenerator() {};

    FILE *generateLexerSource(LexerDefinition *lexerDefinition, std::string path);
};

int main() {
    std::string input;

    std::getline(std::cin, input, '\0'); // Procitaj citav stdin u varijablu input

    //std::cout << input;

    LexerDefinitionParser parser;
    parser.parseInput(input);
    LexerDefinition definition = parser.getLexerDefintion();

    LexerGenerator generator;

    //FILE* result = generator.generateLexerSource(&definition, "./analizator");

    return 0;
}

/**
 * Parsira definiciju lexera
 *
 * @param input String koji sadrži potpunu definiciju lexera
 */
void LexerDefinitionParser::parseInput(std::string input) {
    std::string row;
    std::istringstream iss(input);
    std::string first;
    std::string second;
    //Koraci - citanje regdef, stanja, imena jedinki i pravila lex analizatora
    bool pos = false;
    //Parserski bool i string temp
    bool half = false;
    std::string temp;
    LexerAction temp_LexerAction;
    std::vector<LexerAction> temp_actions;
    bool microActionReader = false;

    while (std::getline(iss, row)) {
        if (!pos) { //regdef
            if (row.substr(0, 1) == "{") {
                half = false;
                std::istringstream iss2(row);
                while (std::getline(iss2, temp, ' ')) {
                    if (!half) {
                        first = temp.substr(1, temp.length() - 2);
                        half = true;
                    } else {
                        second = temp;
                        break;
                    }
                }
                parsedLexerDefinition.regDefs.emplace(first, second);
            } else { // ocekuje se %X pa %L
                row = row.substr(3, row.length() - 3);
                std::istringstream iss2(row);
                while (std::getline(iss2, temp, ' '))
                    parsedLexerDefinition.lexerStates.insert(temp);
                std::getline(iss, row);
                row = row.substr(3, row.length() - 3);
                std::istringstream iss3(row);
                while (std::getline(iss3, temp, ' '))
                    parsedLexerDefinition.lexicalUnits.insert(temp);
                pos = true;
            }
        } else {
            if (row.substr(0, 1) == "<") {
                row = row.substr(1);
                std::istringstream iss4(row);
                half = false;
                while (std::getline(iss4, temp, '>')) {
                    if (!half) {
                        first = temp;
                        half = true;
                    } else {
                        second = temp;
                        break;
                    }
                }
            } else if (row == "{") {
                microActionReader = true;
                temp_LexerAction.stateRegex = std::make_pair(first, second);

                bool finder = false;
                for(auto obj : temp_actions) {
                    if(obj.stateRegex == temp_LexerAction.stateRegex)
                        finder = true;
                }
                if(!finder) {
                    temp_actions.push_back(temp_LexerAction);
                }

            } else if (microActionReader && row != "}") {
                for(auto &obj : temp_actions) {
                    if(obj.stateRegex == temp_LexerAction.stateRegex) {
                        obj.commands.push_back(row);
                    }

                }
            } else {
                temp_LexerAction.stateRegex.first.clear();
                temp_LexerAction.stateRegex.second.clear();
                microActionReader = false;
            }
        }
    }
    parsedLexerDefinition.actions = temp_actions;

}


/**
 * Generira izvorni kod lexera
 *
 * @param lexerDefinition Pokazivač na definiciju lexera koji se generira
 * @param path Odredišna putanja direktorija u kojemu će biti izvorni kod lexera
 * @return Pokazivač na generiranu datoteku
 */
FILE *
LexerGenerator::generateLexerSource(LexerDefinition *lexerDefinition, std::string path) { throw NotImplemented(); }