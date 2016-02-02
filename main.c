#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NB_PLAYER 10
#define PLATEAU_HEIGHT 200
#define PLATEAU_WIDTH 200

#define LIFE_POINTS 21

#define CORVETTE_ID 6  //pique
#define CORVETTE_WIDTH 1
#define CORVETTE_NB 1

#define DESTROYER_ID 5 //trefle
#define DESTROYER_WIDTH 3
#define DESTROYER_NB 2

#define CROISEUR_ID 3   //coeur
#define CROISEUR_WIDTH 4
#define CROISEUR_NB 2

#define PORTE_AVION_ID 4    //carreaux
#define PORTE_AVION_WIDTH 6
#define PORTE_AVION_NB 1

//initialisation du plateau
void clean_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs);
//impression de la map
void print_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int whosPlaying);
//Fonction random
int randomize(int MIN, int MAX);
//test de position
int test_boat_positon(int x, int y, int direction, int length_boat, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player);
//placement bateau
void set_boat(int id, int size, int nbr, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player);
//Appel de tout les bateaux un par un
void set_all_ships(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs);
//D�finir des couleurs pour les joueurs diff�rents
void color(int t,int f);
//D�finition nbr Joueurs et taille de map
void set_parameters(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]);
//D�fini la couleur des bateaux selon les joueurs
void set_color_ships(int k);
//R�cup�ration des coordonn�es de tir
int set_longitude();
int set_lattitude();
//D�filement des joueurs
void player_rolling(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs);
//test Hits
int test_hit(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude);
//Placement des tirs sur la carte
void update_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude, int playerHit, int touch);
//Test of victory
int victory_test(int lifeTable[NB_PLAYER], int nb_joueurs);
//Set the screen for victory
void set_victory_screen(int lifeTable[NB_PLAYER], int nb_joueurs);
//Init the life points for all the players
void init_table_life(int lifeTable[NB_PLAYER], int nb_joueurs);

int main(int argc, char *argv[])
{
    char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER];
    set_parameters(board_game);

    return 0;
}

void set_victory_screen(int lifeTable[NB_PLAYER], int nb_joueurs){
    int i = 0;
    int winner = 0;
    char flag = 0;
    for(i = 0; i < nb_joueurs; i++){
        if(lifeTable[i] > 0){
            winner = i + 1;
        }
    }

    system("cls");

    while(flag != '\r' && flag != '\n'){
        fflush(stdin);
        for(i = 0; i < 15; i++){
            color(i, 0);
        }
        printf("#############################################################\n");
        printf("#############################################################\n");
        printf("#                                                           #\n");
        printf("#                                                           #\n");
        printf("#                          VICTORY                          #\n");
        printf("#                   Player %d is the best !!!                #\n", winner);
        printf("#                                                           #\n");
        printf("#                                                           #\n");
        printf("#############################################################\n");
        printf("#############################################################\n");
        flag = getchar();
    }
}

int victory_test(int lifeTable[NB_PLAYER], int nb_joueurs){
    int countingPlayers = 0;
    int i = 0;
    for(i = 0; i < nb_joueurs; i++){
        if(lifeTable[i]  == 0){
            countingPlayers++;
        }
    }
    if(countingPlayers == (nb_joueurs - 1)){
        return 0;
    }
    else{
        return 1;
    }
}

void update_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude, int playerHit, int touch){
    if(touch == 1){
        board_game[lattitude - 1][longitude - 65][player] = 15;   //Une �toile/mine en ASCII
        board_game[lattitude - 1][longitude - 65][playerHit] = 15;   //Une �toile/mine en ASCII
    }
    else{
        board_game[lattitude - 1][longitude - 65][player] = 88;   //Un X en ASCII
    }
}

