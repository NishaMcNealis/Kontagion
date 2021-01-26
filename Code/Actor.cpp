#include "Actor.h"
#include "StudentWorld.h"

//#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//------------------------------ ACTOR ------------------------------//
Actor::Actor(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0) : GraphObject(imageID, startX, startY)
{
    m_alive = true;
    m_studWorld = sw;
    m_x = startX;
    m_y = startY;
}
Actor::~Actor()
{
    m_studWorld = nullptr;
}
bool Actor::isAlive() const
{
    return m_alive;
}
void Actor::setDead() 
{
    m_alive = false;
}
bool Actor::isEdible() const
{
    return false;
}
bool Actor::canBlock() const //defaults to false
{
    return false;
}
StudentWorld* Actor::getStudWorld() const
{
    return m_studWorld;
}
double Actor::getX() const
{
    return m_x;
}
double Actor::getY() const
{
    return m_y;
}
void Actor::setX(double x)
{
    m_x = x;
}
void Actor::setY(double y)
{
    m_y = y;
}

//------------------------------ SOCRATES ------------------------------//

Socrates::Socrates(StudentWorld* sw) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, sw)
{
    m_sprayCharges = 20;
    m_flameCharges = 5;
    m_health = 100;
    setX(0);
    setY(VIEW_HEIGHT/2);
}
int Socrates::getSprayCharges() const
{
    return m_sprayCharges;
}
int Socrates::getFlameCharges() const
{
    return m_flameCharges;
}
int Socrates::getHealth() const
{
    return m_health;
}
bool Socrates::isDamageable() const
{
    return false;
}
void Socrates::damage(int pointsLost)
{
    if (pointsLost < 0) return;
    m_health -= pointsLost;
    if (m_health <= 0)
    {
        setDead();
        getStudWorld()->playSound(SOUND_PLAYER_DIE);
    }
    else
    {
        getStudWorld()->playSound(SOUND_PLAYER_HURT);
    }
}
void Socrates::setHealth(int health)
{
    m_health = health;
}

