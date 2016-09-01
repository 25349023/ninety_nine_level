#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <windows.h>

#define remind 1
#define debug 0

using namespace std;

struct Card_type{
    char word;
    char color;
};

class people{
public:
    int quantity;

    void deal_card(int = 0);
    void get_card();
    void print_card();
    Card_type play_card(int, int* = NULL, int = 0);
    int find_card(char);

    static bool check(string, int = 0);

    people(){
        this->quantity = 0;
    }

protected:
    char hand[32];
    char hand_suit[32];
};

class player:public people{
public:
    void print_card(){
        int max_card, more_than_7;
        max_card = (this->quantity > 7)? 7 : this->quantity;
        more_than_7 = (this->quantity > 7)? 1 : 0;

        for (int k = 0; k <= more_than_7; k++){
            for (int i = 0; i < max_card; i++){
                if ((i + 7 * k) >= quantity)
                    break;
                cout << " _______ " << "  ";
            }
            cout << endl;
            for (int j = 0; j < 2; j++){
                for (int i = 0; i < max_card; i++){
                    if ((i + 7 * k) >= quantity)
                        break;
                    if (j == 0)
                        cout << "|" << this->hand_suit[i+7*k] << "      |" << "  ";
                    else
                        cout << "|       |" << "  ";
                }
                cout << endl;
            }

            for (int i = 0; i < max_card; i++){
                if ((i + 7 * k) >= quantity)
                    break;
                cout << "|   " << this->hand[i+7*k] << "   |" << "  ";
            }
            cout << endl;

            for (int j = 0; j < 2; j++){
                for (int i = 0; i < max_card; i++){
                    if ((i + 7 * k) >= quantity)
                        break;
                    if (j == 0)
                        cout << "|       |" << "  ";
                    else
                        cout << "|      " << this->hand_suit[i+7*k] << "|" << "  ";
                }
                cout << endl;
            }
            for (int i = 0; i < max_card; i++){
                if ((i + 7 * k) >= quantity)
                    break;
                cout << " ------- " << "  ";
            }
            cout << endl;
        }
    }

    player(){
        this->quantity = 0;
    }
private:
    char last_card;
};

class enemy:public people{
public:
    bool lose = false;

    void print_card(){
        char smile = 2, black = 1;
        int max_card, more_than_7;
        max_card = (this->quantity > 7)? 7 : this->quantity;
        more_than_7 = (this->quantity > 7)? 1 : 0;

        for (int k = 0; k <= more_than_7; k++){
            for (int i = 0; i < max_card; i++){
                if ((i + 7 * k) >= quantity)
                    break;
                cout << " _______ " << "  ";
            }
            cout << endl;
            for (int j = 0; j < 5; j++){
                for (int i = 0; i < max_card; i++){
                    if ((i + 7 * k) >= quantity)
                        break;
                    cout << "|";
                    for (int k = 0; k < 7; k++){
                        if ((j+k) % 2 == 0)
                            cout << smile;
                        else
                            cout << black;
                    }
                    cout << "|" << "  ";
                }
                cout << endl;
            }
            for (int i = 0; i < max_card; i++){
                if ((i + 7 * k) >= quantity)
                    break;
                cout << " ------- " << "  ";
            }
            cout << endl;
        }
    }
    virtual int find_large(){
        return -1;
    }
    virtual int find_small(){
        return -1;
    }
    virtual int use_which_card(char){
        return -1;
    }

    enemy(){
        this->quantity = 0;
    }
};

class stupid_enemy:public enemy{
public:
    stupid_enemy(){
        this->quantity = 0;
    }
};

class moderate_enemy:public enemy{
public:
    virtual int find_large();
    virtual int find_small();

    moderate_enemy(){
        this->quantity = 0;
    }
};

class smart_enemy:public moderate_enemy{
public:
    virtual int use_which_card(char);

    smart_enemy(){
        this->quantity = 0;
    }
};

