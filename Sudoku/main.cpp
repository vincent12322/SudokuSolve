#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <math.h>
#include <random>
#include <time.h>


using namespace std;

#define MAX 9
#define BLANK 0
#define SPACE " "
#define LINE "|"
#define NEW_ROW "---------------------------------------"
#define SEPARATE_ROW "======================================="
#define FULL make_pair(9, 9)
#define e 2.718281828

int solution[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int puzzle[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

struct coord {
    int x;
    int y;
};

struct node {
    vector<int> domain;
};

struct arc {
    coord one;
    coord two;
};


queue<arc> arcQ;
vector<coord> usedCoords;
node nodeGrid[MAX][MAX];
int minVal = 100;

//Function Prototypes
bool revise(coord one, coord two);
bool is_safe(int grid[MAX][MAX], int row, int col, int num);
bool used_in_box(int grid[MAX][MAX], int box_start_row, int box_start_col, int num);
bool used_in_row(int grid[MAX][MAX], int row, int num);
bool used_in_col(int grid[MAX][MAX], int col, int num);
bool coordEqual(coord one, coord two);
void check_neighbors_row(int grid[MAX][MAX], int row, arc temp);
void check_neighbors_col(int grid[MAX][MAX], int row, arc temp);
void check_neighbors_box(int grid[MAX][MAX], int box_start_row, int box_start_col, arc temp);
void get_arcs_box(int grid[MAX][MAX], int box_start_row, int box_start_col, coord currentCoord);
void get_arcs_row(int grid[MAX][MAX], int row, coord currentCoord);
void get_arcs_col(int grid[MAX][MAX], int row, coord currentCoord);
void setArcs(int grid[MAX][MAX]);
void fillRandom(int grid[MAX][MAX]);
int scoreRandom(int grid[MAX][MAX]);
int get_boxCount(int grid[MAX][MAX], int box_start_row, int box_start_col);
int get_rowCount(int grid[MAX][MAX], int row);
int get_colCount(int grid[MAX][MAX], int col);
double fRandDouble(double fMin, double fMax);
int fRandInt(int fMin, int fMax);

bool AC3() {
    cout << "STARTED!" << endl;
    while (!arcQ.empty()) {
        arc temp = arcQ.front(); arcQ.pop();
        if (revise(temp.one, temp.two)) {
            if (nodeGrid[temp.one.x][temp.one.y].domain.empty()) {
                cout << "!!!!!FAILURE AT " << temp.one.x << temp.two.y << endl;
                return false;
            }
            // Send the box start in to get the neighbors of box

            check_neighbors_row(puzzle, temp.one.x, temp);
            check_neighbors_col(puzzle, temp.one.y, temp);
            check_neighbors_box(puzzle, temp.one.x - temp.one.x % 3, temp.one.y - temp.one.y % 3, temp);
        }
    }
    return true;
}

bool revise(coord one, coord two) {
    bool revised = false;
    bool gotToEnd = true;
    for (int i = 0; i < nodeGrid[one.x][one.y].domain.size(); i++) {
        int num = nodeGrid[one.x][one.y].domain[i];
        if (!is_safe(puzzle, one.x, one.y, num)) {
            nodeGrid[one.x][one.y].domain.erase(nodeGrid[one.x][one.y].domain.begin() + i);
            cout << "REVISED ARC" << endl;
            continue;
        }
        puzzle[one.x][one.y] = num;
        for (int j = 0; j < nodeGrid[two.x][two.y].domain.size(); j++) {
            int num2 = nodeGrid[two.x][two.y].domain[j];
            if(is_safe(puzzle, two.x, two.y, num2)) {
                gotToEnd = false;
                break;
            } else if( j ==  nodeGrid[two.x][two.y].domain.size() - 1) {
                gotToEnd = true;
                int temp = nodeGrid[two.x][two.y].domain[i];
                nodeGrid[one.x][one.y].domain.erase(nodeGrid[one.x][one.y].domain.begin() + i);
                cout << "REVISED ARC:" << one.x << one.y << "-" << two.x << two.y << " OF " << temp << endl;
                revised = true;
                break;
            }
        }
        puzzle[one.x][one.y] = BLANK;
    }
    return revised;
}

void setArcs(int grid[MAX][MAX]) {
    coord currentCoord;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (grid[i][j] == BLANK) {
                currentCoord = { i , j };
                get_arcs_col(puzzle, j, currentCoord);
                get_arcs_row(puzzle, i, currentCoord);
                get_arcs_box(puzzle, i - i % 3, j - j % 3, currentCoord);
            }
        }
    }
    /*
    while (!arcQ.empty()) {
        arc arc;
        arc = arcQ.front(); arcQ.pop();
        cout << "CURRENT ARC: " << arc.one.x << arc.one.y << "-" << arc.two.x << arc.two.y << endl;
    }
    */
}


