#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

enum ConsoleColor
{
    BLACK = 0,
    DARKBLUE = 1,
    DARKGREEN = 2,
    DARKCYAN = 3,
    DARKRED = 4,
    DARKMAGENTA = 5,
    DARKYELLOW = 6,
    GREY = 7,
    GRAY = 7,
    DARKGREY = 8,
    DARKGRAY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    PINK = 13,
    YELLOW = 14,
    WHITE = 15
};

enum ConsoleKeys
{
    LEFT = 75,
    RIGHT = 77,
    UP = 72,
    DOWN = 80,
    ENTER = 13,
    RETURN = 13,
    BACKSPACE = 8,
    SPACE = 32,
    ESCAPE = 27
};

class Application {
private:
    HANDLE h_out;
    HANDLE h_in;
    HWND hwnd;
    HDC hdc;
    unsigned int console_window_width;
    unsigned int console_window_height;

public:
    Application() {
        h_out = GetStdHandle(STD_OUTPUT_HANDLE);
        h_in = GetStdHandle(STD_INPUT_HANDLE);
        hwnd = GetConsoleWindow();
        hdc = GetDC(hwnd);
        SetConsoleWindowWidth(120 * 8);
        SetConsoleWindowHeight(30 * 12);
    }

    unsigned int GetConsoleWindowWidth() const {
        return console_window_width;
    }

    unsigned int GetConsoleWindowHeight() const {
        return console_window_height;
    }

    void SetConsoleWindowWidth(unsigned int width) {
        console_window_width = width;
    }

    void SetConsoleWindowHeight(unsigned int height) {
        console_window_height = height;
    }

    void SetTitle(const string& title) {
        string command = "title " + title;
        system(command.c_str());
    }

    void SetConsoleWindowColumnsAndRows(unsigned int columns, unsigned int rows)
    {
        string width = to_string(columns);
        string height = to_string(rows);
        string command = "mode con cols=" + width + " lines=" + height;
        system(command.c_str());

        COORD bufferSize;
        bufferSize.X = columns; 
        bufferSize.Y = 1001; 
        SetConsoleScreenBufferSize(h_out, bufferSize);
    }

    void ClearScreen() {
        system("cls");
    }

    void Exit() {
        ExitProcess(0);
    }

    void ShowCursor(bool visible) {
        CONSOLE_CURSOR_INFO cci;
        cci.dwSize = sizeof(cci);
        cci.bVisible = visible;
        SetConsoleCursorInfo(h_out, &cci);
    }

    void SetColor(ConsoleColor foreground, ConsoleColor background) {
        SetConsoleTextAttribute(h_out, ((background << 4) | foreground));
    }

    void GotoXY(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(h_out, coord);
    }

    void WriteText(int x, int y, const string& text) {
        GotoXY(x, y);
        cout << text;
    }
};

class Menu {
private:
    Application app;
    int menu_option;
    int difficulty_option;
    int key;
    int dif_key;
public:

    Menu() {
        setMenuOption(1);
        setDifficultyOption(1);
    }

    void setMenuOption(int menu_option) {
        this->menu_option = menu_option;
    }

    void setDifficultyOption(int difficulty_option) {
        this->difficulty_option = difficulty_option;
    }
    void showMenu() {
        app.ClearScreen();
        app.WriteText(40, 10, "====Welcome to Sudoku====");
        app.GotoXY(47, 12);
        cout << (menu_option == 1 ? ">" : " ") << "1. Play\n";
        app.GotoXY(47, 13);
        cout << (menu_option == 2 ? ">" : " ") << "2. How to play\n";
        app.GotoXY(47, 14);
        cout << (menu_option == 3 ? ">" : " ") << "3. Exit\n";
    }

