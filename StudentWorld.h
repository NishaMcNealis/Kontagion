#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;
class Socrates;

//Responsible for keeping track
//of your game world and all of the actors/objects
//(Socrates, bacteria, projectiles,
//goodies, piles of dirt, pits, food, fungi, etc.)
//that are inside the game.

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move(); //runs a single tick of the game by calling doSomething on all alive actors

    virtual void cleanUp();
    
    void addActor(Actor* a); //adds an Actor to the StudentWorld vector of Actors

    Socrates* getPlayer() const;
    bool overlap(Actor* a, Actor *b, int dist) const; //determines whether a and b overlap by a distance <= dist

    Actor* overlapProperty(Actor* a, int property);
    Actor* overlapProperty(double x, double y, int property, int dist);
    
    
    void addNumPitsEmpty(); //increases the number of empty pits when a pit calls it, signaling that it is empty
    void addNumBacteriaAlive(int change);
    
    double distance(double x1, double y1, double x2, double y2) const;
    double distance(Actor* a, Actor* b) const; //returns euclidean distance between a and b
    double getAngleToPlayer(Actor* a); //returns angle from a to Socrates
    
    double toRadians(double degrees);
    double toDegrees(double radians);
    

private:
    
    std::vector<Actor*> actors;
    Socrates* m_player;
    int m_numPitsEmpty;
    int m_numBacteriaAlive;
    bool m_initialized;
    bool isValidFoodOrPit(double x, double y);
    bool isValidDirt(double x, double y) const;
    double distance(Actor* a, double x, double y) const;
    bool passTest(Actor* actor, int property);
    void removeDeadGameObjects();
    int numBacteriaAlive() const;
    int numPitsEmpty() const;
    void updateDisplayText();
    void addNewActors();

    
    bool overlap(Actor *a, double x, double y, int dist) const;
    
};

#endif // STUDENTWORLD_H_
