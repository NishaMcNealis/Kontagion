#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"

#include "SoundFX.h"
#include "Actor.h"
#include <vector>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_initialized = false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}
//GETTERS
Socrates* StudentWorld::getPlayer() const
{
    return m_player;
}

void StudentWorld::addActor(Actor* a)
{
    actors.push_back(a);
}
//The init() method is responsible for constructing a representation of the current level in
//your StudentWorld object and populating it with initial objects (e.g., pits, food, dirt piles
//and Socrates), using one or more data structures that you come up with.
int StudentWorld::init()
{
    m_numPitsEmpty = 0;
    m_numBacteriaAlive = 0;
    m_player = new Socrates(this);
    double randX, randY;
    //    Add max(180 – 20 * L, 20) dirt objects to the Petri dish at random locations, in a
    //    manner such that no dirt objects overlap with previously-placed food objects
    //    or pits (their centers are at least 2*SPRITE_RADIUS pixels apart from each
    //    other). It is OK for dirt objects to overlap with each other, however. Each dirt
    //    object must be no more 120 pixels from the center of the Petri dish which is at
    //    (VIEW_WIDTH/2, VIEW_HEIGHT/2).
    for (int i = 0; i < max(180 - (20 * getLevel()), 20);)
        {
            randX = randInt(0, VIEW_WIDTH);
            randY = randInt(0, VIEW_HEIGHT);
            if (isValidDirt(randX, randY))
            {
                Actor* a = new Dirt(this, randX, randY);
                actors.push_back(a);
                i++;
            }
            
        }
    //    Add min(5 * L, 25) food objects to the Petri dish at random locations, in a manner
    //    such that no two food objects overlap with each other or previously-placed
    //    pits (their centers must be at least 2*SPRITE_RADIUS pixels apart from each
    //    other). Each food object must be no more 120 pixels from the center of the Petri
    //    dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2).
        for (int i = 0; i < min(5 * getLevel(), 25);)
        {
            randX = randInt(0, VIEW_WIDTH);
            randY = randInt(0, VIEW_HEIGHT);
            if (isValidFoodOrPit(randX, randY))
            {
                Actor* a = new Food(this, randX, randY);
                actors.push_back(a);
                i++;
            }
            
        }
//    Add L pits to the Petri dish at random locations, in a manner such that no two pits
//    overlap with each other (their centers are at least 2*SPRITE_RADIUS pixels
//    apart from each other). Each pit must be no more 120 pixels from the center of the
//    Petri dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2)
    for (int i = 0; i < getLevel();)
    {
        randX = randInt(0, VIEW_WIDTH);
        randY = randInt(0, VIEW_HEIGHT);
        if (isValidFoodOrPit(randX, randY))
        {
            Actor* a = new Pit(this, randX, randY);
            actors.push_back(a);
            i++;
        }
        
    }
    m_initialized = true;
    return GWSTATUS_CONTINUE_GAME; //must return GWSTATUS_CONTINUE_GAME.
}
bool StudentWorld::isValidFoodOrPit(double x, double y)
{
    if (distance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120)
    {
        return false; //Each pit must be no more 120 pixels from the center
    }
    vector<Actor*>::iterator itr = actors.begin();
    while (itr != actors.end())
    {
        if (distance(*itr, x, y) <= 2*SPRITE_RADIUS)
        {
            return false;
        }
        itr++;
    }
    
    return true;
}
bool StudentWorld::isValidDirt(double x, double y) const
{
    if (distance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120)
    {
        return false; //Each pit must be no more 120 pixels from the center
    }
    return true;
}
double StudentWorld::distance(Actor* a, Actor* b) const
{
    return distance(a->getX(), a->getY(), b->getX(), b->getY());
}
double StudentWorld::distance(Actor* a, double x, double y) const
{
    return distance(a->getX(), a->getY(), x, y);
}
double StudentWorld::distance(double x1, double y1, double x2, double y2) const
{
    return sqrt(
                (pow(x1 - x2, 2)) +
                (pow(y1 - y2, 2))
                ) * 1.0;
}
// Each time the move() method is called, it must run a single tick of the game. This
//means that it is responsible for asking each of the game actors (e.g., Socrates, each
//bacterium, each goodie, each projectile, food, fungi, pits, etc.) to try to do something:
//e.g., move themselves and/or perform their specified behavior. This method might also
//introduce new actors into the game, for instance adding a new flame thrower goodie into
//the Petri dish. Finally, this method is responsible for disposing of (i.e., deleting) actors
//that need to disappear during a given tick
int StudentWorld::move()
{

    m_player->doSomething();
    vector<Actor*>::iterator itr = actors.begin();
    while (itr != actors.end())
    {
        if ((*itr)->isAlive())
        {
            (*itr)->doSomething();
        }
        if (!m_player->isAlive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (numPitsEmpty() == getLevel() && numBacteriaAlive() <= 0)
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        itr++;
    }
    //    // Potentially add new actors to the game (e.g., goodies or fungi)
    addNewActors();
    //    // Update the Game Status Line
    updateDisplayText(); // update the score/lives/level text at screen top
    //    // the player hasn’t completed the current level and hasn’t died, so
    //    // continue playing the current level
    
        // Remove newly-dead actors after each tick
    removeDeadGameObjects(); // delete dead game objects
    return GWSTATUS_CONTINUE_GAME;



}
int StudentWorld::numPitsEmpty() const
{
    return m_numPitsEmpty;
}
void StudentWorld::addNumPitsEmpty()
{
    m_numPitsEmpty++;
}
int StudentWorld::numBacteriaAlive() const
{
    return m_numBacteriaAlive;
}
void StudentWorld::addNumBacteriaAlive(int change)
{
    m_numBacteriaAlive += change;
}
void StudentWorld::updateDisplayText()
{
    //eg. Score: 004500 Level: 4 Lives: 3 Health: 82 Sprays: 16 Flames: 4
    ostringstream oss;
    oss << "Score: ";
    if (getScore() < 0)
    {
        oss << "-";
        oss.fill('0');
        oss << setw(5) << getScore() * -1;
    }
    else
    {
        oss.fill('0');
        oss << setw(6) << getScore();
    }
    oss << "  Level: ";
    oss << getLevel();
    oss << "  Lives: ";
    oss << getLives();
    oss << "  Health: ";
    oss << getPlayer()->getHealth();
    oss << "  Sprays: ";
    oss << getPlayer()->getSprayCharges();
    oss <<   "  Flames: ";
    oss << getPlayer()->getFlameCharges();
    setGameStatText(oss.str());
}
void StudentWorld::addNewActors()
{
    //Add fungus
    int randNum = randInt(0,  max(510 - getLevel() * 10, 200));
    if (randNum == 0)
    {
        int angle = randInt(0, 359);
        double x = VIEW_WIDTH/2 + VIEW_RADIUS * cos(toRadians(angle));
        double y = VIEW_HEIGHT/2  + VIEW_RADIUS * sin(toRadians(angle));
        actors.push_back(new Fungus(this, x, y));
    }
    //Add goodies
    randNum = randInt(0,  max(510 - getLevel() * 10, 250));
//    If the random number is 0, then add a new goodie at a random angle/position
//    around the circumference of the Petri dish, exactly VIEW_RADIUS pixels from
//    the center of the Petri dish
    if (randNum == 0)
    {
        int angle = randInt(0, 359);
        double x = VIEW_WIDTH/2 + VIEW_RADIUS * cos(toRadians(angle));
        double y = VIEW_HEIGHT/2  + VIEW_RADIUS * sin(toRadians(angle));
        int pickGoodie = randInt(1, 100);
        //    A. There is a 60% chance the goodie will be a restore health goodie.
        if (pickGoodie <= 60)
        {
            actors.push_back(new RestoreHealth(this, x, y));
        }
        //    B. There is a 30% chance the goodie will be a flamethrower goodie.
        else if (pickGoodie <= 90)
        {
            actors.push_back(new FlameThrower(this, x, y));
        }
        //    C. There is a 10% chance the goodie will be an extra-life goodie.
        else
        {
            actors.push_back(new ExtraLife(this, x, y));
        }
    }

}
void StudentWorld::removeDeadGameObjects()
{
    vector<Actor*>::iterator itr = actors.begin();
    while (itr != actors.end())
    {
        if (!(*itr)->isAlive())
        {
            delete (*itr);
            itr = actors.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}
void StudentWorld::cleanUp()
{
    if (m_initialized)
    {
        delete m_player;
        vector<Actor*>::iterator itr = actors.begin();
        while (itr != actors.end())
        {
            delete (*itr);
            itr = actors.erase(itr);
        }
    }
    m_initialized = false;
    
}

bool StudentWorld::overlap(Actor* a, Actor *b, int dist) const
{
    return(distance(a, b) <= dist);
}
bool StudentWorld::overlap(Actor *a, double x, double y, int dist) const
{
    return(distance(a, x, y) <= dist);
}
Actor* StudentWorld::overlapProperty(Actor* a, int property)
{
    return (overlapProperty(a->getX(), a->getY(), property, SPRITE_WIDTH));
}
Actor* StudentWorld::overlapProperty(double x, double y, int property, int dist)
{
    vector<Actor*>::iterator itr = actors.begin();
    while (itr != actors.end())
    {
        if(passTest(*itr, property) && overlap(*itr, x, y, dist))
        {
            return (*itr);
        }
        itr++;
    }
    return nullptr;
}
bool StudentWorld::passTest(Actor* a, int property)
{
    switch (property)
    {
        case (1): //food
            return (a->isEdible());
        case (2): //damageable
            return (a->isDamageable());
        case (3): //can block
            return (a->canBlock());
    }
    return false;
}

double StudentWorld::toRadians(double degrees)
{
    const double PI = 4 * atan(1);
    return (degrees * PI/180);
}
double StudentWorld::toDegrees(double radians)
{
    const double PI = 4 * atan(1);
    return (radians * 180/PI);
}
double StudentWorld::getAngleToPlayer(Actor* a)
{
    double xDist = a->getX() - getPlayer()->getX();
    double yDist = a->getY() - getPlayer()->getY();
    double theta;
    if (a->getX() > getPlayer()->getX() && a->getY() > getPlayer()->getY())
         theta = (270 - toDegrees(atan(xDist/yDist)));
    else if (a->getX() <= getPlayer()->getX() && a->getY() > getPlayer()->getY())
        theta = (270 - toDegrees(atan(xDist/yDist)));
    else if (a->getX() > getPlayer()->getX() && a->getY() <= getPlayer()->getY())
        theta = (180 + toDegrees(atan(yDist/xDist)));
    else // (a->getX() <= getPlayer()->getX() && a->getY() <= getPlayer()->getY())
        theta = (toDegrees(atan(yDist/xDist)));
    return theta;
}