int test_hit(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude){
    int k = 1;
    int playerHit = 0;
    int touch = 0;
    for(k = 1; k <= nb_joueurs; k++){
        if(board_game[lattitude - 1][longitude - 65][k] != '~'){
            if(k == player){
                printf("\nYou can't shoot your own ships ...\n");
                break;
            }
            else{
                playerHit = k;
                touch = 1;
                switch(board_game[lattitude - 1][longitude - 65][playerHit]){
                    case 6:
                        printf("\nxxx You hit a corvette of player %d xxx\n", playerHit);
                        break;
                    case 5:
                        printf("\nxxx You hit a destroyer of player %d xxx\n", playerHit);
                        break;
                    case 3:
                        printf("\nxxx You hit a croiseur of player %d xxx\n", playerHit);
                        break;
                    case 4:
                        printf("\nxxx You hit a porte-avion of player %d xxx\n", playerHit);
                        break;
                    default:
                        break;
                }
            }
        }
        else{
            printf("\nYou missed, try again ...\n");
        }
    }
    update_board(board_game, width, height, nb_joueurs, player, longitude, lattitude, playerHit, touch);
    return playerHit;
}

void init_table_life(int lifeTable[NB_PLAYER], int nb_joueurs){
    int i = 0;
    for(i = 0; i < nb_joueurs; i++){
        lifeTable[i] = LIFE_POINTS;
    }
}

void player_rolling(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs){
    int player = 0;
    char next = ' ';
    int longitude = 0;
    int lattitude = 0;
    int lifeTable[NB_PLAYER];
    init_table_life(lifeTable, nb_joueurs);
    int flag;
    while(flag){
        for(player = 1; player <= nb_joueurs; player++){
            int hit = 0;
            if(lifeTable[player - 1] > 0){
                printf("Player %d have to play...\n", player);
                print_board(board_game, width, height, nb_joueurs, player);
                printf("Give the coordinates you want to shoot ?\n");
                longitude = set_longitude();
                lattitude = set_lattitude();
                hit = test_hit(board_game, width, height, nb_joueurs, player, longitude, lattitude);
                if(hit){
                    (lifeTable[hit - 1])--;
                }
                print_board(board_game, width, height, nb_joueurs, player);
            }
            else{
                printf("\nPlayer %d is dead !!!\n", player);
            }
            while(next != '\r' && next != '\n'){
                fflush(stdin);
                printf("Press enter for next player ...\n");
                next = getchar();
            }
        }
        flag = victory_test(lifeTable, nb_joueurs);
    }
    set_victory_screen(lifeTable, nb_joueurs);
}

int set_longitude(){
    char longitude = 0;
    fflush(stdin);
    printf("Longitude : ");
    scanf("%c", &longitude);
    return longitude;
}

int set_lattitude(){
    int lattitude = 0;
    fflush(stdin);
    printf("Lattitude : ");
    scanf("%d", &lattitude);
    return lattitude;
}

void set_color_ships(int k){
    switch(k){
        case 0:
            color(15, 0);    //couleur de la mer
            break;
        case 1:
            color(12, 0);   //Couleur ROUGE du Player 1
            break;
        case 2:
            color(9, 0);   //Couleur BLEU du player 2
            break;
        case 3:
            color(10, 0);   //Couleur VERT FLUO du player 3
            break;
        case 4:
            color(14, 0);   //Couleur JAUNE FLUO du player 4
            break;
        default:
            color(15, 0);    //couleur de la mer
            break;
    }
}

void set_parameters(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]){
    int width;
    int height;
    int nb_joueurs;

    printf("Veuillez saisir un nombre de joueur : \n");
    scanf("%d", &nb_joueurs);
    printf("Veuillez saisir une largeur de map : \n");
    scanf("%d", &width);
    printf("Veuillez saisir une hauteur de map : \n");
    scanf("%d", &height);
    clean_board(board_game, width, height, nb_joueurs);
    set_all_ships(board_game, width, height, nb_joueurs);
    player_rolling(board_game, width, height, nb_joueurs);
}

void color(int t,int f){
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}
/*les valeurs:
0: noir
1: bleu fonc�
2: vert
3: bleu-gris
4: marron
5: pourpre
6: kaki
7: gris clair
8: gris
9: bleu
10: vert fluo
11: turquoise
12: rouge
13: rose fluo
14: jaune fluo
15: blanc */

void set_all_ships(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs){
    int p = 0;
    for(p = 0; p < nb_joueurs; p++){
        set_boat(PORTE_AVION_ID, PORTE_AVION_WIDTH, PORTE_AVION_NB, board_game, width, height, p);
        set_boat(CROISEUR_ID, CROISEUR_WIDTH, CROISEUR_NB, board_game, width, height, p);
        set_boat(DESTROYER_ID, DESTROYER_WIDTH, DESTROYER_NB, board_game, width, height, p);
        set_boat(CORVETTE_ID, CORVETTE_WIDTH, CORVETTE_NB, board_game, width, height, p);
   }
}

