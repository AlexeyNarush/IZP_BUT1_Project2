#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct select {
    int start_row;
    int start_col;
    int end_row;
    int end_col;
};

void get_size_matrix(int arr[], char DELIM, char *filename)
//function for receive data from stdin and putting it into the 3D matrix
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
                max_sym = l;
            }
        }
        else if (ch == DELIM)
        {
            c++;
            if (c >= max_col)
            {
                max_col = c;
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
//function for printing the data from 3D matrix to stdout
//(rows and collumns[words are printed in print_value function]) 
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
        if((mat[i][0][0] != '\0' || mat[i][1][0] != '\0' || mat[i][2][0] != '\0') && mat[i][0][0] != EOF)
        {
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}

void read_file(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM, char *filename)
//function for receive data from stdin and putting it into the 3D matrix
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
// function to copy the exact data from matix "mat_from" to matrix "mat_to"
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
//function for cleaning(eraising) the given matrix 
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
//function made for testing of the sertain part of the matrix
//Was made becouse of diference in new line characte in Linux and Windows 
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
void select (char *data, char selected_zone[4], int rows, int cols)
{
    int s = 0;
    int d = 0;

    if (strcmp(data,"[min]") == 0){    
        if (selected_zone[0] != '_'){
            if(selected_zone[2] <= selected_zone[0] && selected_zone[2] != '\0'){
                selected_zone[0] = selected_zone[2];
            }
        }
        else
        {
            selected_zone[0] = '1';
        }
        
        if (selected_zone[1] != '_'){
            if(selected_zone[3] <= selected_zone[1]){
                selected_zone[1] = selected_zone[3];
            }
        }
        else
        {
            selected_zone[1] = '1';
        }
    }
    else if (strcmp(data,"[max]") == 0){
        if (selected_zone[2] != '_'){
            if(selected_zone[2] >= selected_zone[0] && selected_zone[2] != '\0'){
                selected_zone[0] = selected_zone[2];
            }
        }
        else
        {
            selected_zone[0] = rows + '0';
        }
        
        if (selected_zone[1] != '_'){
            if(selected_zone[3] >= selected_zone[1]){
                selected_zone[1] = selected_zone[3];
            }
        }
        else
        {
            selected_zone[1] = cols - 1 + '0';
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

struct select SELECT(char selected_zone[4], int rows, int cols)
{
    struct select sel;

    if (selected_zone[0] >= '1' && selected_zone[0] <= '9'){
        sel.start_row = selected_zone[0] - '1';
    }
    else if(selected_zone[0] == '_'){
        sel.start_row = 0;
        sel.end_row = rows - 1;
    }

    if (selected_zone[1] >= '1' && selected_zone[1] <= '9'){
        sel.start_col= selected_zone[1] - '1';
    }
    else if(selected_zone[1] == '_'){
        sel.start_col= 0;
        sel.end_col = cols - 1;
    }

    if (selected_zone[2] >= '1' && selected_zone[2] <= '9'){
        sel.end_row = selected_zone[2] - '1';
    }
    else if(selected_zone[2] == '_'){
        sel.end_row = rows - 1;
    }
    else{
        if (sel.end_row != rows-1){
            sel.end_row = sel.start_row;
        }
    }
    
    if (selected_zone[3] >= '1' && selected_zone[3] <= '9'){
        sel.end_col = selected_zone[3] - '1';
    }
    else if(selected_zone[3] == '_'){
        sel.end_col = cols - 1;
    }
    else{
        if (sel.end_col != cols-1){
            sel.end_col = sel.start_col;
        }
    }

    return sel;
}

void find (char *data, char selected_zone[4], int rows, int cols, int symbols, char mat[rows][cols][symbols])
{
    char* sub_string = malloc(sizeof(data));
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
               break;
            }
        }
    }
    free(sub_string);
}

//Příkazy pro úpravu struktury tabulky
void irow (int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
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

void arow(int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM, char selected_zone[4])
{
    struct select selected = SELECT(selected_zone, rows, cols);

    for (int i = rows - 1; i < selected.end_row; i--)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int d = 0; d < symbols; d++)
            {
                mat[i][j][d] = mat[i - 1][j][d];
                mat[i][j - 1][d] = '\0';
            }
            mat[i][j][0] = DELIM;
        }
    }
}