void print_grid(int grid[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        cout << SPACE << SPACE << SPACE << SPACE << endl;
       
        if (i % 3 == 0 && i != 0) {
            cout << SEPARATE_ROW << endl;
        } else {
             cout << NEW_ROW << endl;
        }
        for (int j = 0; j < MAX; j++) {
            if (j % 3 == 0) {
                cout << LINE;
            }
            cout << SPACE;
            if (BLANK == grid[i][j]) {
                cout << SPACE;
            }
            else {
                cout << grid[i][j];
            }
            cout << SPACE;
            cout << LINE;
        }
    }
    cout << endl << NEW_ROW << endl << endl;;
}

void get_arcs_row(int grid[MAX][MAX], int row, coord currentCoord) {
    coord one = currentCoord;
    for (int col = 0; col < MAX; col++) {
        coord temp = { row, col };
        //If it isn't ourself and it's an empty spot, add it to the set of arcs
        if (grid[row][col] == BLANK && !coordEqual(temp, one)) {
            arc tempArc = { one, temp };
            arcQ.push(tempArc);
        }
    }
}

void get_arcs_col(int grid[MAX][MAX], int col, coord currentCoord) {
    coord one = currentCoord;
    //not self or two
    for (int row = 0; row < MAX; row++) {
        coord temp = { row, col };
        if (grid[row][col] == BLANK && !coordEqual(temp, one)) {
            arc tempArc = { one, temp };
            arcQ.push(tempArc);
        }
    }
}

void get_arcs_box(int grid[MAX][MAX], int box_start_row, int box_start_col, coord currentCoord) {
    coord one = currentCoord;
    //not self or two
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            coord temp = { row + box_start_row, col + box_start_col };
            if (grid[row + box_start_row][col + box_start_col] == BLANK && !coordEqual(temp, one)) {
                arc tempArc = { one, temp };
                arcQ.push(tempArc);
            }
        }
    }
}

void check_neighbors_col(int grid[MAX][MAX], int col, arc temp) {
    coord one = temp.one;
    coord two = temp.two;
    //not self or two
    for (int row = 0; row < MAX; row++) {
        coord temp = { row, col };
        if (grid[row][col] == BLANK && !coordEqual(temp, one) && !coordEqual(temp, two)) {
            arc tempArc = { temp, one };
            arcQ.push(tempArc);
        }
    }
}

void check_neighbors_row(int grid[MAX][MAX], int row, arc temp) {
    coord one = temp.one;
    coord two = temp.two;
    //not self or two
    for (int col = 0; col < MAX; col++) {
        coord temp = { row, col };
        if (grid[row][col] == BLANK && !coordEqual(temp, one) && !coordEqual(temp, two)) {
            arc tempArc = { temp, one };
            arcQ.push(tempArc);
        }
    }
}


void check_neighbors_box(int grid[MAX][MAX], int box_start_row, int box_start_col, arc temp) {
    coord one = temp.one;
    coord two = temp.two;
    //not self or two
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            coord temp = { row + box_start_row, col + box_start_col };
            if (grid[row + box_start_row][col + box_start_col] == BLANK && !coordEqual(temp, one) && !coordEqual(temp, two)) {
                arc tempArc = { temp, one };
                arcQ.push(tempArc);
            }
        }
    }
}

