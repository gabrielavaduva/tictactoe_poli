// Copyright Gabriela Vaduva 2018
#include <stdio.h>
#define NMAX 1000

// initializeaza matricea cu 0
void make_zeros(int a[][NMAX], int n);

// verifica daca index-ul este valid
int index_validation(int x, int y, int n);

// da jucatorului un numar pentru board: 1 sau -1
int give_player_sign(char x);

// cauta un nou index, daca acesta nu e valid
int search_for_new_index(int a[][NMAX], int n,
    int player_sign, int new_x[], int new_y[]);

// afiseaza erorile, apeleaza functiile
// de printare a macroboard-ului si
// afisare a coeficientului de atentie
void game_master(int a[][NMAX], int n, int m, int macro[][NMAX]);

// cauta castigatorul unui board
int search_winner(int a[][NMAX], int n,
    int player_sign, int macro[][NMAX], int x, int y);

// afiseaza macroboard-ul
void print_macro(int macro[][NMAX], int n);

// cauta daca a castigat x in macroboard
int search_winner_x_from_macro(int macro[][NMAX], int n);

// cauta daca a castigat 0 in macro
int search_winner_0_from_macro(int macro[][NMAX], int n);

// afiseaza cine a castigat cu ajutorul functiilor de mai sus
void print_winner(int macro[][NMAX], int n);

// afiseaza coeficientul de atentie
void print_attention(double rounds_played_x,
    double rounds_played_0, int rounds_wone_by_choice_x,
    int rounds_wone_by_choice_0);

int main() {
    // a este matricea cu miniboard-uri
    int a[NMAX][NMAX], n, m;
    int macro[NMAX][NMAX];
    scanf("%d%d", &n, &m);
    make_zeros(a, n*n);
    make_zeros(macro, n);
    game_master(a, n, m, macro);
    return 0;
}

void game_master(int a[][NMAX], int n, int m, int macro[][NMAX]) {
    int i;
    int x, y;
    // am initializat previous_player cu 0
    // deoarece x trebuie sa fie mereu primul
    // player, deci sa fie diferit de cel dinainte
    char player, previous_player = '0';
    // aceasta variabila imi spune daca
    // board-ul este full deja
    int it_is_not_full_yet = 1;
    // player_sign este 1 pentru 1
    // si -1 pentru 0
    int player_sign;
    // noile coordonate in cazul in care
    // coordonatele initiale nu erau valide
    int new_x[1], new_y[1];
    new_x[0] = 0;
    new_y[0] = 0;
    // numarul de runde jucate de tip double
    // pentru precizie
    double rounds_played_0 = 0.0, rounds_played_x = 0.0;
    // numarul de runde jucate si castigate
    // prin alegerea proprie
    int rounds_wone_by_choice_x = 0;
    int rounds_wone_by_choice_0 = 0;
    for (i = 0; i < m; ++i) {
        scanf(" %c", &player);
        scanf("%d %d", &x, &y);
        if (it_is_not_full_yet) {
            // verifica daca e randul lui sau nu
            if (player != previous_player) {
                previous_player = player;
                player_sign = give_player_sign(player);
                if (player_sign == 1) {
                    rounds_played_x++;
                } else {
                    rounds_played_0++;
                }
                if (index_validation(x, y, n*n)) {
                    if (a[x][y] == -1 || a[x][y] == 1) {
                        // cazul in care a fost deja acea mutare
                        printf("NOT AN EMPTY CELL\n");
                        if (search_for_new_index(a, n*n,
                            player_sign, new_x, new_y) ) {
                            search_winner(a, n, player_sign,
                                macro, new_x[0], new_y[0]);
                        } else {
                            printf("FULL BOARD\n");
                            it_is_not_full_yet = 0;
                        }
                    } else {
                        // cazul ideal in care acea mutare este valida 100%
                        a[x][y] = player_sign;
                        if (search_winner(a, n, player_sign, macro, x, y)) {
                            if (player_sign == 1) {
                                rounds_wone_by_choice_x++;
                            } else {
                                rounds_wone_by_choice_0++;
                            }
                        }
                    }
                } else {
                    // cazul index-ului invalid
                    printf("INVALID INDEX\n");
                    if (search_for_new_index(a, n*n,
                        player_sign, new_x, new_y)) {
                        search_winner(a, n, player_sign,
                            macro, new_x[0], new_y[0]);
                    } else {
                        printf("FULL BOARD\n");
                        it_is_not_full_yet = 0;
                    }
                }
            } else {
                // cazul cand nu e randul lui
                printf("NOT YOUR TURN\n");
            }
        }
    }
    print_macro(macro, n);
    print_winner(macro, n);
    print_attention(rounds_played_x, rounds_played_0,
        rounds_wone_by_choice_x, rounds_wone_by_choice_0);
}

