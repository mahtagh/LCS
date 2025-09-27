#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100 // Maximum string size
#define Merg_size 4

/* Remove all duplicate of character string[i] */
void removeAll(char *str, const char toRemove, int index) {
    int i;

    while (str[index] != '\0') {
        /* If duplicate character is found */
        if (str[index] == toRemove) {
            /* Shift all characters from current position to one place left */
            i = index;
            while (str[i] != '\0') {
                str[i] = str[i + 1];
                i++;
            }
        } else {
            index++;
        }
    }
}

void removeDuplicates(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        removeAll(str, str[i], i + 1);
        i++;
    }

}

// find the index of char s in string
int charIndex(char s, char *string, int k) {
    int i;
    int counter[MAX_SIZE];
    int c = 0;
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == s) {
            counter[c] = i + 1;
            c++;
        }
    }
    if (k > c - 1)
        return -1;
    else
        return counter[k] - 1;
}

// get value to successor table
int **makeMatrix(char *a, char *merg) {
    int i, j;
    char ch[MAX_SIZE];
    strcpy(ch, "#");

    strcat(ch, a);
    strcpy(a, ch);

    // size of col and row
    int row = strlen(merg);
    int col = strlen(a);

    int **matrix = (int **) malloc(sizeof(int *) * row);
    for (j = 0; j < strlen(merg); j++) {
        int k = 0;
        int d = 0;
        matrix[j] = (int *) malloc(sizeof(int) * col);
        for (i = 0; i < strlen(a); i++) {
            if (a[i] == merg[j]) {
                k++;
            }
            d = charIndex(merg[j], a, k);
            if (d == -1) {
                matrix[j][i] = strlen(a);
            } else {
                matrix[j][i] = d;
            }
        }
    }
    return matrix;
}

void printMatrix(char *a, char *merg) {
    int **matrix;
    //making the successor matrix T for two sequence
    matrix = makeMatrix(a, merg);
    int i, j;

    //printing the matrix
    for (i = 0; i < strlen(merg); i++) {
        for (j = 0; j < strlen(a); j++) {
            printf(" %3d ", matrix[i][j]);
        }
        printf("\n");
    }

}

struct pos {
    int i;
    int j;
    int size;
};

// generate successor table
struct pos *succ(int **matrix1, int **matrix2, int i, int j, int size, int max, int *sizeSucc) {
    struct pos *ps = malloc(sizeof(struct pos) * size);
    //printf("%d",max);

    int counter = 0;
    for (int a = 0; a < size; a++) {
        //printf("%d %d",matrix1[a][i] , matrix2[a][j]);
        if (matrix1[a][i] != max && matrix2[a][j] != max) {
            ps[counter].i = matrix1[a][i];
            ps[counter].j = matrix2[a][j];
            counter++;
        }
    }
    //printf("%d",counter);

    struct pos *result_pos = malloc(sizeof(struct pos) * counter);

    for (int k = 0; k < counter; k++) {
        result_pos[k].i = ps[k].i;
        result_pos[k].j = ps[k].j;
        result_pos[k].size = ((pow(result_pos[k].i, 2)) + (pow(result_pos[k].j, 2)));

    }
    *sizeSucc = counter;
    return result_pos;
}

// function for qsort
int cmpfunc(const void *a, const void *b) {
    struct pos *data_1 = (struct pos *) a;
    struct pos *data_2 = (struct pos *) b;
    return (data_1->size - data_2->size);
}

//minimize the successor
struct pos *Minima(struct pos *ps, int size, char *merg, int *arraySize) {

    if (size == 1) {
        *arraySize = 1;
        return ps;
    } else {
        qsort(ps, size, sizeof(struct pos), cmpfunc);
        int last = strlen(merg) - 1;
        int counter = 0;
        for (int k = size - 1; k > 1; k--) {
            if ((ps[0].i <= ps[k].i) && (ps[0].j <= ps[k].j)) {
                ps[k].i = -1;
                ps[k].j = -1;
                counter++;
            }
        }

        int S = size - counter;
        struct pos *result_pos = malloc(sizeof(struct pos) * S);

        int c = 0;
        for (int k = 0; k < size; k++) {
            if (ps[k].i != -1 && ps[k].j != -1) {
                result_pos[c].i = ps[k].i;
                result_pos[c].j = ps[k].j;
                result_pos[c].size = ((pow(result_pos[c].i, 2)) + (pow(result_pos[c].j, 2)));
                c++;
            }

        }
        *arraySize = c;
        return result_pos;
    }
}

struct pos *CopyPos(struct pos *ps, int sizePS) {
    struct pos *result = malloc(sizeof(struct pos) * sizePS);

    for (int j = 0; j < sizePS; j++) {
        result[j].i = ps[j].i;
        result[j].j = ps[j].j;
        result[j].size =
                ((pow(result[j].i, 2)) + (pow(result[j].j, 2)));
    }
    return result;
}

