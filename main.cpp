// JOSE C.S. CURET

#include <iostream>
#include <vector>

using namespace std;

const char ANT = 'o';
const char DOODLEBUG = 'X';
const char EMPTY_SPACE = '-';
const int GRID_HEIGHT = 20;
const int GRID_WIDTH = 20;
const int MINIMUM_GRID = 0;
const int ANT_INIT_SIZE = 100;
const int DOODLEBUG_INIT_SIZE = 5;
const int ANT_STEPS_TO_BREED = 3;
const int DOODLEBUG_STEPS_TO_STARVE = 3;
const int DOODLEBUG_STEPS_TO_BREED = 8;
const char ENTER = '\n';

class Organism{
private:
    vector <vector<char>> crittersWorld;
public:
    Organism();
    void generateInitCritters(char Critter, int amount);
    void printGrid();
    vector <vector<char>> getCrittersWorld(){return this->crittersWorld;}
    void setCrittersWorld(int left, int right, char species){
        crittersWorld[left][right] = species;
    }
    virtual void move(Organism &o){};
    virtual void breed(Organism &o, int x, int y){};
    void randomMove(int x, int y, int &randomXChoice, int &randomYChoice);
};

Organism::Organism(){

    crittersWorld.resize(GRID_HEIGHT, vector<char>(GRID_WIDTH));

    for(auto & i : crittersWorld){
        for(int j = 0; j < crittersWorld.size(); j++){
            i[j] = EMPTY_SPACE;
        }
    }

    generateInitCritters(DOODLEBUG, DOODLEBUG_INIT_SIZE);
    generateInitCritters(ANT, ANT_INIT_SIZE);

}

void Organism::generateInitCritters(char critter, int amount){

    int x, y;

    int critterSpecies = 0;
    while(critterSpecies < amount){

        x = (rand() % GRID_WIDTH);
        y = (rand() % GRID_HEIGHT);
        if(crittersWorld[x][y] == EMPTY_SPACE){
            crittersWorld[x][y] = critter;
            critterSpecies++;
        }


    }
}

void Organism::printGrid(){

    for(auto & i : crittersWorld){
        cout << "\t";
        for(int j = 0; j < crittersWorld.size(); j++){
            cout << i[j] << " ";
        }cout << endl;
    }
}

void Organism::randomMove(int x, int y, int &randomXChoice, int &randomYChoice){

    int randomMove = (rand() % 3) + 1;

    int left = x-1, right = x+1;
    int up = y-1, down = y+1;
    int centerLeft = x, centerRight = y;
    if((left >= MINIMUM_GRID) && (right >= MINIMUM_GRID) && (up >= MINIMUM_GRID) && (down >= MINIMUM_GRID) &&
       (centerLeft >= MINIMUM_GRID) && (centerRight >= MINIMUM_GRID) && (left < GRID_WIDTH) &&
       (right < GRID_WIDTH) && (up < GRID_HEIGHT) && (down < GRID_HEIGHT) && (centerLeft < GRID_WIDTH) &&
       (centerRight < GRID_HEIGHT)){

        switch (randomMove) {
            case 1:{
                randomXChoice = left;
                randomYChoice = centerRight;
            }
                break;
            case 2:{
                randomXChoice = right;
                randomYChoice = centerRight;
            }
                break;
            case 3: {
                randomXChoice = centerLeft;
                switch (((rand() % 2) + 1)) {
                    case 1: randomYChoice = up;
                        break;
                    case 2: randomYChoice = down;
                }
            }

        }
    }

}

class Ant: public Organism{
private:
    int randomXChoice{}, randomYChoice{};
    vector <vector<int>> stepsToBreed;
public:
    Ant(){stepsToBreed.resize(GRID_HEIGHT, vector<int>(GRID_WIDTH));}
    void move(Organism &o) override;
    void breed(Organism &o, int x, int y) override;
    int getStepsToBreed(int x, int y){return this->stepsToBreed[x][y];}
};