template <class type>
void exchange(type*, type*);

template <class type>
void shuf(type[], int);

int sum = 0;
int left_card = 52;
bool point_flag = false;//this controls whether computer uses the 5s or not.
bool reverse_flag = false;
char card[4][13];
char suit[4][13];
char *cardptr = &card[0][0];
char *suitptr = &suit[0][0];

int main(){
    system("title Ninety-nine");

    /**< front */

    for (int i = 0; i < 4; i++){
        for (unsigned char j = 0; j < 13; j++){
            switch (j){
            case 0:
                card[i][j] = 'a';
                break;
            case 1 ... 8:
                card[i][j] = j+49;
                break;
            case 9:
                card[i][j] = 'X';
                break;
            case 10:
                card[i][j] = 'J';
                break;
            case 11:
                card[i][j] = 'Q';
                break;
            case 12:
                card[i][j] = 'K';
                break;
            }
        }
    }
    card[0][0] = 'A';
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 13; j++){
            switch (i){
            case 0:
                suit[i][j] = 6;
                break;
            case 1:
                suit[i][j] = 3;
                break;
            case 2:
                suit[i][j] = 4;
                break;
            case 3:
                suit[i][j] = 5;
                break;
            }
        }
    }


    srand(time(NULL));
    for (int i = 0; i < 2000; i++){///shuffle
        static int x, m;
        static int y, n;
        x = (rand() % 4);
        y = (rand() % 13);
        m = (rand() % 4);
        n = (rand() % 13);

        exchange(&card[x][y], &card[m][n]);
        exchange(&suit[x][y], &suit[m][n]);
    }
    system("color F0");

    /**< main */

    player pla;
    int number, level, used_card_amount = 1, used_index = 0;
    char found;

    do {
        cout << "Please input what kind of enemy you want to challenge (1 to 4):";
        cin >> level;
        if (level == 0){
            system("cls");
            system("color 3E");
            cout << "\tThis is a game named Ninety-Nine." << endl
                 << "\tYou have to play all the cards in your hand," << endl
                 << "\tand avoid the total number exceeding 99." << endl << endl
                 << "\tFollowings are the rules of this game:" << endl
                 << "\t## At the beginning of the game, you\'ll get 12 cards." << endl
                 << "\t## The 2s to 3s, 6s to 9s is the normal card,\n which means their value is the same as its literal number." << endl
                 << "\t## Value of a lower-case 'a' is one,\n\t   BUT a upper-case 'A' represents to bring the total number return to ZERO." << endl;
            system("pause");
            system("cls");
            system("color F0");
        }
    } while (level > 4 || level <= 0);

    enemy *ene;
    int tarnum;//tarnum means the number of enemy.
    bool flag = false;//flag is the thing which
    switch (level){ //control whether computer wins or not.
    case 1:
        ene = new stupid_enemy[2];
        tarnum = 2;
        used_card_amount = 28;
        break;
    case 2:
        ene = new moderate_enemy[3];
        tarnum = 3;
        used_card_amount = 24;
        break;
    case 3:
        ene = new moderate_enemy[2];
        tarnum = 2;
        used_card_amount = 34;
        break;
    case 4:
        ene = new smart_enemy[3];
        tarnum = 3;
        used_card_amount = 32;
        break;
    }

    pla.deal_card(level);///deal cards
    for (int i = 0; i < tarnum; i++){
        ene[i].deal_card(level);
    }

    int roundnum = 0;
    char shuffle[used_card_amount], suit_shuffle[used_card_amount];//the amount of cards which will be reshuffle.

    ///game start
    while (1){
        int end_flag = 0, check = 0;//end_flag and check controls
                                   //whether computer is all lose or not.
        int lose_enemy[3] = {4, 4, 4}, lose_enemy_index = 0;
        int i = 0;  //i controls the order of enemy(s)
        Card_type used_temp;
        point_flag = false;
        if (reverse_flag){
            i = tarnum - 1;
        }

        if ((roundnum % 20 == 0) && (roundnum > 0)){
            char ans;
            system("color 72");
            Sleep(150);
            system("color 75");
            cout << "Computer wants to tie with you[Y/n]:";
            cin >> ans;
            if (ans != 'n' && ans != 'N'){
                system("color F9");
                cout << endl << "====================================" << endl << endl;
                cout << "It's a Tie!" << endl << endl;
                system("pause");
                return 0;
            }
        }

        ///your turn
        Sleep(50);
#if remind == 1
        system("color 72");
        Sleep(150);
        system("color F0");
#endif // remind
        roundnum++;
        cout << endl << "----------Your cards-----------" << endl;
        pla.print_card();
        cout << endl << "Now, the point(s) is(are) : " << sum << endl;
        cout << "Which card do you want to play? :";
        do {
            cin >> found;
            if (found == '!')
                exit(0);
            if (found == '#'){
                for (int k = 0; k < tarnum; k++){
                    if (ene[k].lose){
                        check++;
                    }
                }
                if (check == tarnum){
                    break;
                }
            }
            number = pla.find_card(found);
            if (number == -1)
                cout << "Again :";
        } while (number == -1); //found can't be found
        if (check == tarnum){
            system("color F2");
            cout << endl << "====================================" << endl << endl;
            cout << "You win!" << endl << endl;
            break;
        }

        used_temp = pla.play_card(number, &i, tarnum+1);
        if (people::check("You"))//check if lose
            break;
        if (left_card)
            pla.get_card();

        shuffle[used_index] = used_temp.word;
        suit_shuffle[used_index++] = used_temp.color;
        if (used_index >= used_card_amount){
            used_index = 0;
        }

        if (!left_card){
            shuf(shuffle, used_card_amount);
            shuf(suit_shuffle, used_card_amount);
            left_card = used_card_amount;
            cardptr = &shuffle[0];
            suitptr = &suit_shuffle[0];
        }

        cout << "-------------------------------" << endl << endl;


        ///computer's turn
        for ( ; (i >= 0) && (i < tarnum); ){
            bool repeat_flag = false;
            if (ene[i].lose){
                end_flag++;
                for (int j = 0; j < 3; j++){
                    if (i == lose_enemy[j]){
                        end_flag--;
                        repeat_flag = true;
                        break;
                    }
                }
                if (!repeat_flag){//if the index of lose doesn't repeat
                    lose_enemy[lose_enemy_index++] = i;
                }
                if (!reverse_flag){//if they don't use reverse.
                    i++;
                }
                else { // or, they do.
                    i--;
                }
#if debug == 1
                cout << endl << "end flag : " << end_flag << endl;
#endif // debug
                continue;
            }
            cout << endl << "-------computer" << i+1 << "\'s card--------" << endl;
            ene[i].print_card();
            cout << endl << "Now, the point(s) is(are) : " << sum << endl;
            srand(time(NULL));
            number = (rand() % ene[i].quantity);
            cout << endl << "Computer is playing..." << endl;
            if (level == 1 && sum <= 54){
                int numtemp;
                numtemp = ene[i].find_card('4');
                if (numtemp != -1)
                    number = numtemp;
            }
            if (level >= 2 && sum <= 75){
                int numtemp;
                numtemp = ene[i].find_large();
                if (numtemp != -1)
                    number = numtemp;
            }
            if (level == 3 && (sum > 18 && sum <= 46)){
                int numtemp;
                numtemp = ene[i].find_card('K');
                if (numtemp != -1)
                    number = numtemp;
            }
            if (level >= 3 && sum >= 78){
                int numtemp;
                numtemp = ene[i].find_small();
                if (numtemp != -1)
                    number = numtemp;
            }
            if (level >= 4){
                int numtemp;
                numtemp = ene[i].use_which_card(found);
                if (numtemp != -1)
                    number = numtemp;
            }
            if (level == 4)
                Sleep(2000);
            else
                Sleep(1500);
            used_temp = ene[i].play_card(number);
            if (people::check("Computer", i+1)){
                ene[i].lose = true;
                end_flag++;
                if (!reverse_flag){//if they don't use reverse.
                    i++;
                }
                else { // or, they do.
                    i--;
                }
                continue;
            }
            if (left_card)
                ene[i].get_card();

            shuffle[used_index] = used_temp.word;
            suit_shuffle[used_index++] = used_temp.color;
            if (used_index >= used_card_amount){
                used_index = 0;
            }

            if (!left_card){
                shuf(shuffle, used_card_amount);
                shuf(suit_shuffle, used_card_amount);
                left_card = used_card_amount;
                cardptr = &shuffle[0];
                suitptr = &suit_shuffle[0];
            }

            cout << "-------------------------------" << endl << endl;

            if (point_flag){
                break;
            }
            if (!reverse_flag){//if they don't use reverse.
                i++;
            }
            else { // or, they do.
                i--;
            }
        }
        if (flag)
            break;
        if (end_flag == tarnum){
            system("color F2");
            cout << endl << "====================================" << endl << endl;
            cout << "You win!" << endl << endl;
            break;
        }
    }

    system("pause");
    return 0;
}