bool coordEqual(coord one, coord two) {
    if (one.x == two.x && one.y == two.y) {
        return true;
    } else
        return false;
}

bool used_in_row(int grid[MAX][MAX], int row, int num) {
    for (int col = 0; col < MAX; col++)
        if (grid[row][col] == num) {
            return true;
        }
    return false;
}

bool used_in_col(int grid[MAX][MAX], int col, int num) {
    for (int row = 0; row < MAX; row++)
        if (grid[row][col] == num) {
            return true;
        }
    return false;
}

bool used_in_box(int grid[MAX][MAX], int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + box_start_row][col + box_start_col] == num) {
                return true;
            }
    return false;
}

bool is_safe(int grid[MAX][MAX], int row, int col, int num) {
    return !used_in_row(grid, row, num) &&
    !used_in_col(grid, col, num) &&
    !used_in_box(grid, row - row % 3, col - col % 3, num);
}

pair<int, int> get_unassigned_location(int grid[MAX][MAX]) {
    for (int row = 0; row < MAX; row++)
        for (int col = 0; col < MAX; col++)
            if (grid[row][col] == BLANK) {
                return make_pair(row, col);
            }
    return FULL;
}

pair<int, int> get_unassigned_location_MRV(int grid[MAX][MAX]) {
    pair<int, int> bestPair = FULL;
    pair<int, int> tempPair;
    for (int row = 0; row < MAX; row++) {
        for (int col = 0; col < MAX; col++) {
            if (nodeGrid[row][col].domain.size() < minVal && grid[row][col] == BLANK) {
                minVal = (int)nodeGrid[row][col].domain.size();
                bestPair = make_pair(row, col);
                cout << "MRV OF " << bestPair.first << bestPair.second << " : " << minVal << endl;
            } else if(grid[row][col] == BLANK) {
                tempPair = make_pair(row, col);
            }
        }
    }
    if (bestPair == FULL) {
        return tempPair;
    }
    return bestPair;
}

void readFile(int grid[MAX][MAX]) {
    ifstream file;
    string fileName;
    cout << "Input puzzle file name: " << endl;
    cin >> fileName;
    file.open(fileName);
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            file >> grid[i][j];
        }
    }
    file.close();
    cout << "File Read Complete." << endl;
}


bool solve_soduko_backtrack(int grid[MAX][MAX]) {
    if (get_unassigned_location(grid) == FULL) {
        return true;
    }

    pair<int, int> row_and_col = get_unassigned_location(grid);
    int row = row_and_col.first;
    int col = row_and_col.second;

    for (int num = 1; num <= 9; num++) {
        if (is_safe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solve_soduko_backtrack(grid)) {
                return true;
            }
            grid[row][col] = BLANK;
        }
    }
    return false;
}

void setDomains(int grid[MAX][MAX], node nodeGrid[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (grid[i][j] == 0) {
                for (int l = 1; l < 10; l++) {
                    nodeGrid[i][j].domain.push_back(l);
                }
            } else {
                nodeGrid[i][j].domain.push_back(grid[i][j]);
            }
        }
    }
}

void printDomains(int grid[MAX][MAX], node nodeGrid[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (grid[i][j] == 0) {
                cout << "DOMAIN FOR:" << i << j << " is ";
                for (int l = 0; l < nodeGrid[i][j].domain.size(); l++) {
                   cout << nodeGrid[i][j].domain[l] << "-";
                }
                cout << endl;
            }
        }
    }
}

void FC(int grid[MAX][MAX], node nodeGrid[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int l = 0; l < nodeGrid[i][j].domain.size(); l++) {
                if (!is_safe(grid, i, j, nodeGrid[i][j].domain[l]) && !nodeGrid[i][j].domain.empty()) {
                    cout << "Forward checked: " << i << j << ": " << nodeGrid[i][j].domain[l] << endl;
                    nodeGrid[i][j].domain.erase(nodeGrid[i][j].domain.begin() + l);
                }
            }
        }
    }
}

void fillRandom(int grid[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (grid[i][j] == BLANK) {
                int num = fRandInt(1, 9);
                coord temp = { i, j };
                grid[i][j] = num;
                usedCoords.push_back(temp);
            }
        }
    }
}

