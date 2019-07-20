#include "card.h"
#include "player.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

const char *SUIT_NAMES[] = {"Spades", "Hearts", "Clubs", "Diamonds"};

const char *SUIT_CHARS[] = {"♠", "♥", "♣", "♦"};

const char *SPOT_NAMES[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen",
                            "King", "Ace"};

const char *SPOT_CHARS[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q",
                            "K", "A"};

const char *ACTION_NAMES[] = {"Strike", "Dodge", "Peach", "Arrow Barrage", "Barbarian Invasion",
                              "Something for Nothing", "Bountiful Harvest", "Dismantle", "Snatch", "Duel"};

Card::Card(Spot spot, Suit suit, Action action) : spot(spot), suit(suit), action(action) {}

Spot Card::getSpot() const {
    return spot;
}

Suit Card::getSuit() const {
    return suit;
}

Action Card::getAction() const {
    return action;
}

std::string Card::toString() const {
    return std::string(SPOT_NAMES[spot]) + " of " + std::string(SUIT_NAMES[suit]) + ": " +
           std::string(ACTION_NAMES[action]);
}

static void printCardBlankLine() {
    cout << "│";
    for (size_t i = 0; i < CARD_WIDTH - 2; i++) {
        cout << " ";
    }
    cout << "│" << endl;
}

static void printCardWordLine(const string &word) {
    cout << "│";
    size_t num_spaces_left = (CARD_WIDTH - 2 - word.length()) / 2;
    size_t num_spaces_right = CARD_WIDTH - 2 - word.length() - num_spaces_left;
    for (size_t i = 0; i < num_spaces_left; i++) {
        cout << " ";
    }
    cout << word;
    for (size_t i = 0; i < num_spaces_right; i++) {
        cout << " ";
    }
    cout << "│" << endl;
}

void Card::printCard() const {
    vector<string> words;
    string token;
    istringstream iss(ACTION_NAMES[action]);
    while (iss >> token) {
        words.push_back(token);
    }
    cout << "┌";
    for (size_t i = 0; i < CARD_WIDTH - 2; i++) {
        cout << "─";
    }
    cout << "┐" << endl;
    cout << "│" << " " << SPOT_CHARS[spot];
    for (size_t i = 0; i < CARD_WIDTH - (strlen(SPOT_CHARS[spot]) == 1 ? 6 : 7); i++) {
        cout << " ";
    }
    cout << SUIT_CHARS[suit] << " " << "│" << endl;
    if (words.size() >= 3) {
        printCardBlankLine();
        for (const string &word : words) {
            printCardWordLine(word);
        }
        printCardBlankLine();
    } else if (words.size() == 2) {
        printCardBlankLine();
        for (const string &word : words) {
            printCardWordLine(word);
            printCardBlankLine();
        }
    } else {
        printCardBlankLine();
        printCardBlankLine();
        for (const string &word : words) {
            printCardWordLine(word);
        }
        printCardBlankLine();
        printCardBlankLine();
    }
    cout << "│" << " " << SUIT_CHARS[suit];
    for (size_t i = 0; i < CARD_WIDTH - (strlen(SPOT_CHARS[spot]) == 1 ? 6 : 7); i++) {
        cout << " ";
    }
    cout << SPOT_CHARS[spot] << " " << "│" << endl;
    cout << "└";
    for (size_t i = 0; i < CARD_WIDTH - 2; i++) {
        cout << "─";
    }
    cout << "┘" << endl;
}

void Strike::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    if (source->getStriked()) {
        throw NonPlayableCardException(this);
    } else {
        Player *target = source->selectTarget();
        if (target == source) {
            throw SelfTargetException(source);
        } else if (!target->getHealth()) {
            throw DeadTargetException(target);
        } else {
            source->printPlay(this, target);
            try {
                const Card *card = target->requestCard(DODGE);
                target->printPlay(card);
            } catch (DiscardException &e) {
                target->printHit(this);
                target->decreaseHealth();
            }
            source->setStriked(true);
        }
    }
}

void Dodge::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    throw NonPlayableCardException(this);
}

void Peach::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    if (source->getHealth() >= source->getHero()->getLife()) {
        throw NonPlayableCardException(this);
    } else {
        source->printPlay(this);
        source->increaseHealth();
    }
}

void ArrowBarrage::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    // TODO: Your implementation here
}

void BarbarianInvasion::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    // TODO: Your implementation here
}

void SomethingForNothing::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    source->printPlay(this);
    source->drawCards(2);
}

void BountifulHarvest::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    source->printPlay(this);
    for (Player *target: targets) {
        if (target->getHealth()) {
            target->drawCard();
        }
    }
}

void Dismantle::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    // TODO: Your implementation here
}

void Snatch::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    // TODO: Your implementation here
}

void Duel::takeEffect(Player *source, const std::vector<Player *> &targets) const {
    // TODO: Your implementation here
}