void drow(int rows, int cols, int symbols, char mat[rows][cols][symbols])
{
    char new_mat[rows][cols][symbols];
    clear_mat(rows, cols, symbols, new_mat);
    int is_shifted = 0;

    for (int i = 0; i < rows - 1; i++)
    {
        // if (i + 1 == R)
        // {
        //     is_shifted = 1;
        //     continue;
        // }
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
{
    for (int d = 0; d <= rows; d++)
    {
        mat[d][cols][0] = DELIM;
    }
}

void dcol(int rows, int cols, int symbols, char mat[rows][cols][symbols], int C)
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

//Zpracování dat
void set(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *STR, char selected_zone[4])
{
    struct select selected = SELECT(selected_zone, rows, cols);

    for (int i = selected.start_row; i <= selected.end_row; i++){
        for (int j = selected.start_col; j <= selected.end_col; j++){
            for (int d = 0; d <= symbols; d++){
                mat[i][j][d] = '\0'; 
            }  
            strcpy(mat[i][j], STR);
        }
    }
}

void clear(int rows, int cols, int symbols, char mat[rows][cols][symbols], char selected_zone[4])
{
    struct select selected = SELECT(selected_zone, rows, cols);

    for (int i = selected.start_row; i <= selected.end_row; i++){
        for (int j = selected.start_col; j <= selected.end_col; j++){
            for (int d = 0; d < symbols; d++){
                mat[i][j][d] = '\0'; 
            }  
        }
    }
}

void swap(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *selecting, char selected_zone[4])
{
    int col = 0;
    int row = 0;

    struct select selected = SELECT(selected_zone, rows, cols);

    if (selecting[1] >= '1' && selecting[1] <= '9'){
        row = selecting[1] - '1';
    }
    else if(selecting[1] == '_'){
        row = rows;
    }

    if (selecting[3] >= '1' && selecting[3] <= '9'){
        col= selecting[3] - '1';
    }
    else if(selecting[3] == '_'){
        col= cols;
    }
    
    char *new_arr = malloc(symbols);
    strcpy(new_arr, mat[row][col]);
    strcpy(mat[row][col], mat[selected.start_row][selected.start_col]);
    strcpy(mat[selected.start_row][selected.start_col], new_arr);
    free(new_arr);

}

void sum(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *selecting, char selected_zone[4])
{
    
    int col = 0;
    int row = 0;
    if (selecting[1] >= '1' && selecting[1] <= '9'){
        row = selecting[1] - '1';
    }
    else if(selecting[1] == '_'){
        row = rows;
    }

    if (selecting[3] >= '1' && selecting[3] <= '9'){
        col= selecting[3] - '1';
    }
    else if(selecting[3] == '_'){
        col= cols;
    } 

    struct select selected = SELECT(selected_zone, rows, cols);

    float to_be_sumed;
    float final_summ = 0;
    char *summ_of_floats = malloc(symbols);

    for (int i = selected.start_row; i <= selected.end_row; i++){
        for (int j = selected.start_col; j <= selected.end_col; j++){
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

void avg(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *selecting, char selected_zone[4])
{
    int col = 0;
    int row = 0;

    struct select selected = SELECT(selected_zone, rows, cols);

    if (selecting[1] >= '1' && selecting[1] <= '9'){
        row = selecting[1] - '1';
    }
    else if(selecting[1] == '_'){
        row = rows;
    }

    if (selecting[3] >= '1' && selecting[3] <= '9'){
        col= selecting[3] - '1';
    }
    else if(selecting[3] == '_'){
        col= cols;
    } 

    float to_be_sumed;
    float final_summ = 0;
    char *summ_of_floats = malloc(symbols);
    int elem_counter = 0;

    for (int i = selected.start_row; i <= selected.end_row; i++){
        for (int j = selected.start_col; j <= selected.end_col; j++){
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

void count(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *selecting, char selected_zone[4])
{
    int col = 0;
    int row = 0;

    struct select selected = SELECT(selected_zone, rows, cols);

    if (selecting[1] >= '1' && selecting[1] <= '9'){
        row = selecting[1] - '1';
    }
    else if(selecting[1] == '_'){
        row = rows;
    }

    if (selecting[3] >= '1' && selecting[3] <= '9'){
        col= selecting[3] - '1';
    }
    else if(selecting[3] == '_'){
        col= cols;
    } 

    char *summ_of_elements = malloc(symbols);
    int elem_counter = 0;

    for (int i = selected.start_row; i <= selected.end_row; i++){
        for (int j = selected.start_col; j <= selected.end_col; j++){
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

void len(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *selecting, char selected_zone[4])
{
    int col = 0;
    int row = 0;
    int length;

    struct select selected = SELECT(selected_zone, rows, cols);

    if (selecting[1] >= '1' && selecting[1] <= '9'){
        row = selecting[1] - '1';
    }
    else if(selecting[1] == '_'){
        row = rows;
    }

    if (selecting[3] >= '1' && selecting[3] <= '9'){
        col= selecting[3] - '1';
    }
    else if(selecting[3] == '_'){
        col= cols;
    } 

    char *length_of_str = malloc(symbols);

    for (int d = 0; d <= symbols; d++){
        mat[row][col][d] = '\0';   
    }

    length = strlen(mat[selected.start_row][selected.start_col]);
    sprintf(length_of_str, "%d", length);
    strcpy(mat[row][col], length_of_str);
    free(length_of_str);
}

//Příkazy pro práci s dočasnými proměnnými
void def_tem(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4], char **temp_v)
{
    int _some = position[1];
    int elem_row = selected[0];
    int elem_col = selected[1];
    strcpy(temp_v[_some], mat[elem_row][elem_col]);
}

void use_tem(int rows, int cols, int symbols, char mat[rows][cols][symbols], char *position, char selected[4], char **temp_v)
{
    int _some = position[1];
    int elem_row = selected[0];
    int elem_col = selected[1];
    strcpy(mat[elem_row][elem_col], temp_v[_some]);
}

void inc_tem(char *position, char **temp_v)
{
    int _some = position[1];
    if (temp_v[_some][0] >= '1' && temp_v[_some][0] <= '9'){
        temp_v[_some][0] += 1;
    }
}


// Reading the arguments & executing functions 
void read_args(int argc, char** argv, int rows, int cols, int symbols, char mat[rows][cols][symbols], char DELIM)
//Get arguments of functions that need to be executed
{
    int arg_sym_counter = 1;
    int arg_number = 1;
    while (argv[argc - 2][arg_sym_counter] != '\0'){
        if (argv[argc - 2][arg_sym_counter] == ';' || argv[argc - 2][arg_sym_counter] == '\x20'){
            arg_number++;
        }
        arg_sym_counter++;
    }
    char *sequence = (char *) malloc(arg_sym_counter);
    strcpy(sequence, argv[argc - 2]);
    int l = 0, d = 0;

    char **arguments = (char **) calloc(arg_number, sizeof(char *)); 
    for (int i = 0; i < arg_number; i++){ 
        arguments[i] = (char *) calloc(arg_sym_counter, sizeof(char));
    }

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
    char **temp_variab = (char **) calloc(12, sizeof(char *)); 
    for (int i = 0; i < 12; i++){ 
        temp_variab[i] = (char *) calloc(symbols, sizeof(char));
    }
    char *selected_zone =(char*) malloc(4);
    int error = 1;
    for (int i = 0; i < arg_number; i++){
        //Úprava tabulky

        if (arguments[i][0] == '['){
            select(arguments[i], selected_zone, rows, cols);
            error = 0;
        }

        if (strcmp(arguments[i],"[find") == 0){
            find(arguments[i + 1], selected_zone, rows, cols, symbols, mat);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"irow") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            irow(rows, cols, symbols, mat, DELIM);
            error = 0;
        }

        if (strcmp(arguments[i],"arow") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            arow(rows, cols, symbols, mat, DELIM, selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"drow") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            drow(rows, cols, symbols, mat);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"icol") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            icol(rows, cols, symbols, mat, DELIM, selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"acol") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            acol(rows, cols, symbols, mat, DELIM);
            error = 0;
        }

        if (strcmp(arguments[i],"dcol") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            dcol(rows, cols, symbols, mat, atoi(arguments[i + 1]));
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"set") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            set(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"clear") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            clear(rows, cols, symbols, mat, selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"swap") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            swap(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"sum") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            sum(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"avg") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            avg(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"count") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            count(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"len") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            len(rows, cols, symbols, mat, arguments[i + 1], selected_zone);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"def") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            def_tem(rows, cols, symbols, mat, arguments[i + 1], selected_zone, temp_variab);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"use") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            use_tem(rows, cols, symbols, mat, arguments[i + 1], selected_zone, temp_variab);
            error = 0;
            i++;
        }

        if (strcmp(arguments[i],"inc") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            inc_tem(arguments[i + 1], temp_variab);
            error = 0;
            i++;
        }
        
        if (strcmp(arguments[i],"[set]") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            strcpy(temp_variab[11], selected_zone);
            error = 0;
        }

        if (strcmp(arguments[i],"[_]") == 0){           
            //irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
            strcpy(selected_zone, temp_variab[11]);
            error = 0;
        }

        if (error == 1)
        {
            fprintf(stderr, "An unexpected command was entered in command sequense!\n");
            free(sequence);
            for (int i = 0; i < arg_number; i++){ 
                 free(arguments[i]);
            }
            free(arguments);
            free(selected_zone);
            for (int i = 0; i < 12; i++){ 
                free(temp_variab[i]);
            }
            free(temp_variab);
            exit(0);
        }
        
    }  


    free(sequence);
    for (int i = 0; i < arg_number; i++){ 
        free(arguments[i]);
    }
    free(arguments);
    free(selected_zone); 
    for (int i = 0; i < 12; i++){ 
        free(temp_variab[i]);
    }
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
            break;
        }
        else{
            DELIM = '\x20';  // '\x20' - space symbol 
        }
    }
    
    get_size_matrix(arr, DELIM, filename);
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
    //dump(rows, cols, symbols, mat);
    write_file(rows, cols, symbols, mat, DELIM, filename);

    return 0;
}

//Made a function SELECT that operate with the structure to send selected zone to functions to work with,
//In some of the functions there is stil some ammount of code that selects the "row" and "col"
//This code in function selects the place that was given not by selection but as argument of the function 
