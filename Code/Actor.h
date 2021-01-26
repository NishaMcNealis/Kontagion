#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//------------------------------------------------------------------------------------------//
class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* sw, Direction dir, int depth, double size);
    virtual ~Actor();
    
    virtual void doSomething() = 0; //runs every time the StudentWorld object calls move. Calls on each actor to perform any necessary actions (eg. movement around the screen)
    
    virtual void damage(int pointsLost) = 0; //subtracts hit points from an Actor where applicable, kills an Actor when it has been sufficiently damaged
    
    virtual bool isDamageable() const = 0;
    virtual bool canBlock() const; //not pure virtual b/c defaults to false
                            //returns whether an Actor can block others (eg, dirt) or not

    virtual bool isEdible() const;
    bool isAlive() const; //sets an Actor’s m_alive status to false (dead)
    
    //getters and setters
    void setDead();
    StudentWorld* getStudWorld() const;
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    
private:
    bool m_alive;
    double m_x;
    double m_y;
    StudentWorld* m_studWorld;
    
};
#endif // ACTOR_H_

//------------------------------------------------------------------------------------------//

#ifndef SOCRATES_H_
#define SOCRATES_H_
class Socrates: public Actor
{
public:
    Socrates(StudentWorld* studWorld);
    void doSomething();
    bool isDamageable() const;
    void damage(int pointsLost); //decrements Socrates’ health points and kills him if necessary
    void setHealth(int health);
    void addFlameThrowers(int amount); //increases number of flameThrowers Socrates has
    
    //getters
    int getHealth() const;
    int getSprayCharges() const;
    int getFlameCharges() const;
private:
    int m_sprayCharges;
    int m_flameCharges;
    int m_health;
    void moveSocrates(double amount);
};
#endif // SOCRATES_H_

//------------------------------------------------------------------------------------------//

#ifndef DIRT_H_
#define DIRT_H_
class Dirt: public Actor
{
public:
    Dirt(StudentWorld* sw, double x, double y);
    void doSomething(); //does nothing
    bool isDamageable() const;
    void damage(int pointsLost);
    bool canBlock() const; //returns true because Dirt can block
};
#endif // DIRT_H_

//------------------------------------------------------------------------------------------//

#ifndef FOOD_H_
#define FOOD_H_
class Food: public Actor
{
public:
    Food(StudentWorld* sw, double x, double y);
    void doSomething(); //does nothing
    bool isDamageable() const;
    void damage(int pointsLost) {}
    bool isEdible() const; //returns false because Food can’t be damaged


};
#endif // FOOD_H_

//------------------------------------------------------------------------------------------//

#ifndef WEAPON_H_
#define WEAPON_H_
class Weapon: public Actor
{
public:
    Weapon(int imageID, StudentWorld* sw, double x, double y, Direction dir, int maxPixels);
    virtual void doSomething(); //if the weapon is alive, checks if the weapon is overlapping with any damageable object and damages that object accordingly

    int getWeaponDirection();
    bool isDamageable() const;
    void damage(int pointsLost) {}
    virtual int distance() = 0;
    virtual int pointsLost() = 0; //returns how many points a specific Weapon should take away from an Actor it hits

private:
    int m_currPixels;
    int m_maxPixels;
    int m_direction;

};
#endif // WEAPON_H_
#ifndef FLAME_H_
#define FLAME_H_
class Flame: public Weapon
{
public:
    Flame(StudentWorld* sw, double x, double y, Direction dir);
    void doSomething(); //calls doSomething in its base class Weapon

    int distance();
    int pointsLost();
};
#endif // FLAME_H_

#ifndef SPRAY_H_
#define SPRAY_H_
class Spray: public Weapon
{
public:
    Spray(StudentWorld* sw, double x, double y, Direction dir);
    void doSomething(); //calls doSomething in its base class Weapon

    int distance();
    int pointsLost();
};
#endif // SPRAY_H_

//------------------------------------------------------------------------------------------//

#ifndef PIT_H_
#define PIT_H_
class Pit: public Actor
{
public:
    Pit(StudentWorld* sw, double x, double y);
    void doSomething(); //if the pit has emitted all of its bacteria, it should set itself to dead and tell StudentWorld that it has died. Otherwise, there is a 1 in 50 chance that the pit will emit a bacterium

    bool isDamageable() const;
    void damage(int pointsLost) {}

private:
    int m_regularSalmonella;
    int m_aggressiveSalmonella;
    int m_eColi;
    int bacteriaType();
};
#endif // PIT_H_