void Socrates::addFlameThrowers(int amount)
{
    m_flameCharges += amount;
}
void Socrates::doSomething()
{
//    Socrates must check to see if he is currently alive (i.e., his hit points are above zero). If not, then Socrates’s doSomething() method must return immediately –
//    none of the following steps should be performed.
    if (!this->isAlive()) return;

    int ch;
    if (getStudWorld()->getKey(ch))
    {
    // user hit a key during this tick!
        switch (ch)
        {

                //    User asks to move clockwise or counterclockwise by pressing a
                //    directional key:
                //    i. Adjust Socrates’s positional angle in the Petri dish either clockwise
                //    or counterclockwise by 5 degrees.
                //    ii. Adjust Socrates’s (x, y) coordinates around the perimeter of the
                //    Petri dish based on his new positional angle.
                //    iii. Set the direction that Socrates is facing to his positional angle +
                //    180 degrees (causing him to always face toward the center of the
                //    Petri dish).
        case KEY_PRESS_LEFT:
        case 'a':
        //... move Socrates counterclockwise ...;
                moveSocrates(5.0);
                break;
        case KEY_PRESS_RIGHT:
        case 'd':
       // ... move Socrates clockwise...;
                moveSocrates(-5.0);
                break;
        case KEY_PRESS_SPACE: //SPRAY
                if (m_sprayCharges > 0)
                {
                    //    i. Add a spray object 2*SPRITE_RADIUS pixels directly in front of
                    //    himself
                    Spray* spr = new Spray(getStudWorld(), getX(), getY(), getDirection());
                    getStudWorld()->addActor(spr);
                    spr->setDirection(getDirection());
                    //Socrates’s spray count must decrease by 1.
                    m_sprayCharges --;
                    //Socrates must play the SOUND_PLAYER_SPRAY sound effect
                    getStudWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
        break;
         case KEY_PRESS_ENTER: //FLAME
                //    i. Add 16 flame objects exactly 2*SPRITE_RADIUS pixels away
                //    from himself, starting directly in front of Socrates and in 22 degree
                //    increments around Socrates (making a complete circle of flames
                //    around Socrates)
                if (m_flameCharges > 0)
                {
                    for (int i = getDirection(); i < 16*22 + getDirection(); i += 22)
                    {
                        getStudWorld()->addActor(new Flame(getStudWorld(), getX() + 2*SPRITE_RADIUS*cos(getStudWorld()->toRadians(i)), getY() + 2*SPRITE_RADIUS*sin(getStudWorld()->toRadians(i)), i));

                    }
                        
                    //    ii. Socrates’s flame thrower count must decrease by 1.
                    m_flameCharges --;
                }
                //    iii. Socrates must play the SOUND_PLAYER_FIRE sound effect
                getStudWorld()->playSound(SOUND_PLAYER_FIRE);
         break;
        }
    }
    else //the player did NOT press a key
    {
        if (m_sprayCharges < 20) //the maximum number of spray charges is 20
        {
            m_sprayCharges++;
        }
    }
}

void Socrates::moveSocrates(double amount)
{
    double x1, y1;
    x1 = 128 - VIEW_RADIUS * cos(getStudWorld()->toRadians(getDirection() + amount));
    y1 = 128 - VIEW_RADIUS * sin(getStudWorld()->toRadians(getDirection() + amount));
    moveTo(x1, y1);
    setDirection(getDirection() + amount);
    setX(x1);
    setY(y1);
}

//------------------------------ DIRT ------------------------------//

Dirt::Dirt(StudentWorld* sw, double x, double y) : Actor(IID_DIRT, x, y, sw, 90, 1){}

void Dirt::doSomething()
{
//dirt does nothing
}
bool Dirt::isDamageable() const
{
    return true;
}
void Dirt::damage(int pointsLost)
{
    setDead();
}
bool Dirt::canBlock() const
{
    return true;
}
//------------------------------ FOOD ------------------------------//
Food::Food(StudentWorld* sw, double x, double y) : Actor(IID_FOOD, x, y, sw, 90, 1){}

void Food::doSomething()
{
//food does nothing
}
bool Food::isDamageable() const
{
    return false;
}
bool Food::isEdible() const
{
    return true;
}
//------------------------------ WEAPON ------------------------------//
Weapon::Weapon(int imageID, StudentWorld* sw, double x, double y, Direction dir, int mP) : Actor(imageID, x, y, sw, dir, 1)
{
    m_maxPixels = mP;
    m_currPixels = 0;
    m_direction = dir;
}
int Weapon::getWeaponDirection()
{
    return m_direction;
}
void Weapon::doSomething()
{
    if (!isAlive()) return;
    Actor* firstDamegeableOverlap = getStudWorld()->overlapProperty(this, 2); // 2 = damageable
    if (firstDamegeableOverlap != nullptr)
    {
        firstDamegeableOverlap->damage(pointsLost());
        setDead();
        return;
    }
//    3. Otherwise, the flame will move forward in its current direction by
//    SPRITE_RADIUS*2 pixels.

    moveAngle(getWeaponDirection(), distance());
    setX(getX() + SPRITE_RADIUS*2 * cos(getStudWorld()->toRadians(getWeaponDirection()*1.0)));
    setY(getY() + SPRITE_RADIUS*2 * sin(getStudWorld()->toRadians(getWeaponDirection()*1.0)));
    m_currPixels += distance();

//    4. If the flame has moved a total of 32 pixels after step 3, then it immediately sets its
//    status to not alive (it dissipates) and can cause no more damage.
    if (m_currPixels >= m_maxPixels)
    {
        setDead();
    }
}
bool Weapon::isDamageable() const
{
    return false;
}
//------------------------------ FLAME ------------------------------//
Flame::Flame(StudentWorld* sw, double x, double y, Direction dir) : Weapon(IID_FLAME, sw, x, y, dir, 32) {}
                                                                                         
void Flame::doSomething()
{
    Weapon::doSomething();
}
int Flame::distance()
{
    return SPRITE_RADIUS*2;
}
int Flame::pointsLost()
{
    return 5;
}
//------------------------------ SPRAY ------------------------------//
Spray::Spray(StudentWorld* sw, double x, double y, Direction dir) : Weapon(IID_SPRAY, sw, x, y, dir, 112) {}
                                                                                         
void Spray::doSomething()
{
    Weapon::doSomething();
}
int Spray::distance()
{
    return SPRITE_WIDTH;
}
int Spray::pointsLost()
{
    return 2;
}
//------------------------------ FLAME THROWER ------------------------------//
FlameThrower::FlameThrower(StudentWorld* sw, double x, double y) : Goodie(IID_FLAME_THROWER_GOODIE, sw, x, y) {}

void FlameThrower::goodieAction()
{
    getStudWorld()->increaseScore(300);
    getStudWorld()->getPlayer()->addFlameThrowers(5);
}
//------------------------------ GOODIE ------------------------------//
Goodie::Goodie(int imageID, StudentWorld* sw, double x, double y) : Actor(imageID, x, y, sw, 0, 1)
{
    m_lifetime = max(randInt(0, 300 - (10 * sw->getLevel() - 1)), 50);
}

void Goodie::doSomething()
{
    //The goodie must check to see if it is currently alive. If not, then its
//    doSomething() method must return immediately – none of the following steps
//    should be performed. Otherwise
    if (!isAlive()) return;
//    The goodie must determine if it overlaps with Socrates. If so, then
//    the goodie must:
    if (getStudWorld()->overlap(this, getStudWorld()->getPlayer(), SPRITE_WIDTH))
    {
        goodieAction();
        setDead();
        getStudWorld()->playSound(SOUND_GOT_GOODIE);
    }
    m_lifetime --;
    if (m_lifetime <= 0) setDead();
}
bool Goodie::isDamageable() const
{
    return true;
}

void Goodie::damage(int pointsLost)
{
    setDead();
}

//------------------------------ RESTORE HEALTH ------------------------------//
RestoreHealth::RestoreHealth(StudentWorld* sw, double x, double y) : Goodie(IID_RESTORE_HEALTH_GOODIE, sw, x, y) {}

void RestoreHealth::goodieAction()
{
    getStudWorld()->increaseScore(250);
    getStudWorld()->getPlayer()->setHealth(100);
}

//------------------------------ EXTRA LIFE ------------------------------//
ExtraLife::ExtraLife(StudentWorld* sw, double x, double y) : Goodie(IID_EXTRA_LIFE_GOODIE, sw, x, y) {}

void ExtraLife::goodieAction()
{
    getStudWorld()->increaseScore(500);
    getStudWorld()->incLives();
}
//------------------------------ FUNGUS ------------------------------//
Fungus::Fungus(StudentWorld* sw, double x, double y) : Goodie(IID_FUNGUS, sw, x, y) {}

void Fungus::goodieAction()
{
    getStudWorld()->increaseScore(-50);
    getStudWorld()->getPlayer()->damage(20);
}

//------------------------------ PIT ------------------------------//
Pit::Pit(StudentWorld* sw, double x, double y) : Actor(IID_PIT, x, y, sw, 0, 1)
{
    m_regularSalmonella = 5;
    m_aggressiveSalmonella = 3;
    m_eColi = 2;
}
int Pit::bacteriaType()
{
    if (m_regularSalmonella > 0)
    {
        if (m_aggressiveSalmonella > 0)
        {
            if (m_eColi > 0)
            {
                return randInt(1, 3);
            }
            return randInt(1, 2);
        }
        return 1;
    }
    if (m_aggressiveSalmonella > 0)
    {
        if (m_eColi > 0)
        {
            return randInt(2, 3);
        }
        return 2;
    }
    if (m_eColi > 0)
    {
        return 3;
    }
    return -1;
}
void Pit::doSomething()
{
    if (m_regularSalmonella == 0 && m_aggressiveSalmonella == 0 && m_eColi == 0)
    {
        //Inform the StudentWorld object that it has emitted all of its bacteria
        getStudWorld()->addNumPitsEmpty();
        setDead();
    }
    if (randInt(1, 50) == 1)  // there is a 1 in 50 chance that during a tick, a given pit will emit a bacterium
    {
//        Considering only the types of bacteria that is has not yet run out of, the pit
//        must randomly select one of those types (each type being equally likely to be
//        chosen).
        int bT = bacteriaType();
        if (bT == 1)
        {
            getStudWorld()->addActor(new RegularSalmonella(getStudWorld(), getX(), getY()));
            m_regularSalmonella--;
        }
        else if (bT == 2)
        {
            getStudWorld()->addActor(new AggressiveSalmonella(getStudWorld(), getX(), getY()));
            m_aggressiveSalmonella--;
        }
        else if (bT == 3)
        {
            getStudWorld()->addActor(new EColi(getStudWorld(), getX(), getY()));
            m_eColi--;
        }
        getStudWorld()->playSound(SOUND_BACTERIUM_BORN); //play a sound effect to indicate that the
//        bacterium was just born
    }
    
}
bool Pit::isDamageable() const
{
    return false;
}

//------------------------------ BACTERIA ------------------------------//
Bacteria::Bacteria(int imageID, StudentWorld* sw, double x, double y, int hitPoints) : Actor(imageID, x, y, sw, 90, 0)
{
    m_movementPlanDistance = 0;
    m_hitPoints = hitPoints;
    m_currFood = 0;
    setDirection(90);
    getStudWorld()->addNumBacteriaAlive(1);
}

void Bacteria::doSomething()
{
    if (!isAlive()) return;
    specificBacteriaBehavior();
    //call functions here so they don't have to be called multiple times
    Actor* foodOverlap = getStudWorld()->overlapProperty(this, 1);
    bool playerOverlap = getStudWorld()->overlap(this, getStudWorld()->getPlayer(), SPRITE_WIDTH);
//    check to see if it overlaps with Socrates. If so it
//    will:
    if (playerOverlap)
    {
        getStudWorld()->getPlayer()->damage(damagePower());
    }
    else if (m_currFood == 3)
    {
        clone();
    }
    else if (foodOverlap != nullptr)
    {
        m_currFood ++;
        foodOverlap->setDead();
    }
    if (m_doMovementPlan)
    {
        if (m_movementPlanDistance > 0) //wants to continue moving the same direction it was previously moving in
        {
            m_movementPlanDistance--;
            if (!moveForwardIfPossible(3, 3)) moveRandomly();
        }
        else
        {
            Actor* targetFood = getStudWorld()->overlapProperty(this, 1);
            if (targetFood == nullptr) //If no food can be found within 128 pixels
            {
                moveRandomly();
                return;
            }
            if (!moveForwardIfPossible(3, 3)) moveRandomly(); //food can be found within 128 pixels, so the regular salmonella will try to
            //    move toward that food.

        }
    }
}
void Bacteria::clone()
{
    double newX, newY;
    if(getX() < VIEW_WIDTH/2)
    {
        newX = getX() + SPRITE_WIDTH/2;
    }
    else if(getX() > VIEW_WIDTH/2)
    {
        newX = getX() - SPRITE_WIDTH/2;
    }
    else // x coordinate is equal to VIEW_WIDTH/2
    {
        newX = getX();
    }
    if(getY() < VIEW_HEIGHT/2)
    {
        newY = getY() + SPRITE_WIDTH/2;
    }
    else if(getX() > VIEW_HEIGHT/2)
    {
        newY = getY() - SPRITE_WIDTH/2;
    }
    else // y coordinate is equal to VIEW_HEIGHT/2
    {
        newY = getY();
    }
    addBacteria(newX, newY); //add a new regular salmonella object at the specified (newx, newy)
    //    coordinate.
    m_currFood = 0; //reset its food-eaten count back to zero
}
void Bacteria::setDoMovementPlan(bool value)
{
    m_doMovementPlan = value;
}
bool Bacteria::moveForwardIfPossible(int dist, int amount)
{
    if (!isBlocked(dist))
    {
       double newX, newY;
        getPositionInThisDirection(getDirection(), amount, newX, newY);
        moveAngle(getDirection(), amount);
       setX(newX);
       setY(newY);
       return true;
   }
    return false;
}
void Bacteria::moveRandomly()
{
    double randDirection = randInt(0, 359);
    setDirection(randDirection);
    m_movementPlanDistance = 10;
}
bool Bacteria::isBlocked(int dist)
{
    double newX, newY;
    getPositionInThisDirection(getDirection(), dist, newX, newY);
    return (isBlockedByDish(newX, newY) || isBlockedByDirt(dist, newX, newY)); //= nullptr if there is no dirt blocking bacteria
}
bool Bacteria::isBlockedByDish(double newX, double newY)
{
    if (getStudWorld()->distance(newX, newY, VIEW_WIDTH/2, VIEW_HEIGHT/2) >= VIEW_RADIUS)
    {
        return true;
    }
    return false;
}
bool Bacteria::isBlockedByDirt(int dist, double newX, double newY)
{
    return (getStudWorld()->overlapProperty(newX, newY, 3, dist) != nullptr); //= nullptr if there is no dirt blocking bacteria
}
bool Bacteria::isDamageable() const
{
    return true;
}
void Bacteria::damage(int pointsLost)
{
     m_hitPoints -= pointsLost;
    if (m_hitPoints <= 0 && isAlive())
    {
        getStudWorld()->addNumBacteriaAlive(-1);
        getStudWorld()->playSound(getDeathSound());
        getStudWorld()->increaseScore(100);
        setDead();
        if (randInt(1, 2) == 1) //there is a 50% chance it will turn into food before it dies.
        {
            getStudWorld()->addActor(new Food(getStudWorld(), getX(), getY()));
        }
    }
    else
    {
        getStudWorld()->playSound(getHurtSound());
    }
}

//------------------------------  SALMONELLA ------------------------------//
Salmonella::Salmonella(StudentWorld* sw, double x, double y, int hitPoints) : Bacteria(IID_SALMONELLA, sw, x, y, hitPoints)
{ }

int Salmonella::getDeathSound() const
{
    return SOUND_SALMONELLA_DIE;
}
int Salmonella::getHurtSound() const
{
    return SOUND_SALMONELLA_DIE;
}
//------------------------------ REGULAR SALMONELLA ------------------------------//
RegularSalmonella::RegularSalmonella(StudentWorld* sw, double x, double y) : Salmonella(sw, x, y, 4)
{
    setDoMovementPlan(true);
}

void RegularSalmonella::addBacteria(double x, double y)
{
    getStudWorld()->addActor(new RegularSalmonella(getStudWorld(), x, y));
}
void RegularSalmonella::specificBacteriaBehavior() {}

int RegularSalmonella::damagePower()
{
    return 1;
}
//------------------------------ AGGRESSIVE SALMONELLA ------------------------------//
AggressiveSalmonella::AggressiveSalmonella(StudentWorld* sw, double x, double y) : Salmonella(sw, x, y, 10) {}

void AggressiveSalmonella::addBacteria(double x, double y)
{
    getStudWorld()->addActor(new AggressiveSalmonella(getStudWorld(), x, y));
}
void AggressiveSalmonella::specificBacteriaBehavior()
{
    if (getStudWorld()->distance(this, getStudWorld()->getPlayer()) <= 72)
    {
        setDirection(getStudWorld()->getAngleToPlayer(this));
        
        if (moveForwardIfPossible(SPRITE_WIDTH/2, 3))
        {
            setDoMovementPlan(false);
        }
    }
    else setDoMovementPlan(true);
}

int AggressiveSalmonella::damagePower()
{
    return 2;
}
//------------------------------ E. COLI ------------------------------//
EColi::EColi(StudentWorld* sw, double x, double y) : Bacteria(IID_ECOLI, sw, x, y, 5)
{
    setDoMovementPlan(true);
}

void EColi::addBacteria(double x, double y)
{
    getStudWorld()->addActor(new EColi(getStudWorld(), x, y));
}
void EColi::specificBacteriaBehavior()
{
    if (getStudWorld()->distance(this, getStudWorld()->getPlayer()) <= 256)
       {
           double theta = getStudWorld()->getAngleToPlayer(this);
           setDirection(theta);
           if (moveForwardIfPossible(SPRITE_WIDTH/2, 2))
           {
               setDoMovementPlan(false);
           }
       }
       else setDoMovementPlan(true);
}

int EColi::getDeathSound() const
{
    return SOUND_ECOLI_DIE;
}
int EColi::getHurtSound() const
{
    return SOUND_ECOLI_HURT;
}
int EColi::damagePower()
{
    return 4;
}
