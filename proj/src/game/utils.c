#include "utils.h"

bool is_char (char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

void get_keys() {
    char keys[26];
    memset(keys, 0, sizeof(keys));

    keys[A_T] = 'A';
    keys[B_T] = 'B';
    keys[C_T] = 'C';
    keys[D_T] = 'D';
    keys[E_T] = 'E';
    keys[F_T] = 'F';
    keys[G_T] = 'G';
    keys[H_T] = 'H';
    keys[I_T] = 'I';
    keys[J_T] = 'J';
    keys[K_T] = 'K';
    keys[L_T] = 'L';
    keys[M_T] = 'M';
    keys[N_T] = 'N';
    keys[O_T] = 'O';
    keys[P_T] = 'P';
    keys[Q_T] = 'Q';
    keys[R_T] = 'R';
    keys[S_T] = 'S';
    keys[T_T] = 'T';
    keys[U_T] = 'U';
    keys[V_T] = 'V';
    keys[W_T] = 'W';
    keys[X_T] = 'X';
    keys[Y_T] = 'Y';
    keys[Z_T] = 'Z';

}