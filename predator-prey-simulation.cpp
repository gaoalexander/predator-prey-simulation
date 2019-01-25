

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int GRID_SIZE = 20;
const int DB_INIT = 5;
const int ANT_INIT = 100;

class Organism;
class DoodleBug;
class Ant;
class World;  //Contains WORLD GRID

class Organism {
public:
    friend class World;
    friend class DoodleBug;
    friend class Ant;
    
    Organism(int xVal, int yVal, World globe); // Constructor
    ~Organism(); //Destructor
    
    virtual void move(World& globe);
    int get_posX();
    int get_posY();
    virtual int get_ID();
    int get_timeSinceBreeding();
    void set_timeSinceBreeding(int& timeSinceBreeding);
    virtual int get_timeSinceEating();
    virtual bool get_movedStatus();
    virtual void set_movedStatus(bool movedStatus);
    
private:
    int posX, posY, timeSinceBreeding;
    int ID = 0;
    bool hasMoved;
};

// DOODLEBUG DEFINITION

class DoodleBug : public Organism{
public:
    friend class World;
    friend class Organism;
    friend class Ant;
    
    DoodleBug(int xVal, int yVal, World globe);
    DoodleBug(const DoodleBug& dbCopy);
    ~DoodleBug();
    
    virtual void move(World& globe);
    void breed(World& globe);
    void starve(World& globe);
    virtual int get_timeSinceEating();
    virtual int get_ID();
    void set_timeSinceEating(int new_timeSinceEating);
    virtual bool get_movedStatus();
    virtual void set_movedStatus(bool movedStatus);
    
private:
    int timeSinceEating; //if this value reaches 3, DoodleBug starves and dies.
    int ID = 2;
    bool hasMoved;
};


// ANT DEFINITION

class Ant : public Organism{
public:
    friend class World;
    friend class Organism;
    friend class DoodleBug;
    
    Ant(int xVal, int yVal, World globe);
    Ant(const Ant& antCopy);
    ~Ant();
    
    virtual void move(World& globe);
    void breed(World& globe);
    virtual int get_ID();
    virtual bool get_movedStatus();
    virtual void set_movedStatus(bool movedStatus);
    
private:
    int ID = 1;
    bool hasMoved;
};

// WORLD DEFINITION

class World{
public:
    friend class DoodleBug;
    friend class Ant;
    friend class Organism;
    
    World();   // Constructor
    ~World();   // Destructor
    
    void Draw();  // Output the grid
    int global_get_ID(int x, int y);  // Get Organism ID at given point on grid
    Organism* get_Ptr(int x, int y);  // returns the Organism Pointer at the given location in the grid
    void set_Ptr(int x, int y, Organism* newOrg);  // Sets the organism pointer to an Ant or Doodlebug pointer
    void TimeStepForward();  // Advances time one step forward
    
protected:
    Organism* grid[GRID_SIZE][GRID_SIZE];
};

// MEMBER FUNCTION DEFINITIONS - ORGANISM

Organism :: Organism(int xVal, int yVal, World globe) : posX(xVal), posY(yVal), timeSinceBreeding(0),ID(0), hasMoved(false){
    globe.grid[xVal][yVal] = this;
}
Organism :: ~Organism(){
}
void Organism :: move(World& globe){
}
int Organism :: get_posX(){
    return posX;
}
int Organism :: get_posY(){
    return posY;
}
int Organism :: get_timeSinceBreeding(){
    return timeSinceBreeding;
}
int Organism :: get_ID(){
    return ID;
}
void Organism :: set_timeSinceBreeding(int& timeSinceBreeding){
    if (timeSinceBreeding < 8)
        timeSinceBreeding++;
}
bool Organism::get_movedStatus(){
    return hasMoved;
}
void Organism::set_movedStatus(bool movedStatus){
    this->hasMoved = movedStatus;
}
int Organism::get_timeSinceEating(){
    return 0;
}
// MEMBER FUNCTION DEFINITIONS - DOODLEBUG