void Ant::move(Organism &o){

    for(int i = 0; i < o.getCrittersWorld().size(); i++){
        for(int j = 0; j < o.getCrittersWorld().size(); j++){

            if(o.getCrittersWorld()[i][j] == ANT){



                if((i >= MINIMUM_GRID) && (j < GRID_WIDTH) && (i < GRID_HEIGHT) && (j >= MINIMUM_GRID)){

                    randomMove(i, j, randomXChoice, randomYChoice);

                    if(o.getCrittersWorld()[randomXChoice][randomYChoice] == EMPTY_SPACE){


                        o.setCrittersWorld(randomXChoice, randomYChoice, ANT);
                        o.setCrittersWorld(i, j, EMPTY_SPACE);
                        stepsToBreed[randomXChoice][randomYChoice] = stepsToBreed[i][j] + 1;
                        stepsToBreed[i][j] = 0;
                        breed(o, randomXChoice, randomYChoice);

                    }

                }
            }
        }
    }
}


void Ant::breed(Organism &o, int x, int y){

        if((getStepsToBreed(x, y) >= ANT_STEPS_TO_BREED)){

                randomMove(x, y, randomXChoice, randomYChoice);

                if(o.getCrittersWorld()[randomXChoice][randomYChoice] == EMPTY_SPACE){
                    o.setCrittersWorld(randomXChoice, randomYChoice, ANT);
                    stepsToBreed[x][y] = 0;
                }

        }

}

class Doodlebug: public Organism{
private:
    int randomXChoice{}, randomYChoice{};
    vector <vector<int>> stepsToBreed;
    vector <vector<int>> stepsToStarve;
public:
    Doodlebug(){
        stepsToBreed.resize(GRID_HEIGHT, vector<int>(GRID_WIDTH));
        stepsToStarve.resize(GRID_HEIGHT, vector<int>(GRID_WIDTH));
    }
    void move(Organism &o) override;
    void breed(Organism &o, int x, int y) override;
    void starve(Organism &o, int x, int y);
    int getStepsToBreed(int x, int y){return this->stepsToBreed[x][y];}

};


void Doodlebug::move(Organism &o) {

    stepsToBreed.resize(GRID_HEIGHT, vector<int>(GRID_WIDTH));
    for(int i = 0; i < o.getCrittersWorld().size(); i++){
        for(int j = 0; j < o.getCrittersWorld().size(); j++){

            if(o.getCrittersWorld()[i][j] == DOODLEBUG){

                if(stepsToStarve[i][j] > DOODLEBUG_STEPS_TO_STARVE){
                    starve(o, i, j);

                }

                if((i >= MINIMUM_GRID) && (j < GRID_WIDTH) && (i < GRID_HEIGHT) && (j >= MINIMUM_GRID)){

                    randomMove(i, j, randomXChoice, randomYChoice);

                    if(o.getCrittersWorld()[randomXChoice][randomYChoice] != DOODLEBUG){
                        o.setCrittersWorld(randomXChoice, randomYChoice, DOODLEBUG);
                        o.setCrittersWorld(i, j, EMPTY_SPACE);
                        stepsToBreed[randomXChoice][randomYChoice] = stepsToBreed[i][j] + 1;
                        stepsToStarve[randomXChoice][randomYChoice] = stepsToBreed[i][j] + 1;
                        stepsToBreed[i][j] = 0;
                        stepsToStarve[i][j] = 0;
                        breed(o, randomXChoice, randomYChoice);
                    }

                }

            }
        }
    }

}

void Doodlebug::breed(Organism &o, int x, int y){

    if((getStepsToBreed(x, y) >= DOODLEBUG_STEPS_TO_BREED)){
        randomMove(x, y, randomXChoice, randomYChoice);
            o.setCrittersWorld(randomXChoice, randomYChoice, DOODLEBUG);
            stepsToBreed[x][y] = 0;
    }

}

void Doodlebug::starve(Organism &o, int x, int y){
    o.setCrittersWorld(x, y, EMPTY_SPACE);
    stepsToStarve[x][y] = 0;
}

void messageContinueProgram(Organism &organism){
    organism.printGrid();
    std::cout << std::endl << std::endl << std::endl
    << "Press ENTER to continue or Q to quit" << std::endl;

}

int main() {

    srand(time(nullptr));
    Organism organism;
    Ant * ant = new Ant();
    Doodlebug * doodlebug = new Doodlebug();

    messageContinueProgram(organism);

    int count = 0;
    while(cin.get() == ENTER){

        if(count){
            ant->move(organism);
            doodlebug->move(organism);
            messageContinueProgram(organism);
        }else{
            cout << count << endl;
            doodlebug->move(organism);
        }

        count++;
    }


    delete ant;
    delete doodlebug;

    return 0;
}
