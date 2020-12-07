#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void get_size_matrix(int arr[], char DELIM, char *filename)
//Function for count rows, cols and symbols and sending the value to main to make matrix of proper size
{
    FILE * fp;
    fp = fopen(filename,"r");

    int r = 1;
    int c = 1;
    int l = 0;
    int max_col = 0;
    int max_sym = 0;
    char ch;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch != DELIM && ch != '\0' && ch != EOF && ch != '\n' && ch != '\r')
        {
            l++;
            if (l >= max_sym)
            {
                max_sym = l; //Variable for taking the maximum count of symbols 
            }
        }
        else if (ch == DELIM)
        {
            c++;
            if (c >= max_col)
            {
                max_col = c; //Variable for taking the maximum count of collumns 
            }
            l = 0;
        }
        else if (ch == '\n' || ch == '\r')
        {
            r++;
            l = 0;
            c = 1;
        }
    }

    arr[0] = r + 1;
    arr[1] = max_col + 1;
    arr[2] = max_sym + 1;

    fclose(fp);
}

void write_file(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM, char *filename)
//Function for printing the data from 3D matrix to file
{
    FILE * fp;
    fp = fopen(filename,"w");
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols - 1; j++)
        {
            for (int d = 0; d < symbols; d++)
            {
                if (mat[i][j][d] != '\0')
                {
                    fprintf(fp, "%c", mat[i][j][d]);
                }
            }
            if(mat[i][0][0] != '\0' && j != cols - 1 && mat[i][j][0] != DELIM)
            {
                fprintf(fp, "%c", DELIM);
            }
        }
        if(mat[i + 1][0][0] != '\0' && mat[i + 1][0][0] != EOF)
        {
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}

void read_file(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM, char *filename)
//Function for receive data from file and putting it into the 3D matrix
{
    FILE * fp;
    fp = fopen(filename,"r");

    int r = 0;
    int c = 0;
    int l = 0;
    char ch;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch != DELIM && ch != EOF && ch != '\n' && ch != '\r')
        {
            mat[r][c][l] = ch;
            l++;
        }
        else if (ch == DELIM)
        {
            c++;
            l = 0;
        }
        else if (ch == '\n' || ch == '\r')
        {
            r++;
            c = 0;
            l = 0;
        }
    }

    fclose(fp);
}

void copy_mat(int rows, int cols, int symbols, char mat_to[rows][cols][symbols], char mat_from[rows][cols][symbols])
//Function to copy the exact data from matix "mat_from" to matrix "mat_to"
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            for (int d = 0; d < 100; d++)
            {
                if (mat_from[i][j][d] != '\0')
                {
                    mat_to[i][j][d] = mat_from[i][j][d];
                }
            }
        }
    }
}

void clear_mat(int rows, int cols, int symbols, char mat[rows][cols][symbols])
//Function for cleaning(eraising) the given matrix 
{
    for (int i = 0; i <= rows; i++)
    {
        for (int j = 0; j <= cols; j++)
        {
            for (int d = 0; d <= symbols; d++)
            {
                mat[i][j][d] = '\0';
            }
        }
    }
}

void dump (int rows, int cols, int symbols, char mat[rows][cols][symbols])
//Function for testing of the positioning of every element in the matrix
{
    for (int i = 0; i < rows;i++)
    {
        for (int j = 0; j < cols;j++)
        {
            for (int d = 0; d < symbols; d++)
            {
               printf("(x%d, y%d, z%d = %c)\n", i + 1, j + 1, d + 1, mat[i][j][d]);
            }
        }
    }
}

//Příkazy pro změnu výběru
void select (char *data, char selected_zone[4])
//Výběr buněk
{
    int s = 0;
    int d = 0;
    int select_count = 1;
    while (data[s] != ']'){
        if (data[s] != ','){
            select_count++;
        }
        s++;
    }
    s = 0;

    if (strcmp(data,"[min]") == 0){
        if (selected_zone[1] == '_'){
            selected_zone[1] = '1';
        }
        else if (selected_zone[2] != '\0'){
            selected_zone[2] = '\0';
            selected_zone[3] = '\0';
        }  
    }
    else if (strcmp(data,"[max]") == 0){
        if (selected_zone[2] != '\0' && selected_zone[3] != '\0'){
            selected_zone[0] = selected_zone[2];
            selected_zone[1] = selected_zone[3];
            selected_zone[2] = '\0';
            selected_zone[3] = '\0';
        } 
    }
    else
    {
        while (data[s] != ']')
        {
            if (data[s] != ',' && data[s] != ']' && data[s] != '[')
            {
                selected_zone[d] = data[s];
                d++;
            }
            s++;
        }  
    }
}

