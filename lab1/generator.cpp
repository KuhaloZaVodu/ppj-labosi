#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

// Privremeni exception za funkcije čija implementacija nije napisana
class NotImplemented : public std::logic_error
{
public:
    NotImplemented() : std::logic_error("Funkcija nije implementirana") { };
};

// Definicija akcije koju leksički analizator može poduzeti
struct LexerAction {
    std::string state;
    std::string regex;
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
    LexerDefinitionParser() { };
    LexerDefinition getLexerDefintion() { return this->parsedLexerDefinition; };

    void parseInput(std::string input);
};

// Generator 'lexer.c' datoteke
class LexerGenerator {
public:
    LexerGenerator() { };
    FILE* generateLexerSource(LexerDefinition* lexerDefinition, std::string path);
};

int main() {
    std::string input;

    std::getline(std::cin, input, '\0'); // Procitaj citav stdin u varijablu input

    LexerDefinitionParser parser;
    parser.parseInput(input);
    LexerDefinition definition = parser.getLexerDefintion();

    LexerGenerator generator;

    FILE* result = generator.generateLexerSource(&definition, "./analizator");
    
    return 0;
}

/**
 * Parsira definiciju lexera
 *
 * @param input String koji sadrži potpunu definiciju lexera
 */
void LexerDefinitionParser::parseInput(std::string input) { throw NotImplemented(); }


/**
 * Generira izvorni kod lexera
 *
 * @param lexerDefinition Pokazivač na definiciju lexera koji se generira
 * @param path Odredišna putanja direktorija u kojemu će biti izvorni kod lexera
 * @return Pokazivač na generiranu datoteku
 */
FILE* LexerGenerator::generateLexerSource(LexerDefinition* lexerDefinition, std::string path) { throw NotImplemented(); }