void emptyRandom(int grid[MAX][MAX]) {
    while (!usedCoords.empty()) {
        coord temp = usedCoords.back();
        grid[temp.x][temp.y] = BLANK;
        usedCoords.pop_back();
    }
}

int scoreRandom(int grid[MAX][MAX]) {
    double score = 0;
    for (int row = 0; row < MAX; row++)
        for (int col = 0; col < MAX; col++) {
            score += get_boxCount(grid, row - row % 3, col - col % 3);
            score += get_rowCount(grid, row);
            score += get_colCount(grid, col);
        }
    return score;
}

int get_rowCount(int grid[MAX][MAX], int row) {
    int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0, eight = 0, nine = 0;
    int sum = 0;
    for (int col = 0; col < MAX; col++) {
        switch (grid[row][col]) {
            case 1:
                one++; break;
            case 2:
                two++; break;
            case 3:
                three++; break;
            case 4:
                four++; break;
            case 5:
                five++; break;
            case 6:
                six++; break;
            case 7:
                seven++; break;
            case 8:
                eight++; break;
            case 9:
                nine++; break;
            case BLANK:
                break;
            default:
                break;
        }
    }
    if(one > 1)
        sum++;
    if(two > 1)
        sum++;
    if(three > 1)
        sum++;
    if(four > 1)
        sum++;
    if(five > 1)
        sum++;
    if(six > 1)
        sum++;
    if(seven > 1)
        sum++;
    if(eight > 1)
        sum++;
    if(nine > 1)
        sum++;
    return sum;
}

int get_colCount(int grid[MAX][MAX], int col) {
    int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0, eight = 0, nine = 0;
    int sum = 0;
    for (int row = 0; row < MAX; row++) {
            switch (grid[row][col]) {
                case 1:
                    one++; break;
                case 2:
                    two++; break;
                case 3:
                    three++; break;
                case 4:
                    four++; break;
                case 5:
                    five++; break;
                case 6:
                    six++; break;
                case 7:
                    seven++; break;
                case 8:
                    eight++; break;
                case 9:
                    nine++; break;
                case BLANK:
                    break;
                default:
                    break;
            }
    }
    if(one > 1)
        sum++;
    if(two > 1)
        sum++;
    if(three > 1)
        sum++;
    if(four > 1)
        sum++;
    if(five > 1)
        sum++;
    if(six > 1)
        sum++;
    if(seven > 1)
        sum++;
    if(eight > 1)
        sum++;
    if(nine > 1)
        sum++;
    return sum;
}

int get_boxCount(int grid[MAX][MAX], int box_start_row, int box_start_col) {
    int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0, eight = 0, nine = 0;
    int sum = 0;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                switch (grid[row + box_start_row][col + box_start_col]) {
                    case 1:
                        one++; break;
                    case 2:
                        two++; break;
                    case 3:
                        three++; break;
                    case 4:
                        four++; break;
                    case 5:
                        five++; break;
                    case 6:
                        six++; break;
                    case 7:
                        seven++; break;
                    case 8:
                        eight++; break;
                    case 9:
                        nine++; break;
                    case BLANK:
                        break;
                    default:
                        break;
                }
            }
        }
    if(one > 1)
        sum++;
    if(two > 1)
        sum++;
    if(three > 1)
        sum++;
    if(four > 1)
        sum++;
    if(five > 1)
        sum++;
    if(six > 1)
        sum++;
    if(seven > 1)
        sum++;
    if(eight > 1)
        sum++;
    if(nine > 1)
        sum++;
    return sum;
}

void setNode(int node[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            node[i][j] = puzzle[i][j];
        }
    }
}

void setPuzzle(int node[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            puzzle[i][j] = node[i][j];
        }
    }
}

void changeOneRandom(int node[MAX][MAX]) {
    coord temp = usedCoords[fRandInt(0, (int)usedCoords.size() - 1)];
    node[temp.x][temp.y] = fRandInt(1, 9);
    //cout << "CHANGED" << temp.x << "-" << temp.y << " to" << node[temp.x][temp.y] << endl;
}

