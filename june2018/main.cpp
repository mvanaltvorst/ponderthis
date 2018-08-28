#include <iostream>
#include <stack>
#include <unordered_set>
#include <array>

#define SIZE 6


int pieces[2][4][3][3] = {
    {
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 0, 1}
        },
        {
            {0, 1, 0},
            {0, 1, 0},
            {1, 1, 0}
        },
        {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 1, 1},
            {0, 1, 0},
            {0, 1, 0}
        }
    },
    {
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 1}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {1, 0, 0}
        },
        {
            {1, 1, 0},
            {0, 1, 0},
            {0, 1, 0}
        }
    }
};


bool insideGrid(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

struct Placement {
    int x, y, rotation, type;
};


class Board {
    private:
        bool matrix[SIZE][SIZE];
    public:
        int piecesPlaced;
        void doPlacement(Placement p) {
            piecesPlaced++;
            int rx, ry;
            for (int px = 0; px < 3; px++) {
                for (int py = 0; py < 3; py++) {
                    rx = p.x - 1 + px;
                    ry = p.y - 1 + py;

                    if (pieces[p.type][p.rotation][py][px]) matrix[ry][rx] = true;
                }
            }
        }
        void undoPlacement(Placement p) {
            piecesPlaced--;
            int rx, ry;
            for (int px = 0; px < 3; px++) {
                for (int py = 0; py < 3; py++) {
                    rx = p.x - 1 + px;
                    ry = p.y - 1 + py;

                    if (pieces[p.type][p.rotation][py][px]) matrix[ry][rx] = false;
                }
            }
        }
        bool checkPlacement(Placement p) const {
            int rx, ry;
            for (int px = 0; px < 3; px++) {
                for (int py = 0; py < 3; py++) {
                    rx = p.x - 1 + px;
                    ry = p.y - 1 + py;

                    if (!insideGrid(rx, ry) && pieces[p.type][p.rotation][py][px]) return false;
                    if (matrix[ry][rx] && pieces[p.type][p.rotation][py][px]) return false;
                }
            }
            return true;
        }
        bool checkPlacementFullyOccupied(Placement p) const {
            int rx, ry;
            for (int px = 0; px < 3; px++) {
                for (int py = 0; py < 3; py++) {
                    rx = p.x - 1 + px;
                    ry = p.y - 1 + py;

                    if (!insideGrid(rx, ry) && pieces[p.type][p.rotation][py][px]) return false;
                    if (!matrix[ry][rx] && pieces[p.type][p.rotation][py][px]) return false;
                }
            }
            return true;
        }
        void print() {
            for (int y = 0; y < SIZE; y++) {
                for (int x = 0; x < SIZE; x++) {
                    if (matrix[y][x]) {
                        std::cout << "#";
                    } else {
                        std::cout << "*";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        bool isSolved() {
            return (piecesPlaced == 5 && !isSymmetrical() && isFillableWithLs());
        }
        bool isFillableWithLs() {
            if (piecesPlaced == 0) return true;
            Placement p;

            for (int x = 0; x < SIZE; x++) {
                for (int y = 0; y < SIZE; y++) {
                    for (int rot = 0; rot < 4; rot++) {
                        p = Placement{x, y, rot, 1};
                        if (checkPlacementFullyOccupied(p)) {
                            undoPlacement(p);
                            if (isFillableWithLs()) {
                                doPlacement(p);
                                return true;
                            }
                            doPlacement(p);
                        }
                    }
                }
            }
            return false;
        }
        bool isSymmetrical() {
            for (int x = 0; x < SIZE; x++) {
                for (int y = 0; y < SIZE; y++) {
                    if (x==y) continue;
                    if (matrix[y][x] != matrix[x][y]) return false;
                }
            }
            return true;
        }
};

void solve(Board * b) {
    if (b->isSolved()) {
        b->print();
    }
    if (b->piecesPlaced >= 5) return;
    Placement p;

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            for (int rot = 0; rot < 4; rot++) {
                p = Placement{x, y, rot, 0};
                // std::cout << x << " " << y << " " << rot <<std::endl;
                if (b->checkPlacement(p)) {
                    b->doPlacement(p); 
                    solve(b);
                    b->undoPlacement(p);
                }
            }
        }
    }
    return;
}

int main() {
    Board * b = new Board();
    solve(b);
    return 0;
}