void people::deal_card(int type){
    if (left_card == 0){
        cout << "There isn't any cards left!";
        return;
    }
    int cardnum;
    switch (type){
    case 0:
        cardnum = 12;
        break;
    case 1:
        cardnum = 8;
        break;
    case 2:
        cardnum = 7;
        break;
    case 3:
        cardnum = 6;
        break;
    case 4:
        cardnum = 5;
        break;
    default:
        cardnum = 12;
    }

    for (int i = 0; i < cardnum; i++){
        this->hand[i] = *cardptr++;
        this->hand_suit[i] = *suitptr++;
        quantity++;
        left_card--;
    }
}

void people::get_card(){
    this->hand[quantity] = *cardptr++;
    this->hand_suit[quantity++] = *suitptr++;
    left_card--;
}

Card_type people::play_card(int num, int *iptr, int target){
    int a, input = 0;
    switch (this->hand[num]){
    case 'A':
        a = 0;
        system("color AF");
        system("cls");
        cout << " |    -------" << endl;
        for(int i = 0; i < 6; i++)
            cout << " |   |       |" << endl;
        cout << "\\|/   -------" << endl;
        cout << endl << "Reset!!" << endl;
        sum = 0;
        Sleep(1500);
        system("color F0");
        system("cls");
        break;
    case 'a':
        a = 1;
        break;
    case '2' ... '3':
        a = this->hand[num]-48;
        break;
    case '4':
        a = 0;
        system("color 0E");
        system("cls");
        cout << " <-----------" << endl;
        cout << "|            |" << endl;
        cout << "|            |" << endl;
        cout << "|            |" << endl;
        cout << " ----------->" << endl;
        cout << "Reverse order!!" << endl;
        if (iptr != NULL){
            if (!reverse_flag)
                *iptr = target - 2;
            else
                *iptr = 0;
        }
        reverse_flag = !reverse_flag;
        Sleep(1500);
        system("color F0");
        system("cls");
        break;
    case '5':
        a = 0;
        if (target > 1){ //if it's player's turn
            do {
                cout << endl << "Choose target(1 -> computer1";
                if (target > 2){
                    cout << ", 2 -> computer2";
                    if (target > 3)
                        cout << ", 3 -> computer3):";
                    else
                        cout << "):";
                }
                else {
                    cout << "):";
                }
                cin >> input;
            } while (input < 1 || input > 3);
        }
        if (iptr != NULL)
            *iptr = (input - 1);
        if (target == 0)
            point_flag = true;
        system("color 0B");
        system("cls");
        cout << "       |-|" << endl;
        cout << "       | |" << endl;
        cout << "       |=| | | |" << endl;
        cout << "  \\-\\  | | | | |" << endl;
        cout << "   \\=\\    " << input << "    |" << endl;
        cout << "    \\ \\ _____ /" << endl;
        cout << endl << "Appoint!!" << endl;
        Sleep(1500);
        system("color F0");
        system("cls");
        break;
    case '6' ... '9':
        a = this->hand[num]-48;
        break;
    case 'X':
        if (target >= 1){ //if it's player's turn
            do {
                cout << endl << "+10 or -10 ?:";
                cin >> input;
            } while (input != 10 && input != -10);
        }
        else { //or...
            if (sum > 88)
                input = -10;
            else
                input = 10;
        }
        a = input;
        break;
    case 'J':
        a = 0;
        system("color 0D");
        system("cls");
        cout << "          |-|" << endl;
        cout << "       |-|| ||-|" << endl;
        cout << "       | ||=|| ||-|" << endl;
        cout << "       |=|| ||=||=|" << endl;
        cout << "       | || || || |" << endl;
        cout << "  \\-\\             |" << endl;
        cout << "   \\=\\            |" << endl;
        cout << "    \\=\\ ________ /" << endl;
        cout << endl << "Pass!!" << endl;
        Sleep(1500);
        system("color F0");
        system("cls");
        break;
    case 'Q':
        if (target >= 1){ //if it's player's turn
            do {
                cout << endl << "+20 or -20 ?:";
                cin >> input;
            } while (input != 20 && input != -20);
        }
        else {//or...
            if (sum > 79)
                input = -20;
            else
                input = 20;
        }
        a = input;
        break;
    case 'K':
        a = 0;
        system("color CF");
        system("cls");
        cout << " -----    -----" << endl;
        cout << "|     |  |     |" << endl;
        cout << "|     |  |     |" << endl;
        cout << " -----    -----" << endl;
        cout << "      |        |" << endl;
        cout << "      |        |" << endl;
        cout << "-----    -----" << endl;
        cout << endl << "Ninety-Nine!!" << endl;
        Sleep(1500);
        system("color F0");
        system("cls");
        sum = 99;
        break;
    default:
        a = 0;
    }
    sum += a;
    Card_type temp = {this->hand[num], this->hand_suit[num]};

    for (int i = num; i < quantity-1; i++){
        this->hand[i] = this->hand[i+1];
        this->hand_suit[i] = this->hand_suit[i+1];
    }
    quantity--;

    return temp;
}

