#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>

// Epsilon NKA instanca
class ENFA {
private:
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> acceptStates;
    std::string startState;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> transitions;
public:
    // TODO: Probably write ENFAFactory class that takes in Regex and creates ENFA so change
    // the constructor to take states, alphabet, acceptStates, startState and transitions rather
    // than it taking the regex and then generating it itself
    ENFA(std::string regex) { /* TODO: Implement */ }
    ENFA* concat(ENFA* other);
    bool run(std::string inputStream);
};

// Upravljač stanjima lexera i automatima
class ENFAManager {
private:
    std::string currentLexerState;

    // Za svako stanje leksera predodređen je broj epsilon NKA za koje postoji akcija
    std::map<std::string, std::vector<std::pair<ENFA, Action*>>> stateEnfaMap;
public:
    ENFAManager() { };
    void setCurrentLexerState(std::string newState) { this->currentLexerState = newState; };
    void addLexerState(std::string newState);
    void addENFAForLexerState(std::string state, std::pair<ENFA, Action*> newEnfaActionPair);
    unsigned int run(std::string input);
};

// Stvara epsilon NKA za dani regularan izraz
class ENFARegexParser {
public:
    static ENFA* parseRegex(std::string regexExpression);
};

enum class MicroClass {
    NOVI_REDAK
};

// Definira određenu akciju
class Action {
private:
    std::vector<std::string> microActions;
public:
    Action(std::vector<std::string> microActions) {
        this->microActions = microActions;
    }

    void execute(SourceReader* activeReader) {
        for(std::string microAction : microActions) {
            // FIXME: Do something
            switch(microAction) {
                case "NOVI REDAK":
                    activeReader->incrementRowCounter();
                    break;
            }
        }
    }
};

// Čitač koda koji ulazi u analizator (lexer)
class SourceReader {
private:
    std::string* source;
    char* start;
    char* end;
    char* last;
    int currentRow;
    std::string phrase; // TODO: Možda prebaciti ovo u binarnu masku za praćenje svih stanja u kojima se eNKA nalazi
    std::string activeState;
    ENFAManager* statesEnfaManager;
public:
    SourceReader(std::string* source) {
        this->source = source;
        currentRow = 0;
        // ...
        // TODO: Implement the whole constructor
    };

    void analyze();
    void executeAction();
    void recoverFromError();
    void incrementRowCounter() { this->currentRow++; };
};

/**
 * Čita ulaznu datoteku i tokenizira ju po algoritmu iz PPJ udžbenika
 * (2.9.7 Program simulator zasnovan na tablici prijelaza epsilon-NKA)
 * 
 * Metoda započinje kompletan postupak leksičke analize
 */
void SourceReader::analyze() { /* TODO: Implement */ }

/**
 * S obzirom na utarnje varijable SourceReadera metoda pokreće akciju koja
 * je odgovarajuća
 * 
 * Ova metoda poziva se kada ENFAManager::run kao rezultat vrati 0 što simbolizira
 * da za posljednji ulaz nismo uspjeli dobiti niti jedno prihvatljivo stanje
 */
void SourceReader::executeAction() { /* TODO: Implement */ }

/**
 * Metoda izvršava algoritam oporavka od pogreške po udžbeniku PPJ
 */
void SourceReader::recoverFromError() { /* TODO: Implement */ }

/**
 * Stvara epsilon NKA tako da trenutnom dodaje početno stanje s epsilon prijelazon
 * na njega samoga i na drugi epsilon NKA
 *
 * @param other Pokazivač na drugi epsilon NKA
 * @return Pokazivač na novi epsilon NKA
 */
ENFA* ENFA::concat(ENFA* other) { /* TODO: Implement */ }

/**
 * Računa tranziciju sa trenutnih (current) stanja za ulaz (input)
 *
 * @param current Broj dobiven maskiranjem jedinica na pozicijama stanja koja su aktivna
 * @param inputStream Niz ulaznih znakova u automat za koje je potrebno "izračunati prihvatljivost"
 * @return Boolean koji govori je li stanje automata nakon tranzicije prihvatiljivo
 */
bool ENFA::run(std::string inputStream) { /* TODO: Implement */ }

/**
 * Dodaje novo stanje lexera u mapu svih stanja leksera i
 * stvara praznu listu epsilon NKA (regex) provjera koje vrijede
 * za to stanje lexera
 *
 * @param newState Stanje koje dodajem u mapu
 */
void ENFAManager::addLexerState(std::string newState) { /* TODO: Implement*/ }

/**
 * Dodaje epsilon NKA u listu NKA-ova za dano stanje leksera
 *
 * @param state Stanje za koje dodajemo epsilon NKA
 * @param newEnfa Epsilon NKA koji dodajemo
 */
void ENFAManager::addENFAForLexerState(std::string state, std::pair<ENFA, Action*> newEnfaActionPair) { /*TODO: Implement*/ }

/**
 * Za trenutno stanje leksera prolazi po svim epsilon NKA za dani ulazni niz
 * znak po znak
 *
 * @param input Ulaz u automat za koji je potrebno "izračunati" tranziciju
 * @return Broj koji nastaje maskiranjem bitova na pozicijama automata za koje je stanje prihvatljivo nakon prolaza
 */
unsigned int ENFAManager::run(std::string input) { /*TODO: Implement*/ }

#endif