int fRandInt(int fMin, int fMax) {
    return (int)(fMin + (rand() % ((int)fMax - fMin + 1)));
}

unsigned long fRandFloat() {
    return (rand() / RAND_MAX);
}

float timeSchedule(int t, float T) {
    //cout << "Temp is T=" << T * pow(.999999, t) << endl << endl;
    return T * pow(.99999, t);
}

bool simulated_annealing(int grid[MAX][MAX]) {
    int node[MAX][MAX];
    float T = 2500;
    fillRandom(puzzle);
    setNode(node);
    int current = scoreRandom(puzzle);
    int next = 0;
    
    for (int t = 1;; t++) {
        T = timeSchedule(t, T);
        //RANDOM RESTART
        if (T == 0 && current > 20) {
            //cout << "RANDOM RESTART" << endl << endl << endl;
            emptyRandom(puzzle);
            srand(time(NULL));
            fillRandom(puzzle);
            setNode(node);
            T = 1;
            t = 1;
            current = scoreRandom(puzzle);
        } else if(current < 20) {
            print_grid(puzzle);
            return true;
        }
         
        //print_grid(puzzle);
        changeOneRandom(node);
        next = scoreRandom(node);
        cout << next << endl;
        int E = next - current;
        double prob = exp(-(E)/T);

        if (E < 0) {
            current = next;
            setPuzzle(node);
        } else if ((rand() / (double)RAND_MAX) < prob) {
            current = next;
            setPuzzle(node);
        }
        setNode(node);
    }
    return false;
}


bool solve_soduko(int grid[MAX][MAX], node nodeGrid[MAX][MAX]) {
    if (get_unassigned_location(grid) == FULL) {
        return true;
    }
    
    //pair<int, int> row_and_col = get_unassigned_location_MRV(grid);
    pair<int, int> row_and_col = get_unassigned_location(grid);
    int row = row_and_col.first;
    int col = row_and_col.second;
    
    for (int i = 0; i < nodeGrid[row][col].domain.size(); i++) {
        if (is_safe(grid, row, col, nodeGrid[row][col].domain[i])) {
            //cout << nodeGrid[row][col].domain[i] << "is safe at " << row << " " << col << endl;
            grid[row][col] = nodeGrid[row][col].domain[i];
            if (solve_soduko(grid, nodeGrid)) {
                return true;
            }
            grid[row][col] = BLANK;
            //cout << "backtrack" << endl;
        }
    }
    return false;
}

int main() {
    cout << "********************************\n\n\t\tSudoku Solver\n\n********************************" << endl << endl;
    //srand(time(NULL));
    readFile(puzzle);
    print_grid(puzzle);
    //simulated_annealing(puzzle);
    //fillRandom(puzzle);
    print_grid(puzzle);
    cout << endl << scoreRandom(puzzle) << "SCORE" << endl;
    setDomains(puzzle, nodeGrid);
    clock_t begin = clock(); clock_t end;
    setArcs(puzzle);
    FC(puzzle, nodeGrid);
    AC3();
    //printDomains(puzzle, nodeGrid);
    
    bool solve = false;
    
    solve = solve_soduko(puzzle, nodeGrid);
    cout << "the score is : " << scoreRandom(puzzle) << endl;
    if (solve) {
        end = clock();
        print_grid(puzzle);
    } else {
        end = clock();
        cout << "NO SOLUTION!" << endl << endl;
    }
    
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "TIME: " << elapsed_secs << " seconds." << endl << endl;
    
    return 0;
}
//AC3 FC BACKTRACKING MEDIUM PUZZLE TIME: 0.004013 seconds
//NORMAL BACKTRACKING MEDIUM PUZZLE TIME: 0.075212 seconds
//TIME: 0.000262 seconds.
//TIME: 0.00061 seconds.
//TIME: 0.026019 seconds.
//TIME: 0.004775 seconds.
//TIME: 0.004839 seconds.
//TIME: 0.005789 seconds.
//TIME: 0.002215 seconds.