int people::find_card(char c){
    for (int i = 0; i < quantity; i++){
        if (this->hand[i] == c)
            return i;
    }
    return -1;
}

bool people::check(string str, int no){
    if (sum > 99){
        cout << endl << "====================================" << endl;
        if (no != 0){
            system("color 70");
            cout << str << no << " lose!" << endl << endl;
            Sleep(1000);
            system("color F0");
        }
        else {
            system("color 70");            cout << str << " lose!" << endl << endl;
        }
        sum = 99;
        return true;
    }
    if (sum < 0)
        sum = 0;
    return false;
}

int moderate_enemy::find_large(){
    int result = -1;
    if ((result = this->find_card('9')) != -1)
        return result;
    else if ((result = this->find_card('8')) != -1)
        return result;
    else if ((result = this->find_card('7')) != -1)
        return result;

    return result;
}

int moderate_enemy::find_small(){
    int result = -1;
    if ((result = this->find_card('5')) != -1)
        return result;
    else if ((result = this->find_card('X')) != -1)
        return result;
    else if ((result = this->find_card('J')) != -1)
        return result;
    else if ((result = this->find_card('Q')) != -1)
        return result;
    else if ((result = this->find_card('K')) != -1)
        return result;
    else if ((result = this->find_card('2')) != -1)
        return result;
    else if ((result = this->find_card('4')) != -1)
        return result;
    else if ((result = this->find_card('A')) != -1)
        return result;

    return result;

}