void find (char *data, char selected_zone[4], int rows, int cols, int symbols, char mat[rows][cols][symbols])
//[find STR] - v již existujícím výběru buněk vybere první buňku, jejíž hodnota obsahuje podřetězec STR.
{
    char *sub_string = malloc(strlen(data) + 1);
    int s = 0;
    while (data[s] != ']')
    {
        sub_string[s] = data[s];
        s++;
    }  

    for (int i = 0; i < rows;i++){
        for (int j = 0; j < cols;j++){
            if (strstr(mat[i][j], sub_string)){
               selected_zone[0] = i;
               selected_zone[1] = j;
               selected_zone[2] = '\0';
               selected_zone[3] = '\0';
               break;
            }
        }
    }
    free(sub_string);
}

//Příkazy pro úpravu struktury tabulky
void irow (int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
//irow - vloží jeden prázdný řádek nad vybrané buňky.
{
    for (int i = rows + 1; i >= 0; i--)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int d = 0; d < symbols; d++)
            {
                mat[i + 1][j][d] = mat[i][j][d];
                mat[i][j][d] = '\0';
            }
            mat[i][j][0] = DELIM;
        }
    }
}

void arow(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
//arow - přidá jeden prázdný řádek pod vybrané buňky.
{
    for (int i = rows - 1; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int d = 0; d < symbols; d++)
            {
                mat[i][j][0] = DELIM;
            }
        }
    }
}

void drow(int rows, int cols, int symbols, char mat[rows][cols][symbols], int R)
//drow - odstraní vybrané řádky.
{
    char new_mat[rows][cols][symbols];
    clear_mat(rows, cols, symbols, new_mat);
    int is_shifted = 0;

    for (int i = 0; i < rows; i++)
    {
        if (i + 1 == R)
        {
            is_shifted = 1;
            continue;
        }
        for (int j = 0; j < cols; j++)
        {
            for (int d = 0; d < symbols; d++)
            {
               new_mat[i][j][d] = mat[i + is_shifted][j][d];
            }
        }
    }
    clear_mat(rows, cols, symbols, mat);
    copy_mat(rows, cols, symbols, mat,new_mat);
}

void icol(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM, char selected[4])
//icol - vloží jeden prázdný sloupec nalevo od vybraných buněk.
{
    int start_col = 0;

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = cols + 1; j > start_col; j--)
        {
            for (int d = 0; d < symbols; d++)
            {
                mat[i][j][d] = mat[i][j - 1][d];
                mat[i][j - 1][d] = '\0';
            }
            mat[i][j][0] = DELIM;
        }
    }
}

void acol(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
//acol - přidá jeden prázdný sloupec napravo od vybraných buněk.
{
    for (int d = 0; d <= rows; d++)
    {
        mat[d][cols][0] = DELIM;
    }
}

void dcol(int rows, int cols, int symbols, char mat[rows][cols][symbols], int C)
//dcol - odstraní vybrané sloupce.
{
    char new_mat[rows][cols][symbols];
    clear_mat(rows, cols, symbols, new_mat);

    for (int i = 0; i < rows; i++)
    {
        int is_shift = 0;
        for (int j = 0; j < cols - 1; j++)
        {
            if (j == C - 1)
            {
                is_shift = 1;
            }
            for (int d = 0; d < symbols; d++)
            {
                new_mat[i][j][d] = mat[i][j + is_shift][d];
                
            }
        }
    }
    clear_mat(rows, cols, symbols, mat);
    copy_mat(rows, cols, symbols, mat,new_mat);

}

//Příkazy pro úpravu obsahu buněk
void set(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *STR, char selected[4])
//set STR - nastaví hodnotu buňky na řetězec STR. Řetězec STR může být ohraničen uvozovkami a může 
//obsahovat speciální znaky uvozené lomítkem (viz formát tabulky)
{
    int start_row = 0;
    int start_col = 0;
    int end_row = 0;
    int end_col = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = 0;
        end_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col = selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col = 0;
        end_col = cols;
    }

    if (selected[2] >= '1' && selected[2] <= '9'){
        end_row = selected[2] - '1';
    }
    else if(selected[2] == '_'){
        end_row = rows;
    }
    else if(selected[0] != '_' && selected[2] == '\0'){
        end_row = start_row;
    }
    
    if (selected[3] >= '1' && selected[3] <= '9'){
        end_col = selected[3] - '1';
    }
    else if(selected[3] == '_'){
        end_col = cols;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_col = start_col;
    }

    for (int i = start_row; i <= end_row; i++){
        for (int j = start_col; j <= end_col; j++){
            for (int d = 0; d <= symbols; d++){
                mat[i][j][d] = '\0'; 
            }  
            strcpy(mat[i][j], STR);
        }
    }
}

