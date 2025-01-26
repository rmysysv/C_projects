#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MAX_N 20


/**
 * @brief Satranç tahtasý üzerindeki çözümü ekrana yazdýrýr.
 * @param rows Satranç tahtasýnda her satýrdaki konumlarýn dizi temsilcisi.
 * @param cols Satranç tahtasýnda her sütundaki konumlarýn dizi temsilcisi.
 * @param n Tahta boyutu (n x n).
 * @return Fonksiyon deðer döndürmez, çözümü ekrana yazdýrýr.
 */
void print_solution(int rows[], int cols[], int n) {
	int i,j;
    printf("Solution:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            if (cols[i] == j) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}
/**
 * @brief Yerleþimin geçerli olup olmadýðýný kontrol eder.
 * @param rows Her satýrdaki kraliçe konumlarý.
 * @param cols Her sütundaki kraliçe konumlarý.
 * @param n Tahta boyutu (n x n).
 * @return Geçerli ise 1, deðilse 0 döner.
 */
int is_valid(int rows[], int cols[], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (rows[i] == rows[j] || 
                cols[i] == cols[j] || 
                abs(rows[i] - rows[j]) == abs(cols[i] - cols[j])) {
                return 0;
            }
        }
    }
    return 1;
}


/**
 * @brief N-Kraliçe problemini brute-force yöntemiyle çözer.
 * @param n Tahta boyutu (n x n).
 * @param total_solutions toplam çözüm sayýsý
 * @param move_count toplam hamle sayýsý
 */

void brute_force_n_queens(int n,int* total_solutions,long long int* move_count) {
    int rows[MAX_N], cols[MAX_N];
    //int total_solutions = 0;
    int positions[MAX_N * MAX_N];
    int indices[MAX_N];
    int i, j;

    for (i = 0; i < n * n; i++) {
        positions[i] = i;
    }
    for (i = 0; i < n; i++) {
        indices[i] = i;
    }

    while (1) {
        (*move_count)++;

        for (i = 0; i < n; i++) {
            rows[i] = indices[i] / n;
            cols[i] = indices[i] % n;
        }

        if (is_valid(rows, cols, n)) {
            total_solutions++;
            (*total_solutions)++;
            print_solution(rows, cols, n);
        }

        int valid = 0;
        for (i = n - 1; i >= 0; i--) {
            if (indices[i] < (n * n - n + i)) {
                indices[i]++;
                for (j = i + 1; j < n; j++) {
                    indices[j] = indices[j - 1] + 1;
                }
                valid = 1;
                i = -1;
            }
        }

        if (!valid) {
            return;
        }
    }

    printf("Brute Force: Total Valid Solutions: %d\n", total_solutions);
    printf("Hamle Sayýsý: %d\n", move_count);
}
/**
 * @brief N-Kraliçe problemini optimize edilmiþ birinci yöntemle çözer.
 * @param n Tahta boyutu (n x n).
 * @param row Þu anki satýr.
 * @param rows Satýrlardaki kraliçe konumlarý.
 * @param cols Sütunlardaki kraliçe konumlarý.
 * @param total_solutions toplam çözüm sayýsý
 * @param move_count toplam hamle sayýsý
 */
void optimized_1_n_queens(int n, int row, int rows[], int cols[], int *total_solutions,long long int* move_count) {
    int col;
    if (row == n) {
        (*move_count)++;
        if (is_valid(rows, cols, n)) {
            (*total_solutions)++;
            print_solution(rows, cols, n);
        }
        return;
    }

    for (col = 0; col < n; col++) {
        rows[row] = row;
        cols[row] = col;
        optimized_1_n_queens(n, row + 1, rows, cols, total_solutions,move_count);
    }
}

/**
 * @brief N-Kraliçe problemini optimize edilmiþ ikinci yöntemle çözer.
 * @param n Tahta boyutu (n x n).
 * @param board Satranç tahtasý durumu.
 * @param row Þu anki satýr.
 * @param rows Satýrlardaki kraliçe konumlarý.
 * @param cols Sütunlardaki kraliçe konumlarý.
 * @param total_solutions toplam çözüm sayýsý
 * @param move_count toplam hamle sayýsý
 */
void optimized_2_n_queens(int n, int board[MAX_N][MAX_N], int row, int rows[], int cols[], int *total_solutions,long long int* move_count) {
    int col, prev_row;
    if (row == n) {
        (*move_count)++;
        if (is_valid(rows, cols, n)) {
            (*total_solutions)++;
            print_solution(rows, cols, n);
        }
        return;
    }

    for (col = 0; col < n; col++) {
        int valid = 1;
        for (prev_row = 0; prev_row < row; prev_row++) {
            if (board[prev_row][col] == 1) {
                valid = 0;
            }
        }

        if (valid == 1) {
            board[row][col] = 1;
            rows[row] = row;
            cols[row] = col;

            optimized_2_n_queens(n, board, row + 1, rows, cols, total_solutions,move_count);

            board[row][col] = 0;
        }
    }
}

/**
 * @brief Backtracking yönteminde kontrol saðlar
 * @param board Satranç tahtasýndaki kraliçelerin konumlarý.
 * @param row Þu anki satýr.
 * @param col Þu anki sütun
 * @return Geçerli çözüm bulunursa 1, aksi halde 0.
 */