int smart_enemy::use_which_card(char last){
    int result = -1;
    if (last == 'A'){
        if ((result = this->find_card('K')) != -1)
            return result;
    }
    if (last == 'Q'){
        if ((result = this->find_card('Q')) != -1)
            return result;
    }
    if (last == 'J'){
        if ((result = this->find_card('4')) != -1)
            return result;
    }

    switch (sum){
    case 0 ... 24:
        if ((result = this->find_card('9')) != -1)
            return result;
        else if ((result = this->find_card('8')) != -1)
            return result;
        else
            break;
    case 25 ... 39:
        if ((result = this->find_card('K')) != -1)
            return result;
        else if ((result = this->find_card('Q')) != -1)
            return result;
        else
            break;
    case 40 ... 55:
        if ((result = this->find_card('X')) != -1)
            return result;
        else if ((result = this->find_card('9')) != -1)
            return result;
        else if ((result = this->find_card('8')) != -1)
            return result;
        else if ((result = this->find_card('7')) != -1)
            return result;
        else
            break;
    case 56 ... 70:
        if ((result = this->find_card('9')) != -1)
            return result;
        else if ((result = this->find_card('8')) != -1)
            return result;
        else if ((result = this->find_card('7')) != -1)
            return result;
        else if ((result = this->find_card('X')) != -1)
            return result;
        else
            break;
    case 71 ... 79:
        if ((result = this->find_card('Q')) != -1)
            return result;
        else if ((result = this->find_card('9')) != -1)
            return result;
        else if ((result = this->find_card('K')) != -1)
            return result;
        else if ((result = this->find_card('4')) != -1)
            return result;
        else
            break;
    case 80 ... 89:
        if ((result = this->find_card('X')) != -1)
            return result;
        else if ((result = this->find_card('9')) != -1)
            return result;
        else if ((result = this->find_card('J')) != -1)
            return result;
        else if ((result = this->find_card('K')) != -1)
            return result;
        else
            break;
    case 90 ... 95:
        if (last == '4'){
            if ((result = this->find_card('4')) != -1)
                return result;
        }
        if (last == '5'){
            if ((result = this->find_card('J')) != -1)
                return result;
        }
        if ((result = this->find_card('X')) != -1)
            return result;
        else if ((result = this->find_card('a')) != -1)
            return result;
        else if ((result = this->find_card('5')) != -1)
            return result;
        else if ((result = this->find_card('2')) != -1)
            return result;
        else if ((result = this->find_card('3')) != -1)
            return result;
        else if ((result = this->find_card('J')) != -1)
            return result;
        else if ((result = this->find_card('K')) != -1)
            return result;
        else if ((result = this->find_card('4')) != -1)
            return result;
        else
            break;
    case 96 ... 99:
        if (last == '4'){
            if ((result = this->find_card('4')) != -1)
                return result;
        }
        if (last == '5'){
            if ((result = this->find_card('J')) != -1)
                return result;
        }
        if ((result = this->find_card('Q')) != -1)
            return result;
        else if ((result = this->find_card('a')) != -1 && sum <= 98)
            return result;
        else if ((result = this->find_card('X')) != -1)
            return result;
        else if ((result = this->find_card('5')) != -1)
            return result;
        else if ((result = this->find_card('3')) != -1 && sum <= 96)
            return result;
        else if ((result = this->find_card('J')) != -1)
            return result;
        else if ((result = this->find_card('K')) != -1)
            return result;
        else if ((result = this->find_card('4')) != -1)
            return result;
        else if ((result = this->find_card('A')) != -1 && sum > 97)
            return result;
        else
            break;
    }

    return result;
}

template <class type>
void exchange(type *a, type *b){
    type temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

template <class type>
void shuf(type array[], int n){
    for (int i = 0; i < 2000; i++){///shuffle
        static int x, m;
        srand(time(NULL));
        x = (rand() % n);
        srand(time(NULL));
        m = (rand() % n);

        exchange(&array[x], &array[m]);
    }
}