void clear(int rows, int cols, int symbols, char mat[rows][cols][symbols], char selected[4])
//clear - obsah vybraných buněk bude odstraněn (buňky budou mít prázdný obsah)
{
    int start_row = 0;
    int start_col = 0;
    int end_row = 0;
    int end_col = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (selected[2] >= '1' && selected[2] <= '9'){
        end_row = selected[2] - '1';
    }
    else if(selected[2] == '_'){
        end_row = rows;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_row = start_row;
    }

    if (selected[3] >= '1' && selected[3] <= '9'){
        end_col = selected[3] - '1';
    }
    else if(selected[3] == '_'){
        end_col = cols;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_col = start_col;
    }
    

    for (int i = start_row; i <= end_row; i++){
        for (int j = start_col; j <= end_col; j++){
            for (int d = 0; d <= symbols; d++){
                mat[i][j][d] = '\0'; 
            }  
        }
    }
}

void swap(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4])
//swap [R,C] - vymění obsah vybrané buňky s buňkou na řádku R a sloupci C
{
    int start_row = 0;
    int start_col = 0;
    int col = 0;
    int row = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (position[1] >= '1' && position[1] <= '9'){
        row = position[1] - '1';
    }
    else if(position[1] == '_'){
        row = rows;
    }

    if (position[3] >= '1' && position[3] <= '9'){
        col= position[3] - '1';
    }
    else if(position[3] == '_'){
        col= cols;
    }
    
    char *new_arr = malloc(symbols);
    strcpy(new_arr, mat[row][col]);
    strcpy(mat[row][col], mat[start_row][start_col]);
    strcpy(mat[start_row][start_col], new_arr);
    free(new_arr);
}

void sum(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4])
//sum [R,C] - do buňky na řádku R a sloupci C uloží součet hodnot vybraných buněk (odpovídající formátu %g u printf). 
//Vybrané buňky neobsahující číslo budou ignorovány (jako by vybrány nebyly).
{
    int start_row = 0;
    int start_col = 0;
    int col = 0;
    int row = 0;
    int end_row = 0;
    int end_col = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (selected[2] >= '1' && selected[2] <= '9'){
        end_row = selected[2] - '1';
    }
    else if(selected[2] == '_'){
        end_row = rows;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_row = start_row;
    }

    if (selected[3] >= '1' && selected[3] <= '9'){
        end_col = selected[3] - '1';
    }
    else if(selected[3] == '_'){
        end_col = cols;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_col = start_col;
    }

    if (position[1] >= '1' && position[1] <= '9'){
        row = position[1] - '1';
    }
    else if(position[1] == '_'){
        row = rows;
    }

    if (position[3] >= '1' && position[3] <= '9'){
        col= position[3] - '1';
    }
    else if(position[3] == '_'){
        col= cols;
    } 

    float to_be_sumed;
    float final_summ = 0;
    char *summ_of_floats = malloc(symbols);

    for (int i = start_row; i <= end_row; i++){
        for (int j = start_col; j <= end_col; j++){
            if (strtof(mat[i][j], NULL)){
                to_be_sumed = strtof(mat[i][j], NULL);
                final_summ += to_be_sumed;
            }
        }
    }
    for (int d = 0; d <= symbols; d++){
        mat[row][col][d] = '\0';   
    }

    sprintf(summ_of_floats, "%g", final_summ);
    strcpy(mat[row][col], summ_of_floats);
    free(summ_of_floats);
}