int is_safe(int board[], int row, int col) {
    int i;
    for (i = 0; i < row; i++) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return 0;
        }
    }
    return 1;
}
/**
 * @brief N-Kraliçe problemini backtracking yöntemiyle çözer.
 * @param board Satranç tahtasýndaki kraliçelerin konumlarý.
 * @param row Þu anki satýr.
 * @param n Tahta boyutu (n x n).
 * @oaram total_solutions toplam çözüm sayýsý
 * @param move_count toplam hamle sayýsý
 * @return Geçerli çözüm bulunursa 1, aksi halde 0.
 */
int backtracking_n_queens(int board[], int row, int n,int* total_solutions,long long int* move_count) {
    int col;
    if (row >= n) {
        (*total_solutions)++;
        print_solution(board, board, n);
        return 1;
    }

    int success = 0;
    for (col = 0; col < n; col++) {
        if (is_safe(board, row, col)) {
            board[row] = col;
            success = backtracking_n_queens(board, row + 1, n,total_solutions,move_count) || success;
            (*move_count)++;
        }
    }
    return success;
}



int main() {
    char mods[5][30] = {
        "1. BRUTE_FORCE",
        "2. OPTIMIZED_1",
        "3. OPTIMIZED_2",
        "4. BACK_TRACKING",
        "5. COMPARISON_OF_4_APPROACHES"
    };

    int mod, i;
    printf("N QUEEN PROBLEM SOLUTION\n");
    printf("Please choose a mode to be performed:\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", mods[i]);
    }
    scanf("%d", &mod);

    int n;
    printf("Enter the number of queens (n): ");
    scanf("%d", &n);

    if (n > MAX_N) {
        printf("n cannot be greater than %d\n", MAX_N);
        return 1;
    }

    clock_t start, end;
    double time_spent;
	int total_solutions = 0;
	long long int move_count=0;
    switch (mod) {
        case 1:
        	
            start = clock();
            brute_force_n_queens(n, &total_solutions,&move_count);
            end = clock();
            time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Brute Force Execution Time: %.2f seconds\n", time_spent);
            printf("Optimized 1: Total Valid Solutions: %d\n", total_solutions);
            printf("Moves: %lld\n", move_count);
            break;
        case 2: {
           
            int* rows=(int*)malloc(MAX_N*sizeof(int));
            int* cols=(int*)malloc(MAX_N*sizeof(int));
    
        	total_solutions = 0;
            start = clock();
            optimized_1_n_queens(n, 0, rows, cols, &total_solutions,&move_count);
            end = clock();
            time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Optimized 1: Total Valid Solutions: %d\n", total_solutions);
            printf("Moves: %lld\n", move_count);
            printf("Optimized 1 Execution Time: %.2f seconds\n", time_spent);
            break;
        }
        case 3: {
        	int i,j;
            int* rows=(int*)malloc(MAX_N*sizeof(int));
            int* cols=(int*)malloc(MAX_N*sizeof(int));
            int** board=(int**)malloc(MAX_N*sizeof(int*));
            for(i=0;i<MAX_N;i++){
            	board[i]=(int*)malloc(MAX_N*sizeof(int));
			}
			for(i=0;i<MAX_N;i++){
				for(j=0;j<MAX_N;j++){
					board[i][j]=0;
				}
			}
            //int board[MAX_N][MAX_N] = {0};
            total_solutions = 0;
            start = clock();
            optimized_2_n_queens(n, board, 0, rows, cols, &total_solutions,&move_count);
            end = clock();
            time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Optimized 2: Total Valid Solutions: %d\n", total_solutions);
            printf("Moves: %lld\n", move_count);
            printf("Optimized 2 Execution Time: %.2f seconds\n", time_spent);
            break;
        }
        case 4: {
            int* board=(int*)malloc(MAX_N*sizeof(int));
            for (i = 0; i < n; i++) {
                board[i] = -1;
            }
            start = clock();
            if (!backtracking_n_queens(board, 0, n,&total_solutions,&move_count)) {
                printf("No solutions found.\n");
            }
            end = clock();
            time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Backtracking: Total Valid Solutions: %d\n", total_solutions);
            printf("Moves: %lld\n", move_count);
            printf("Backtracking Execution Time: %.2f seconds\n", time_spent);
            break;
        }
        case 5:
            printf("Comparison mode.\n");
            double times[4];
            int rows[MAX_N];
            int cols[MAX_N];
            int board[MAX_N][MAX_N] = {0};
            int total_solutions = 0;

            // Brute Force
            start = clock();
            brute_force_n_queens(n,&total_solutions,&move_count);
            end = clock();
            times[0] = (double)(end - start) / CLOCKS_PER_SEC;

            // Optimized 1
            start = clock();
            optimized_1_n_queens(n, 0, rows, cols, &total_solutions,&move_count);
            end = clock();
            times[1] = (double)(end - start) / CLOCKS_PER_SEC;

            // Optimized 2
            start = clock();
            optimized_2_n_queens(n, board, 0, rows, cols, &total_solutions,&move_count);
            end = clock();
            times[2] = (double)(end - start) / CLOCKS_PER_SEC;

            // Backtracking
            start = clock();
            if (!backtracking_n_queens(board, 0, n,&total_solutions,&move_count)) {
                printf("No solutions found.\n");
            }
            end = clock();
            times[3] = (double)(end - start) / CLOCKS_PER_SEC;

            // Tablo Yazdýrma
            printf("\nComparison Table:\n");
            printf("Method\t\t\tExecution Time (s)\n");
            printf("Brute Force\t\t%.6f\n", times[0]);
            printf("Optimized 1\t\t%.6f\n", times[1]);
            printf("Optimized 2\t\t%.6f\n", times[2]);
            printf("Backtracking\t\t%.6f\n", times[3]);
            break;
        default:
            printf("Invalid mode selected.\n");
    }

    return 0;
}