DoodleBug :: DoodleBug(int xVal, int yVal, World globe) : Organism(xVal,yVal, globe), timeSinceEating(0){
    globe.set_Ptr(xVal,yVal,this);
}
DoodleBug :: ~DoodleBug(){
}
void DoodleBug::move(World& globe){
    if(get_movedStatus() == false){
        // Check for neighboring Ants to eat.  If so, move to that position.
        if((posY > 0) && (globe.global_get_ID(posX, posY-1) == 1)){
            globe.grid[posX][posY-1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY -= 1;
            timeSinceEating = 0;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posY < GRID_SIZE-1)&&(globe.global_get_ID(posX,posY+1) == 1)){
            globe.grid[posX][posY+1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY += 1;
            timeSinceEating = 0;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX > 0)&&(globe.global_get_ID(posX-1,posY) == 1)){
            globe.grid[posX-1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX -= 1;
            timeSinceEating = 0;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX < GRID_SIZE-1)&&(globe.global_get_ID(posX+1,posY) == 1)){
            globe.grid[posX+1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX += 1;
            timeSinceEating = 0;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
    }
    // If no neighboring prey, then move in a random direction
    if(get_movedStatus() == false){
        int randomDir = rand()%4;
        if((posY>0)&&(randomDir == 0)&&(globe.grid[posX][posY-1] == NULL)){
            globe.grid[posX][posY-1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY -= 1;
            timeSinceEating++;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posY<GRID_SIZE-1)&&(randomDir == 1)&&(globe.grid[posX][posY+1] == NULL)){
            globe.grid[posX][posY+1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY += 1;
            timeSinceEating++;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX>0)&&(randomDir == 2)&&(globe.grid[posX-1][posY] == NULL)){
            globe.grid[posX-1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX -= 1;
            timeSinceEating++;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX<GRID_SIZE-1)&&(randomDir == 3)&&(globe.grid[posX+1][posY] == NULL)){
            globe.grid[posX+1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX += 1;
            timeSinceEating++;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else{
            timeSinceEating++;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
    }
    // If the doodlebug has survived 8 time steps, it will breed and produce an offspring
    if (timeSinceBreeding == 8){
        breed(globe);
        timeSinceBreeding = 0;
    }
    if (timeSinceEating==3){
        starve(globe);
    }
};
void DoodleBug :: breed(World& globe){
    int randomDir = rand()%4;
    if((posY>0)&&(randomDir == 0)&&(globe.grid[posX][posY-1] == NULL)){
        DoodleBug* newDB = new DoodleBug(posX, posY-1,globe);
        newDB->set_movedStatus(true);
        globe.grid[posX][posY-1] = newDB;
        timeSinceBreeding = 0;
    }
    else if((posY<GRID_SIZE-1)&&(randomDir == 1)&&(globe.grid[posX][posY+1] == NULL)){
        DoodleBug* newDB = new DoodleBug(posX, posY+1,globe);
        newDB->set_movedStatus(true);
        globe.grid[posX][posY+1] = newDB;
        timeSinceBreeding = 0;
    }
    else if((posX>0)&&(randomDir == 2)&&(globe.grid[posX-1][posY] == NULL)){
        DoodleBug* newDB = new DoodleBug(posX-1, posY,globe);
        newDB->set_movedStatus(true);
        globe.grid[posX-1][posY] = newDB;
        timeSinceBreeding = 0;
    }
    else if((posX<GRID_SIZE-1)&&(randomDir == 3)&&(globe.grid[posX+1][posY] == NULL)){
        DoodleBug* newDB = new DoodleBug(posX+1, posY,globe);
        newDB->set_movedStatus(true);
        globe.grid[posX+1][posY] = newDB;
        timeSinceBreeding = 0;
    }
};
void DoodleBug :: starve(World& globe){
    globe.grid[posX][posY] = NULL;
};
int DoodleBug :: get_timeSinceEating(){
    return timeSinceEating;
};
int DoodleBug :: get_ID(){
    int ID = 2;
    return ID;
}
void DoodleBug :: set_timeSinceEating(int new_timeSinceEating){
    timeSinceEating = new_timeSinceEating;
};
bool DoodleBug::get_movedStatus(){
    return hasMoved;
}
void DoodleBug::set_movedStatus(bool movedStatus){
    hasMoved = movedStatus;
}
// MEMBER FUNCTION DEFINITIONS - ANT
Ant :: Ant(int xVal, int yVal, World globe) : Organism(xVal,yVal, globe){
    globe.set_Ptr(xVal,yVal,this);
}
Ant :: ~Ant(){
    delete this;
}

void Ant :: move(World& globe){
    if(get_movedStatus() == false){
        int randomDir = rand()%4;
        if((posY>0)&&(randomDir == 0)&&(globe.grid[posX][posY-1] == NULL)){
            globe.grid[posX][posY-1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY -= 1;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posY<GRID_SIZE-1)&&(randomDir == 1)&&(globe.grid[posX][posY+1] == NULL)){
            globe.grid[posX][posY+1] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posY += 1;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX>0)&&(randomDir == 2)&&(globe.grid[posX-1][posY] == NULL)){
            globe.grid[posX-1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX -= 1;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else if((posX<GRID_SIZE-1)&&(randomDir == 3)&&(globe.grid[posX+1][posY] == NULL)){
            globe.grid[posX+1][posY] = globe.grid[posX][posY];
            globe.grid[posX][posY] = NULL;
            posX += 1;
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        else{
            timeSinceBreeding++;
            set_movedStatus(true);
        }
        if (timeSinceBreeding == 3){
            breed(globe);
            timeSinceBreeding = 0;
        }
    }
};

void Ant :: breed(World& globe){
    int randomDir = rand()%4;
    if((posY>0)&&(randomDir == 0)&&(globe.grid[posX][posY-1] == NULL)){
        Ant* newAnt = new Ant(posX, posY-1,globe);
        newAnt->set_movedStatus(true);
        globe.grid[posX][posY-1] = newAnt;
    }
    else if((posY<GRID_SIZE-1)&&(randomDir == 1)&&(globe.grid[posX][posY+1] == NULL)){
        Ant* newAnt = new Ant(posX, posY+1,globe);
        newAnt->set_movedStatus(true);
        globe.grid[posX][posY+1] = newAnt;
    }
    else if((posX>0)&&(randomDir == 2)&&(globe.grid[posX-1][posY] == NULL)){
        Ant* newAnt = new Ant(posX-1, posY,globe);
        newAnt->set_movedStatus(true);
        globe.grid[posX-1][posY] = newAnt;
    }
    else if((posX<GRID_SIZE-1)&&(randomDir == 3)&&(globe.grid[posX+1][posY] == NULL)){
        Ant* newAnt = new Ant(posX+1, posY,globe);
        newAnt->set_movedStatus(true);
        globe.grid[posX+1][posY] = newAnt;
    }
};

int Ant :: get_ID(){
    int ID = 1;
    return ID;
}
bool Ant::get_movedStatus(){
    return hasMoved;
}
void Ant::set_movedStatus(bool movedStatus){
    hasMoved = movedStatus;
}
// MEMBER FUNCTION DEFINITIONS - WORLD

World :: World(){
    // Initialize World Grid
    for(int i=0;i<GRID_SIZE;i++){
        for(int j =0; j<GRID_SIZE; j++){
            grid[i][j] = NULL;
        }}}
World :: ~World(){
    //delete world at the terminaton of program
    //for(int i=0;i<GRID_SIZE;i++){
    //    for(int j =0; j<GRID_SIZE; j++){
    //        delete grid[i][j];
    //delete [][] grid;}}
}
void World :: Draw(){
    for(int i = 0; i<GRID_SIZE;i++){
        for(int j=0; j<GRID_SIZE; j++){
            if (grid[i][j] == NULL){
                cout<<" - ";
            }
            else if (grid[i][j]->get_ID() == 1){
                cout<<" o ";
            }
            else if(grid[i][j]->get_ID() == 2){
                cout<<" X ";
            }
        }
        cout<<endl;
    }
}
int World :: global_get_ID(int x, int y){
    if(grid[x][y] == NULL){
        return 0;
    }
    int tempID = grid[x][y]->get_ID();
    return tempID;
}
Organism* World :: get_Ptr(int x, int y){
    if (grid[x][y] == NULL)
        return NULL;
    else
        return grid[x][y];
}
void World :: set_Ptr(int x, int y, Organism* newOrg){
    grid[x][y] = newOrg;
}
void World :: TimeStepForward(){
    for(int i = 0; i<GRID_SIZE; i++){
        for(int j = 0; j<GRID_SIZE; j++){
            if(grid[i][j] != NULL)
            grid[i][j]->set_movedStatus(false);
        }
    }
    // STEP 1 - MOVE DOODLEBUGS
    for(int i = 0; i<GRID_SIZE; i++){
        for(int j = 0; j<GRID_SIZE; j++){
            if ((grid[i][j] != NULL) && (grid[i][j]->get_ID() == 2)){
                if(grid[i][j]->get_movedStatus()==false){
                grid[i][j]->move(*this);
                }
            }
        }
    }
    // STEP 2 - MOVE ANTS
    for(int i = 0; i< GRID_SIZE; i++){
        for(int j =0; j< GRID_SIZE; j++){
            if ((grid[i][j] != NULL) && (grid[i][j]->get_ID() == 1)){
                if(grid[i][j]->get_movedStatus()==false){
                    grid[i][j]->move(*this);
                }
            }
        }
    }
}

int main(){
    char userInput = '\n';
    World currentWorld;
    int timeStepCount=0;
    
    for(int i=0 ; i<DB_INIT ; i++){
        int dbXInit = rand()%GRID_SIZE;
        int dbYInit = rand()%GRID_SIZE;
        while(currentWorld.get_Ptr(dbXInit,dbYInit) != NULL){
            dbXInit = rand()%GRID_SIZE;
            dbYInit = rand()%GRID_SIZE;
        }
        DoodleBug* newDB = new DoodleBug(dbXInit,dbYInit,currentWorld);
        currentWorld.set_Ptr(dbXInit,dbYInit,newDB);
    }
    
    for(int i=0 ; i<ANT_INIT ; i++){
        int antXInit = rand()%GRID_SIZE;
        int antYInit = rand()%GRID_SIZE;
        while(currentWorld.get_Ptr(antXInit,antYInit) != NULL){
            antXInit = rand()%GRID_SIZE;
            antYInit = rand()%GRID_SIZE;
        }
        Ant* newAnt = new Ant(antXInit,antYInit,currentWorld);
        currentWorld.set_Ptr(antXInit,antYInit,newAnt);
    }
    
    while(userInput == '\n'){
        currentWorld.Draw();
        currentWorld.TimeStepForward();
        cout<<"Press 'ENTER' to step forward in time. (Current step: "<<timeStepCount<<") "<<endl;
        timeStepCount++;
        userInput = cin.get();
    }
    
    cout<<"program terminated.";
    return 0;
    
}