void avg(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4])
//avg [R,C] - stejné jako sum, ale ukládá se aritmetický průměr z vybraných buněk
{
    int start_row = 0;
    int start_col = 0;
    int col = 0;
    int row = 0;
    int end_row = 0;
    int end_col = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (selected[2] >= '1' && selected[2] <= '9'){
        end_row = selected[2] - '1';
    }
    else if(selected[2] == '_'){
        end_row = rows;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_row = start_row;
    }

    if (selected[3] >= '1' && selected[3] <= '9'){
        end_col = selected[3] - '1';
    }
    else if(selected[3] == '_'){
        end_col = cols;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_col = start_col;
    }

    if (position[1] >= '1' && position[1] <= '9'){
        row = position[1] - '1';
    }
    else if(position[1] == '_'){
        row = rows;
    }

    if (position[3] >= '1' && position[3] <= '9'){
        col= position[3] - '1';
    }
    else if(position[3] == '_'){
        col= cols;
    } 

    float to_be_sumed;
    float final_summ = 0;
    char *summ_of_floats = malloc(symbols);
    int elem_counter = 0;

    for (int i = start_row; i <= end_row; i++){
        for (int j = start_col; j <= end_col; j++){
            if (strtof(mat[i][j], NULL)){
                to_be_sumed = strtof(mat[i][j], NULL);
                final_summ += to_be_sumed;
                elem_counter++;
            }
        }
    }
    for (int d = 0; d <= symbols; d++){
        mat[row][col][d] = '\0';   
    }

    sprintf(summ_of_floats, "%g", (final_summ/elem_counter));
    strcpy(mat[row][col], summ_of_floats);
    free(summ_of_floats);
}

void count(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4])
//count [R,C] - stejné jako sum, ale ukládá se počet neprázdných buněk z vybraných buněk
{
    int start_row = 0;
    int start_col = 0;
    int col = 0;
    int row = 0;
    int end_row = 0;
    int end_col = 0;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (selected[2] >= '1' && selected[2] <= '9'){
        end_row = selected[2] - '1';
    }
    else if(selected[2] == '_'){
        end_row = rows;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_row = start_row;
    }

    if (selected[3] >= '1' && selected[3] <= '9'){
        end_col = selected[3] - '1';
    }
    else if(selected[3] == '_'){
        end_col = cols;
    }
    else if(selected[1] != '_' && selected[3] == '\0'){
        end_col = start_col;
    }

    if (position[1] >= '1' && position[1] <= '9'){
        row = position[1] - '1';
    }
    else if(position[1] == '_'){
        row = rows;
    }

    if (position[3] >= '1' && position[3] <= '9'){
        col= position[3] - '1';
    }
    else if(position[3] == '_'){
        col= cols;
    } 

    char *summ_of_elements = malloc(symbols);
    int elem_counter = 0;

    for (int i = start_row; i <= end_row; i++){
        for (int j = start_col; j <= end_col; j++){
            if (mat[i][j][0] != '\0'){
                elem_counter++;
            }
        }
    }

    for (int d = 0; d <= symbols; d++){
        mat[row][col][d] = '\0';   
    }

    sprintf(summ_of_elements, "%d", elem_counter);
    strcpy(mat[row][col], summ_of_elements);
    free(summ_of_elements);
}

void len(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4])
//len [R,C] - do buňky na řádku R a sloupci C uloží délku řetězce aktuálně vybrané buňky
{
    int start_row = 0;
    int start_col = 0;
    int col = 0;
    int row = 0;
    int length;

    if (selected[0] >= '1' && selected[0] <= '9'){
        start_row = selected[0] - '1';
    }
    else if(selected[0] == '_'){
        start_row = rows;
    }

    if (selected[1] >= '1' && selected[1] <= '9'){
        start_col= selected[1] - '1';
    }
    else if(selected[1] == '_'){
        start_col= cols;
    }

    if (position[1] >= '1' && position[1] <= '9'){
        row = position[1] - '1';
    }
    else if(position[1] == '_'){
        row = rows;
    }

    if (position[3] >= '1' && position[3] <= '9'){
        col= position[3] - '1';
    }
    else if(position[3] == '_'){
        col= cols;
    } 

    char *length_of_str = malloc(symbols);

    for (int d = 0; d <= symbols; d++){
        mat[row][col][d] = '\0';   
    }

    length = strlen(mat[start_row][start_col]);
    sprintf(length_of_str, "%d", length);
    strcpy(mat[row][col], length_of_str);
    free(length_of_str);
}

//Příkazy pro práci s dočasnými proměnnými
void def_tem(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4], char **temp_v)
//def _X - hodnota aktuální buňky bude nastavena do dočasné proměnné X (kde X může být hodnota 0 až 9)
{
    int variab = position[1];
    int row = selected[0];
    int col = selected[1];
    strcpy(temp_v[variab], mat[row][col]);
}