    void showDifficulty() {
        app.ClearScreen();
        app.WriteText(44, 10, "Choose Difficulty");
        app.GotoXY(47, 12);
        cout << (difficulty_option == 1 ? ">" : " ") << "Easy\n";
        app.GotoXY(47, 13);
        cout << (difficulty_option == 2 ? ">" : " ") << "Medium\n";
        app.GotoXY(47, 14);
        cout << (difficulty_option == 3 ? ">" : " ") << "Hard\n";
    }

    void explainRules() {
        cout << "How to Play Sudoku:\n";
        cout << "1. Goal: Fill a 9x9 grid with numbers so that each row, column, and 3x3 subgrid contains all the digits from 1 to 9.\n";
        cout << "2. Starting Grid: Sudoku starts with some cells already filled in with numbers. These numbers cannot be changed.\n";
        cout << "3. Empty Cells: The rest of the cells are empty, and it's your job to fill them in with numbers.\n";
        cout << "4. Unique Numbers: Each row, column, and 3x3 subgrid must contain the numbers 1 through 9 without repeating any number.\n";
        cout << "5. Filling the Grid: Solve the puzzle by figuring out which numbers go in the empty cells using logic and the process of elimination.\n";
        cout << "6. Logic and Deduction: Look for missing numbers in each row, column, and subgrid. Use existing numbers to help determine the missing ones.\n";
        cout << "7. Process of Elimination: If a number can't go in a certain cell because it's already in the same row, column, or subgrid, eliminate it as a possibility.\n";
        cout << "8. Trial and Error: Sometimes you may need to try different numbers in a cell to see which one works. If a number doesn't work, erase it and try another.\n";
    }

    int chooseDifficulty() {
        showDifficulty();
        dif_key = _getch();
        while (dif_key != ESCAPE) {
            if (dif_key == 224) {
                dif_key = _getch();
                if (dif_key == 72 and difficulty_option > 1) {
                    difficulty_option--;
                }
                else if (dif_key == 80 and difficulty_option < 3) {
                    difficulty_option++;
                }
            }
            if (dif_key == ENTER) {
                return difficulty_option;
            }
            showDifficulty();
            dif_key = _getch();
        }
        difficulty_option = 1;
        return -1;
    }

    int chooseOptions() {
        while (true) {
            showMenu();
            key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == 72 and menu_option > 1) {
                    menu_option--;
                }
                else if (key == 80 and menu_option < 3) {
                    menu_option++;
                }
            }
            else if (key == 13) {
                if (menu_option == 1) {
                    return 1;
                }
                else if (menu_option == 2) {
                    while (key != ESCAPE) {
                        app.ClearScreen();
                        explainRules();
                        key = _getch();
                    }
                }
                else if (menu_option == 3) {
                    app.Exit();
                }
            }
        }
        return -1;
    }

    int getDifficulty() {
        return difficulty_option;
    }
};

class Sudoku {
private:
    Application app;
    Menu menu;
    const int SIZE = 9;
    const int EMPTY_VALUE = 0;
    int startX;
    int startY;
    vector<vector<int>> board;