int index_validation(int x, int y, int n) {
    if (x < 0 || x >= n || y < 0 || y >= n) {
        return 0;
    } else {
        return 1;
    }
}

int give_player_sign(char x) {
    if (x == 'X') {
        return 1;
    } else {
        return -1;
    }
}
int search_for_new_index(int a[][NMAX], int n, int player_sign,
    int new_x[], int new_y[]) {
    int i, j, k;
    // cauta pe diagonala principala
    for (i = 0; i < n; ++i) {
        if (a[i][i] == 0) {
            a[i][i] = player_sign;
            new_x[0] = i;
            new_y[0] = i;
            // daca a gasit se opreste
            return 1;
        }
    }
    // cauta pe diagonalele urmatoare
    for (k = 1 ; k < n; ++k) {
        // deasupra diagonalei
        for (i = 0; i < n - k; ++i) {
            j = i + k;
            if (a[i][j] == 0) {
                a[i][j] = player_sign;
                new_x[0] = i;
                new_y[0] = j;
                return 1;
            }
        }
        // sub diagonala
        for (i = k; i < n; ++i) {
            j = i - k;
            if (a[i][j] == 0) {
                a[i][j] = player_sign;
                new_x[0] = i;
                new_y[0] = j;
                return 1;
            }
        }
    }
    return 0;
}

int search_winner(int a[][NMAX], int n, int player_sign,
    int macro[][NMAX], int x, int y) {
    int z, w, i, j, ok;
    // z si w sunt coordonatele
    // macroboard-ului
    z = x / n;
    w = y / n;
    if (macro[z][w] == 0) {
        // cazul linie orizontala
        for (i = z * n; i < (z + 1) * n; ++i) {
            ok = 1;
            for (j = w * n; j < (w + 1) * n; ++j) {
                if (a[i][j] != player_sign) {
                    ok = 0;
                }
            }
            if (ok) {
                macro[z][w] = player_sign;
                return 1;
            }
        }
        // cazul linie verticala
        for (j = w * n; j < (w + 1) * n; ++j) {
            ok = 1;
            for (i = z * n; i < (z + 1) * n; ++i) {
                if (a[i][j] != player_sign) {
                    ok = 0;
                }
            }
            if (ok) {
                macro[z][w] = player_sign;
                return 1;
            }
        }
        // caz diagonala principala
        int k;
        ok = 1;
        i = z * n;
        j = w * n;
        for (k = 0; k < n; ++k) {
            if (a[i][j] != player_sign) {
                ok = 0;
            }
            i++;
            j++;
        }
        if (ok) {
            macro[z][w] = player_sign;
            return 1;
        }
        // caz diagonala secundara
        ok = 1;
        i = z * n;
        j = (w + 1) * n - 1;
        for (k = 0; k < n; k++) {
            if (a[i][j] != player_sign) {
                ok = 0;
            }
            i++;
            j--;
        }
        if (ok) {
            macro[z][w] = player_sign;
            return 1;
        }
    }
    return 0;
}