void set_boat(int id, int size, int nbr, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player){
    int i = 0;
    int j = 0;

    while(nbr > 0){
        int flag = 1;
        int direction = randomize(0,3);

        while(flag){
            int x = randomize(1, width);
            int y = randomize(1, height);
            if(test_boat_positon(x, y, direction, size, board_game, width, height, player) == 0){
                if(direction == 0){
                    for(i = y; i <= (y + (size - 1)); i++){
                        j = x;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id; //Cr�ation map joueurs
                        printf("Player %d : %c sur x = %d, y = %d\n", player+1, id, j, i);
                        flag = 0;
                    }
                }
                else if(direction == 1){
                    for(j = x; j <= (x + (size - 1)); j++){
                        i = y;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id; //Cr�ation map joueurs
                        printf("Player %d : %c sur x = %d, y = %d\n", player+1, id, j, i);
                        flag = 0;
                    }
                }
                else if(direction == 2){
                    for(i = y; i >= (y - (size - 1)); i--){
                        j = x;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id; //Cr�ation map joueurs
                        printf("Player %d : %c sur x = %d, y = %d\n", player+1, id, j, i);
                        flag = 0;
                    }
                }
                else if(direction == 3){
                    for(j = x; j >= (x - (size - 1)); j--){
                        i = y;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id; //Cr�ation map joueurs
                        printf("Player %d : %c sur x = %d, y = %d\n", player+1, id, j, i);
                        flag = 0;
                    }
                }
            }
        }
        nbr--;
    }
}

int test_boat_positon(int x, int y, int direction, int size, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player){
    int test = 1;
    int i = 0;
    int j = 0;
    if(direction == 0){
        if((y + (size - 1)) <= height){ //Test que le bateau rentre bien en hauteur
            for(i = y; i <= (y + (size - 1)); i++){
                j = x;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 1){
        if((x + (size - 1)) <= width){  //Test que le bateau rentre bien en largeur
            for(j = x; j <= (x + (size - 1)); j++){
                i = y;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 2){
        if((y - (size - 1)) > 0){ //Test que le bateau rentre bien en hauteur
            for(i = y; i >= (y - (size - 1)); i--){
                j = x;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 3){
        if((x - (size - 1)) > 0){  //Test que le bateau rentre bien en largeur
            for(j = x; j >= (x - (size - 1)); j--){
                i = y;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    if(test == 1){
        return -1;
    }
    else{
        return 0;
    }
}

int randomize(int MIN, int MAX){
    srand(time(NULL));
    return (rand() % (MAX - MIN + 1)) + MIN;
}

void clean_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs){
    int i = 0;
    int j = 0;
    int k = 0;
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            for(k = 0; k < (nb_joueurs + 1); k++){
                board_game[i][j][k] = '~';
            }
        }
    }
}

void print_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int whosPlaying){

    system("cls");

    int i = 0;
    int j = 0;
    for(i = -2; i < height; i++){
        for(j = -3; j < width; j++){
            set_color_ships(0);
            if(board_game[i][j][whosPlaying] == 6 || board_game[i][j][whosPlaying] == 5 || board_game[i][j][whosPlaying] == 3 || board_game[i][j][whosPlaying] == 4){
                set_color_ships(whosPlaying);
            }
            else if(board_game[i][j][whosPlaying] == 88 || board_game[i][j][whosPlaying] == 15){
                color(12, 15);
            }
            if(i == -2 && j >= 0){
                color(0, 15);
                printf("%c", (char)(j + 65));
                if(j > 90){
                    printf(" ");
                }
            }
            else if(i == -2 && j < 0){
                printf(" ");
            }
            else if(i == -1){
                printf(" ");
            }
            else{
                if(j == -3 && i >= 0){
                    color(0, 15);
                    printf("%d", i + 1);
                }
                else if(j == -1 && i >= 0){
                    if(i <= 8){
                        color(0, 0);
                        printf("  ");
                    }
                    else{
                        color(0, 0);
                        printf(" ");
                    }
                }
                else if(j >= 0){
                    printf("%c", board_game[i][j][whosPlaying]);
                }
            }
        }
        printf("\n");
    }
}