void use_tem(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4], char **temp_v)
//use _X - aktuální buňka bude nastavena na hodnotu z dočasné proměnné X 
//(kde X identifikuje dočasnou proměnnou _0 až _9)
{
    int variab = position[1];
    int row = selected[0];
    int col = selected[1];
    strcpy(mat[row][col], temp_v[variab]);
}

void inc_tem(char *position, char **temp_v)
//inc _X - numerická hodnota v dočasné proměnné bude zvětšena o 1. 
//Pokud dočasná proměnná neobsahuje číslo, bude výsledná hodnota proměnné nastavená na 1.
{
    int variab = position[1];
    if (temp_v[variab][0] >= '1' && temp_v[variab][0] <= '9'){
        temp_v[variab][0] += 1;
    }
}

// Reading the arguments & executing functions 
void read_args(int argc, char** argv, int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
{
    int arg_sym_counter = 1;
    int arg_number = 1;
    while (argv[argc - 2][arg_sym_counter] != '\0'){
        if (argv[argc - 2][arg_sym_counter] == ';' || argv[argc - 2][arg_sym_counter] == '\x20'){
            arg_number++; //Getting the number of separate argumens in command sequence
        }
        arg_sym_counter++; //Getting a number of symbols of command sequence
    }
    char *sequence = (char *) malloc(arg_sym_counter); 
    strcpy(sequence, argv[argc - 2]); 

    //Making a matrix to separate function calls passed as one command sequece
    char **arguments = (char **) calloc(arg_number, sizeof(char *)); 
    for (int i = 0; i < arg_number; i++){ 
        arguments[i] = (char *) calloc(arg_sym_counter, sizeof(char)); 
    }
    
    int l = 0, d = 0;
    //Passing a separate function calls to matrix
    for (int i = 0; i < arg_sym_counter; i++){
        if (sequence[i] != ';' && sequence[i] != '\x20'){
            arguments[l][d] = sequence[i];
            d++;
        }
        else{
            l++;
            d = 0;
        }
    }
    //Making a matrix to hold a temp variables _0 to _9 and _
    char **temp_variab = (char **) malloc(12); 
    for (int i = 0; i < 12; i++){ 
        temp_variab[i] = (char *) malloc(symbols);
    }
    //Making an array to keep a selected zone of matrix
    char *selected_zone = (char*) malloc(4);
    //Get arguments of functions that need to be executed
    for (int i = 0; i < arg_number; i++){
        //Příkazy pro úpravu obsahu buněk
        int error = 1;

        if (strcmp(arguments[i],"[find") == 0){
            //[find STR] - v již existujícím výběru buněk vybere první buňku,
            //jejíž hodnota obsahuje podřetězec STR.
            find(arguments[i + 1], selected_zone, rows, cols, symbols, mat);
            error = 0;
            i++;
        }

        else if (arguments[i][0] == '['){
            //Změna výběru
            select(arguments[i], selected_zone);
            error = 0;
        }

        //Příkazy pro úpravu struktury tabulky
        if (strcmp(arguments[i],"irow") == 0){           
            //irow - vloží jeden prázdný řádek nad vybrané buňky.
            irow(rows, cols, symbols, mat, DELIM);
            error = 0;
        }

        if (strcmp(arguments[i],"arow") == 0){           
            //arow - přidá jeden prázdný řádek pod vybrané buňky.
            arow(rows, cols, symbols, mat, DELIM);
            error = 0;
        }

        if (strcmp(arguments[i],"drow") == 0){           
            //drow - odstraní vybrané řádky.
            drow(rows, cols, symbols, mat, atoi(arguments[i + 1]));
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"icol") == 0){           
            //icol - vloží jeden prázdný sloupec nalevo od vybraných buněk.
            icol(rows, cols, symbols, mat, DELIM, selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"acol") == 0){           
            //acol - přidá jeden prázdný sloupec napravo od vybraných buněk.
            acol(rows, cols, symbols, mat, DELIM);
            error = 0;
        }

        if (strcmp(arguments[i],"dcol") == 0){           
            //dcol - odstraní vybrané sloupce.
            dcol(rows, cols, symbols, mat, atoi(arguments[i + 1]));
            error = 0;
            i++;
        }

        //Příkazy pro úpravu obsahu buněk
        if (strcmp(arguments[i],"set") == 0){           
            //set STR - nastaví hodnotu buňky na řetězec STR. 
            //Řetězec STR může být ohraničen uvozovkami a může obsahovat speciální 
            //znaky uvozené lomítkem (viz formát tabulky)
            set(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"clear") == 0){           
            //clear - obsah vybraných buněk bude odstraněn (buňky budou mít prázdný obsah)
            clear(rows, cols, symbols, mat, selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"swap") == 0){           
            //swap [R,C] - vymění obsah vybrané buňky s buňkou na řádku R a sloupci C
            swap(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"sum") == 0){           
            //sum [R,C] - do buňky na řádku R a sloupci C uloží součet hodnot 
            //vybraných buněk (odpovídající formátu %g u printf). 
            //Vybrané buňky neobsahující číslo budou ignorovány (jako by vybrány nebyly).
            sum(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"avg") == 0){           
            //avg [R,C] - stejné jako sum, ale ukládá se aritmetický průměr 
            //z vybraných buněk
            avg(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"count") == 0){           
            //count [R,C] - stejné jako sum, ale ukládá se počet neprázdných buněk 
            //z vybraných buněk
            count(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"len") == 0){           
            //len [R,C] - do buňky na řádku R a sloupci C uloží délku řetězce aktuálně 
            //vybrané buňky
            len(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        //Příkazy pro práci s dočasnými proměnnými
        if (strcmp(arguments[i],"def") == 0){           
            //def _X - hodnota aktuální buňky bude nastavena do dočasné proměnné X 
            //(kde X může být hodnota 0 až 9)
            def_tem(rows, cols, symbols, mat, arguments[i + 1], selected_zone, temp_variab);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"use") == 0){           
            //use _X - aktuální buňka bude nastavena na hodnotu z dočasné proměnné X 
            //(kde X identifikuje dočasnou proměnnou _0 až _9)
            use_tem(rows, cols, symbols, mat, arguments[i + 1], selected_zone, temp_variab);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"inc") == 0){           
            //inc _X - numerická hodnota v dočasné proměnné bude zvětšena o 1. 
            //Pokud dočasná proměnná neobsahuje číslo, bude výsledná hodnota proměnné 
            //nastavená na 1.
            inc_tem(arguments[i + 1], temp_variab);
            error = 0;
            i++;
        }
        
        if (strcmp(arguments[i],"[set]") == 0){           
            //[set] - nastaví aktuální výběr buněk do dočasné proměnné _ 
            //(ukládá se pouze, které buňky jsou vybrány, nikoliv jejich obsah)
            strcpy(temp_variab[11], selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"[_]") == 0){           
            //[_] - obnoví výběr z dočasné proměnné (viz níže).
            strcpy(selected_zone, temp_variab[11]);
            error = 0;
        }

        //ERROR - command
        if (error == 1)
        {
            //if none of arguments is found, stop the program and print an error 
            fprintf(stderr, "An unexpected command was entered in command sequense!\n");
            fprintf(stderr, "%s\n", arguments[i]);
            free(sequence);
            free(arguments);
            free(selected_zone);
            free(temp_variab);
            exit(0);
        }
    }  

    //Free the memory after reading of arguments and executing of functions is done
    free(sequence); 
    free(arguments);
    free(selected_zone);
    free(temp_variab);
}