void print_macro(int macro[][NMAX], int n) {
    int i, j;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (macro[i][j] == 0) {
                printf("-");
            }
            if (macro[i][j] == 1) {
                printf("X");
            }
            if (macro[i][j] == -1) {
                printf("0");
            }
        }
        printf("\n");
    }
}

int search_winner_x_from_macro(int macro[][NMAX], int n) {
    // acelasi algoritm de cautare a unui castigator
    // ca pentru miniboard
    int player_1 = 1;
    int i, j;
    int ok;
    // caz linie orizontala
    for (i = 0; i < n; ++i) {
        ok = 1;
        for (j = 0; j < n; ++j) {
            if (macro[i][j] != player_1) {
                ok = 0;
            }
        }
        if (ok) {
            return 1;
        }
    }
    // cazul linie verticala
    for (j = 0; j < n; ++j) {
        ok = 1;
        for (i = 0; i < n; ++i) {
            if (macro[i][j] != player_1) {
                ok = 0;
            }
        }
        if (ok) {
            return 1;
        }
    }
    // caz diagonala principala
    int k;
    ok = 1;
    i = 0;
    j = 0;
    for (k = 0; k < n; ++k) {
        if (macro[i][j] != player_1) {
            ok = 0;
        }
        i++;
        j++;
    }
    if (ok) {
        return 1;
    }
    // caz diagonala secundara
    ok = 1;
    i = 0;
    j = n - 1;
    for (k = 0; k < n; k++) {
        if (macro[i][j] != player_1) {
            ok = 0;
        }
        i++;
        j--;
    }
    if (ok) {
        return 1;
    }
    return 0;
}

int search_winner_0_from_macro(int macro[][NMAX], int n) {
    int player_2 = -1;
    int i, j;
    int ok;
    // caz linie orizontala
    for (i = 0; i < n; ++i) {
        ok = 1;
        for (j = 0; j < n; ++j) {
            if (macro[i][j] != player_2) {
                ok = 0;
            }
        }
        if (ok) {
            return 1;
        }
    }
    // cazul linie verticala
    for (j = 0; j < n; ++j) {
        ok = 1;
        for (i = 0; i < n; ++i) {
            if (macro[i][j] != player_2) {
                ok = 0;
            }
        }
        if (ok) {
            return 1;
        }
    }
    // caz diagonala principala
    int k;
    ok = 1;
    i = 0;
    j = 0;
    for (k = 0; k < n; ++k) {
        if (macro[i][j] != player_2) {
            ok = 0;
        }
        i++;
        j++;
    }
    if (ok) {
        return 1;
    }
    // caz diagonala secundara
    ok = 1;
    i = 0;
    j = n - 1;
    for (k = 0; k < n; k++) {
        if (macro[i][j] != player_2) {
            ok = 0;
        }
        i++;
        j--;
    }
    if (ok) {
        return 1;
    }
    return 0;
}

void print_winner(int macro[][NMAX], int n) {
    if (search_winner_x_from_macro(macro, n) ==
        search_winner_0_from_macro(macro, n)) {
        printf("Draw again! Let's play darts!\n");
    } else if (search_winner_x_from_macro(macro, n)) {
        printf("X won\n");
    } else {
        printf("0 won\n");
    }
}

void print_attention(double rounds_played_x, double rounds_played_0,
    int rounds_wone_by_choice_x, int rounds_wone_by_choice_0) {
    printf("X ");
    if (rounds_played_x == 0) {
        printf("N/A\n");
    } else {
        printf("%.10lf\n", rounds_wone_by_choice_x/rounds_played_x);
    }
    printf("0 ");
    if (rounds_played_0 == 0) {
        printf("N/A\n");
    } else {
        printf("%.10lf\n", rounds_wone_by_choice_0/rounds_played_0);
    }
}

void make_zeros(int a[][NMAX], int n) {
    int i, j;
    for (i = 0; i < n; ++i) {
        for (j =0; j < n; ++j) {
            a[i][j] = 0;
        }
    }
}