    bool isValid(int row, int col, int num) const {
        for (int i = 0; i < SIZE; i++) {
            if (board[row][i] == num || board[i][col] == num) {
                return false;
            }
        }

        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = startRow; i < startRow + 3; i++) {
            for (int j = startCol; j < startCol + 3; j++) {
                if (board[i][j] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    bool fillBoard(int row, int col) {
        if (row == SIZE) {
            return true; 
        }

        int nextRow = col == SIZE - 1 ? row + 1 : row;
        int nextCol = (col + 1) % SIZE;

        vector<int> nums(SIZE);
        for (int i = 0; i < SIZE; ++i) {
            nums[i] = i + 1;
        }
        random_shuffle(nums.begin(), nums.end());

        for (int i = 0; i < SIZE; ++i) {
            int num = nums[i];
            if (isValid(row, col, num)) {
                board[row][col] = num;
                if (fillBoard(nextRow, nextCol)) {
                    return true;
                }
                board[row][col] = EMPTY_VALUE; 
            }
        }

        return false;
    }
    void modifyBoard(double percent) {
        int totalCells = SIZE * SIZE;
        int cellsToModify = percent * totalCells;

        while (cellsToModify > 0) {
            int row = rand() % SIZE;
            int col = rand() % SIZE;

            if (board[row][col] != EMPTY_VALUE) {
                board[row][col] = 0;
                cellsToModify--;
            }
        }
    }
public:
    Sudoku() {
        setBoard(vector<vector<int>>(SIZE, vector<int>(SIZE, EMPTY_VALUE)));
        setStartX(40);
        setStartY(4);
    }

    bool isBoardValid() const {
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (!isValid(row, col, board[row][col])) {
                    return false;
                }
            }
        }
        return true;
    }

    void generateBoard(Menu& menu) {
        fillBoard(0, 0);
        int difficulty = menu.getDifficulty();
        if (difficulty == 1) {
            modifyBoard(0.1);
        }
        else if (difficulty == 2) {
            modifyBoard(0.6);
        }
        else if (difficulty == 3) {
            modifyBoard(0.7);
        }
    }

    void printPuzzle() {

        setStartX(40);
        setStartY(4);
        app.GotoXY(startX, startY);

        cout << char(201); // ╔
        for (int i = 0; i < SIZE * 3 - 1; i++) {
            if ((i + 1) % 3 == 0) {
                cout << char(203); // ╦
            }
            else {
                cout << char(205); // ═
            }
        }
        cout << char(187); // ╗

        for (int i = 0; i < SIZE; i++) {
            startY++;
            app.GotoXY(startX, startY);
            cout << char(186); // ║
            for (int j = 0; j < SIZE; j++) {
                cout << " " << board[i][j] << char(179);
            }

            if ((i + 1) % 3 == 0 && i != SIZE - 1) {
                app.GotoXY(startX, ++startY);
                cout << char(204); // ╠
                for (int k = 0; k < SIZE * 3 - 1; k++) {
                    if ((k + 1) % 3 == 0) {
                        cout << char(206); // ╬
                    }
                    else {
                        cout << char(205); // ═
                    }
                }
                cout << char(185); // ╣
            }
            else if (i != SIZE - 1) {
                app.GotoXY(startX, ++startY);
                cout << char(199); // │
                for (int k = 0; k < SIZE * 3 - 1; k++) {
                    if ((k + 1) % 3 == 0) {
                        cout << char(197); // ┼
                    }
                    else {
                        cout << char(196); // ─
                    }
                }
                cout << char(186); // │
            }
        }

        app.GotoXY(startX, startY + 1);
        cout << char(200); // ╚
        for (int i = 0; i < SIZE * 3 - 1; i++) {
            if ((i + 1) % 3 == 0) {
                cout << char(202); // ╩
            }
            else {
                cout << char(205); // ═
            }
        }
        cout << char(188); // ╝
    }

    void setStartX(int startX) {
        this->startX = startX;
    }

    void setStartY(int startY) {
        this->startY = startY;
    }

    int getStartX() const {
        return startX;
    }

    int getStartY() const {
        return startY;
    }

    const vector<vector<int>>& getBoard() const {
        return board;
    }

    void setBoard(const vector<vector<int>>& newBoard) {
        board = newBoard;
    }

    void setCellValue(int row, int col, int value) {
        board[row][col] = value;
    }

};


class Gameplay {
private:
    Application app;
    Sudoku sudoku;
    Menu menu;
    int cursorRow;
    int cursorCol;
    int errorCount;
public:
    Gameplay() {
        setRow(0);
        setCol(0);
    }

    void setError(int errorCount) {
        this->errorCount = errorCount;
    }

    int getError() {
        return errorCount;
    }
    void setRow(int cursorRow) {
        this->cursorRow = cursorRow;
    }

    void setCol(int cursorCol) {
        this->cursorCol = cursorCol;
    }