//------------------------------------------------------------------------------------------//

#ifndef GOODIE_H_
#define GOODIE_H_
class Goodie: public Actor
{
public:
    Goodie(int imageID, StudentWorld* sw, double x, double y);
    void doSomething(); //if the Goodie is alive, it should determine if it overlaps with Socrates. If so, it should perform its specific goodie action and set itself to dead
    bool isDamageable() const;
    void damage(int pointsLost);
    virtual void goodieAction() = 0;

private:
    int m_lifetime;

};
#endif // GOODIE_H_

#ifndef RESTOREHEALTH_H_
#define RESTOREHEALTH_H_
class RestoreHealth: public Goodie
{
public:
    RestoreHealth(StudentWorld* sw, double x, double y);
    void goodieAction();
};
#endif // RESTOREHEALTH_H_

#ifndef FLAMETHROWER_H_
#define FLAMETHROWER_H_
class FlameThrower: public Goodie
{
public:
    FlameThrower(StudentWorld* sw, double x, double y);
    void goodieAction();
};
#endif // FLAMETHROWER_H_

#ifndef EXTRALIFE_H_
#define EXTRALIFE_H_
class ExtraLife: public Goodie
{
public:
    ExtraLife(StudentWorld* sw, double x, double y);
    void goodieAction();
};
#endif // EXTRALIFE_H_

#ifndef FUNGUS_H_
#define FUNGUS_H_
class Fungus: public Goodie
{
public:
    Fungus(StudentWorld* sw, double x, double y);
    void goodieAction();
};
#endif // FUNGUS_H_

//------------------------------------------------------------------------------------------//

#ifndef BACTERIA_H_
#define BACTERIA_H_
class Bacteria: public Actor
{
public:
    Bacteria(int imageID, StudentWorld* sw, double x, double y, int hitPoints);
    void doSomething();
    virtual void specificBacteriaBehavior() = 0;
    
    bool isDamageable() const;
    void damage(int pointsLost); //decrement the Bacteria’s hit points and set Bacteria to dead if necessary
    
    virtual void addBacteria(double x, double y) = 0;
    bool moveForwardIfPossible(int dist, int amount);
    
    bool isBlocked(int dist);
    void setDoMovementPlan(bool value);
    
    virtual int getHurtSound() const = 0; //returns the sound a type of Bacteria makes when it is hurt
    virtual int getDeathSound() const = 0; //returns the sound a type of Bacteria makes when it dies
    virtual int damagePower() = 0;
private:
    int m_movementPlanDistance;
    int m_hitPoints;
    int m_currFood;
    bool m_doMovementPlan;
    void moveRandomly();
    bool isBlockedByDish(double newX, double newY);
    bool isBlockedByDirt(int dist, double newX, double newY);
    void clone();
    
    
    
};
#endif // GOODIE_H_

#ifndef SALMONELLA_H_
#define SALMONELLA_H_
class Salmonella: public Bacteria
{
public:
    Salmonella(StudentWorld* sw, double x, double y, int hitPoints);
    virtual void specificBacteriaBehavior() = 0;
    virtual void addBacteria(double x, double y) = 0;
    int getHurtSound() const;
    int getDeathSound() const;
};
#endif // REGULARSALMONELLA_H_


#ifndef REGULARSALMONELLA_H_
#define REGULARSALMONELLA_H_
class RegularSalmonella: public Salmonella
{
public:
    RegularSalmonella(StudentWorld* sw, double x, double y);
    void addBacteria(double x, double y);
    void specificBacteriaBehavior(); //does nothing
    int damagePower();
};
#endif // REGULARSALMONELLA_H_

#ifndef AGGRESSIVESALMONELLA_H_
#define AGGRESSIVESALMONELLA_H_
class AggressiveSalmonella: public Salmonella
{
public:
    AggressiveSalmonella(StudentWorld* sw, double x, double y);
    void addBacteria(double x, double y);
    void specificBacteriaBehavior(); //follows the player if the player is within 72 units
    int damagePower();
};
#endif // AGGRESSIVESALMONELLA_H_

#ifndef ECOLI_H_
#define ECOLI_H_
class EColi: public Bacteria
{
public:
    EColi(StudentWorld* sw, double x, double y);
    void addBacteria(double x, double y);
    int getHurtSound() const;
    int getDeathSound() const;
    int damagePower();
    void specificBacteriaBehavior(); //follows the player if the player is within 256 units

};
#endif // ECOLI_H_