// union of 2 pos array
struct pos *Union(struct pos *ps, struct pos *result_pos, int sizePS, int sizeResult, int *unionsize) {
    if (sizeResult == 0) {
        //printf("hi");
        *unionsize = sizePS;
        return ps;
    } else if (sizePS == 0) {
        //printf("hi");
        *unionsize = sizeResult;
        return result_pos;
    } else {
        struct pos *result = malloc(sizeof(struct pos) * MAX_SIZE);

        for (int j = 0; j < sizePS; j++) {
            result[j].i = ps[j].i;
            result[j].j = ps[j].j;
            result[j].size =
                    ((pow(result[j].i, 2)) + (pow(result[j].j, 2)));
        }
        int sizeRe = sizePS;
        //printf("size %d \n",sizeRe);
        int check = -1;
        for (int i = 0; i < sizeResult; i++) {
            for (int j = 0; j < sizePS; j++) {
                if (ps[j].i != result_pos[i].i || ps[j].j != result_pos[i].j) {
                    check = 1;
                } else {
                    check = -1;
                    j = sizePS;
                }
            }
            if (check == 1) {
                //printf("%d %d \n", result_pos[i].i , result_pos[i].j);
                result[sizeRe].i = result_pos[i].i;
                result[sizeRe].j = result_pos[i].j;
                result[sizeRe].size =
                        ((pow(result[sizeRe].i, 2)) + (pow(result[sizeRe].j, 2)));
                sizeRe++;
                //printf("2 %d %d %d\n", result[sizeRe].i , result[sizeRe].j,sizeRe);
            }
            check = -1;
        }
        *unionsize = sizeRe;
        return result;
    }
}

// find the length of lcs
struct pos **Quick_MLCS(char *a, char *b, char *merg, int *level, int *sizeArray) {

    int **matrix1;
    int **matrix2;
    matrix1 = makeMatrix(a, merg);
    matrix2 = makeMatrix(b, merg);

    int row = MAX_SIZE;

    struct pos **matrix = (struct pos **) malloc(sizeof(struct pos *) * row);

    struct pos *ps = malloc(sizeof(struct pos) * MAX_SIZE);
    int *sizeMatrix = malloc(sizeof(int) * MAX_SIZE);
    ps[0].i = 0;
    ps[0].j = 0;
    ps[0].size = 0;
    int sizeps = 1;
    int k = 0;

    while (sizeps != 0) {

        matrix[k] = malloc(sizeof(struct pos) * sizeps);
        matrix[k] = CopyPos(ps, sizeps);
        sizeMatrix[k] = sizeps;

        struct pos *result_pos = malloc(sizeof(struct pos) * MAX_SIZE);
        int sizeResult = 0;
        int sizeps1 = 0;
        for (int i = 0; i < sizeps; i++) {
            //printf("hi");
            struct pos *ps1 = succ(matrix1, matrix2, ps[i].i, ps[i].j, strlen(merg), strlen(a), &sizeps1);
            result_pos = Union(ps1, result_pos, sizeps1, sizeResult, &sizeResult);
            // ps is result of minima
            // result_pos is union of sets
        }
        ps = Minima(result_pos, sizeResult, merg, &sizeps);
        k++;
    }

    *level = k;
    for (int i = 0; i < k; i++) {
        sizeArray[i] = sizeMatrix[i];
    }

    printf("k %d \n",k-1);
    for (int i = 0; i < k; i++) {
        printf("%d ",i);
        for (int j = 0; j < sizeMatrix[i]; j++) {
            printf("[%d %d] ", matrix[i][j].i, matrix[i][j].j);
        }
        printf("\n");

    }

    return matrix;
}

//find the lcs
void findpath(char *a, char *b, char *merg) {

    int k = 0;
    int *sizeMatrix = malloc(sizeof(int) * MAX_SIZE);
    struct pos **matrix = Quick_MLCS(a, b, merg, &k, sizeMatrix);
    k--;
    struct pos p;
    p.i = matrix[k][0].i;
    p.j = matrix[k][0].j;

    char lcs[k+1];
    lcs[k+1] = '\0';
    lcs[k]=b[p.j];

    struct pos p1;
    p1.i = p.i - 1;
    p1.j = p.j - 1;

    while (k > 0) {

        k--;
        for (int i = 0; i < sizeMatrix[k]; i++) {
            if ((matrix[k][i].i <= p1.i) && (matrix[k][i].j <= p1.j) && (b[matrix[k][i].j] != '#')) {
                lcs[k]=b[matrix[k][i].j];
                p1.i = matrix[k][i].i - 1;
                p1.j = matrix[k][i].j - 1;
                break;
            }
        }
    }
    printf("Lonest Common Subsquence: %s \n",lcs);
}

int main() {
    // a is first string
    // b is second one
    // merg is used alphabet
    char a[MAX_SIZE], b[MAX_SIZE], merg[MAX_SIZE];

    //getting 2 strings
    printf("Enter the first string\n");
    gets(a);
    strcpy(merg, a);
    printf("Enter the second string\n");
    gets(b);

    //finding used alphabet
    strcat(merg, b);
    removeDuplicates(merg);

    //print the LCS
    findpath(a, b, merg);

    return 0;
}