// Main file
int main(int argc, char* argv[])
{
    char * filename = argv[argc - 1];
    char DELIM;
    int rows;
    int cols;
    int symbols;
    int arr[3];
    //Set the delimiter DELIM to entered value or space if not entered
    for (int i = 0; i < argc; ++i){
        if (strcmp(argv[i],"-d") == 0){
            //Taking  the DELIM value 
            DELIM = argv[i + 1][0];
            break; // "break;" for DELIM to not be overriden with space symbol
        }
        else{
            //If DELIm is not entered
            DELIM = '\x20';  // '\x20' - space symbol 
        }
    }
    
    get_size_matrix(arr, DELIM, filename);
    //Error if input file is empty
    if (arr[0] == 1 && arr[1] == 1 && arr[2] == 1){
        fprintf(stderr, "Input file cannot be empty!");
        exit(0);
    }

    rows = arr[0];
    cols = arr[1];
    symbols = arr[2];
    char mat[rows][cols][symbols]; 
    clear_mat(rows, cols, symbols, mat);  
    read_file(rows, cols, symbols, mat, DELIM, filename); 
    read_args(argc, argv, rows, cols, symbols, mat, DELIM); 
    //dump(rows, cols, symbols, mat);   // Printing exery element of the matrix and its pozition in it to console
    write_file(rows, cols, symbols, mat, DELIM, filename); 

    return 0;
}