    void startGame() {
        updateCursor();

        while (true) {
            int key_game = _getch();

            if (key_game == 224) {
                key_game = _getch();
                switch (key_game) {
                case UP:
                    moveCursor(-1, 0);
                    break;
                case DOWN:
                    moveCursor(1, 0);
                    break;
                case LEFT:
                    moveCursor(0, -1);
                    break;
                case RIGHT:
                    moveCursor(0, 1);
                    break;
                }
            }
            else if (key_game == ESCAPE) {
                break;
            }
            else if (key_game == ENTER) {
                inputCellValue();
            }
        }
    }

    void moveCursor(int dRow, int dCol) {
        int newRow = cursorRow + dRow;
        int newCol = cursorCol + dCol;

        if (newRow >= 0 && newRow < 9 && newCol >= 0 && newCol < 9) {
            cursorRow = newRow;
            cursorCol = newCol;
            updateCursor();
        }
    }

    void updateCursor() {
        int screenX = 40 + cursorCol * 3 + 2;
        int screenY = 4 + cursorRow * 2 + 1;

        app.GotoXY(screenX, screenY);
        app.ShowCursor(true);
    }

    void inputCellValue() {
        int currentValue = sudoku.getBoard()[cursorRow][cursorCol];
        if (currentValue == 0) {
            int value;
            cin >> value;
            if (value >= 1 && value <= 9) {
                int screenX = 40 + cursorCol * 3 + 2;
                int screenY = 4 + cursorRow * 2 + 1;

                app.GotoXY(screenX, screenY);


                if (!isValidInput(value, cursorRow, cursorCol, sudoku.getBoard())) {
                    errorCount++;
                    app.WriteText(40, 3, "mistakes: " + to_string(errorCount));

                    sudoku.setCellValue(cursorRow, cursorCol, 0);
                }
                else {
                    sudoku.setCellValue(cursorRow, cursorCol, value);
                }
            }
        }
        checkGameResult();
    }

    bool isValidInput(int value, int row, int col, const vector<vector<int>>& board) {
        if (!(value >= 1 && value <= 9)) {
            return false;
        }

        for (int i = 0; i < 9; ++i) {
            if (board[row][i] == value && i != col) {
                return false;
            }
        }

        for (int i = 0; i < 9; ++i) {
            if (board[i][col] == value && i != row) {
                return false;
            }
        }

        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = startRow; i < startRow + 3; ++i) {
            for (int j = startCol; j < startCol + 3; ++j) {
                if (board[i][j] == value && (i != row || j != col)) {
                    return false;
                }
            }
        }

        return true;
    }
    void checkGameResult() {
        if (getError() >= 3) {
            MessageBoxA(NULL, "You lose", "Lose", MB_OK | MB_ICONERROR);
            errorCount = 0;
        }
        else {
            const vector<vector<int>>& board = sudoku.getBoard();
            bool isEmptyCells = false;

            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 9; ++j) {
                    if (board[i][j] == 0) {
                        isEmptyCells = true;
                        break;
                    }
                }
                if (isEmptyCells) {
                    break;
                }
            }

            if (!isEmptyCells) {
                MessageBoxA(NULL, "You win", "Win", MB_OK | MB_ICONINFORMATION);
            }
        }
    }
};


int main() {
    srand(time(NULL));
    Application app;
    Menu menu;
    Sudoku sudoku;
    Gameplay game;
    app.SetTitle("Sudoku");
    app.SetConsoleWindowColumnsAndRows(120, 30);
    app.ShowCursor(false);
    app.SetColor(BLACK, GREY);
    app.ClearScreen();
    int selected_option;
    int selected_difficulty;

    while (true) {
        selected_option = menu.chooseOptions();
        if (selected_option == 1) {
            selected_difficulty = menu.chooseDifficulty();
            sudoku.generateBoard(menu);
            if (selected_difficulty != -1) {
                app.WriteText(40, 3, "mistakes: " + to_string(game.getError()));
                sudoku.printPuzzle();
                game.startGame();
            }
        }
    }
}