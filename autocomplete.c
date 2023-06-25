#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "autocomplete.h"

int length(char *str){
    int i=0 ;
    while(str[i] != '\0'){
        //printf("i: %d character: %c\n", i, str[i]);
        i++;
    }
    return i;
}

int sort_lexico(const void *p_a, const void *p_b){
    term *p_to_a = (term *)p_a;
    term *p_to_b = (term *)p_b;
    //int diff;
    return(strncmp(p_to_a->term, p_to_b->term, 200));
    /*
    for (int i = 0; i < 200; i++){
        // printf("%d th character in p_to_a->term: %d", i, (int)p_to_a->term[i]);
        // printf("%d th character in p_to_b->term: %d", i, (int)p_to_b->term[i]);
        int char_diff = (int)p_to_a->term[i] - (int)p_to_b->term[i];
        // printf("char_diff: %d\n", char_diff);
        if (char_diff > 0){
            // if a is greater than b, char_diff >0
            return 1;
        }
        else if (char_diff < 0){
            // if b is greater than a, char_diff < 0
            return -1;
        }
        else{
            // if a is equal to b, index up
            ;
        }
    }
    */
}

int char_to_int (char *string, int *weight){
    //printf("length of string: %d\n", length(string));
    //printf("string: %s\n", string);
    int length_of_string = length(string);
    
    for(int i = 0; i < length_of_string; i++){
        //printf("i: %d, string[i]: %c\n", i, string[i]);
        if (isdigit(string[i])){
            string[i]= string[i] - '0';
            //printf("i: %d, string[i]: %d\n", i, string[i]);
            //new_int = new_int * 10 + string[i];
            //printf("weight: %d\n", *weight);
            *weight = *weight * 10 + string[i];
            //printf("weight: %d\n", *weight);
        }
        else{
            ;
        }
    }
    //printf("weight: %d\n", *weight);
    return *weight;
}



void read_in_terms(term **terms, int *pnterms, char *filename){
    FILE *fp = fopen(filename, "r"); //read the first line of the file
    char line[200]; //defining an array where I can read stuff in
    fgets(line, 200, fp); //storing a maximum of 200 characters(included the null character) from fp into line
    line[strlen(line) - 1] = '\0'; //replace the newline with a null so it is digits with a null
    *pnterms = atoi(line); 
    * terms = (term*) malloc((*pnterms) * sizeof(term)); 
    for (int j = 0; j < *pnterms ; j++){
        //printf("new starting now\n");
        //char saved_j_char[190]= "";
        fgets(line, 200, fp); 
        int weight = 0;

        char *weight_char = strtok(line,"\t");
        //printf("the weight as a char: %s\n", weight_char);
        weight = char_to_int(weight_char, &weight);
        //printf("the weight as an int: %d\n", weight);
        (*terms)[j].weight = weight;
        //printf("(*terms)[j].weight: %.0f\n", (*terms)[j].weight);
        char *term_to_save = strtok(NULL,"\n");
        //printf("the term: %s\n", term_to_save);
        strcpy((*terms)[j].term, term_to_save);
        //strcpy((*terms)[j].term, city_name);
        //printf("(*terms)[j].term: %s\n", (*terms)[j].term);
    }
    //printf("\nbefore sorting\n");

    /*
    for (int k = 0; k < 15; k++){
        printf("%s %.0f\n", (*terms)[k].term, (*terms)[k].weight);
    }
    */

    qsort(*terms, *pnterms, sizeof(term), sort_lexico);

    //printf("\nafter sorting\n");
    /*
    for (int k = 0; k < 12; k++){
        printf("%s %.0f\n", (*terms)[k].term, (*terms)[k].weight);
    }
    */
    //printf("\nterms: %d\n", *pnterms);
    
}

///*
int lowest_match(struct term *terms, int nterms, char *substr){
    int lower = 0 ;
    int upper = nterms - 1;
    int len_substr = length(substr);

    //print statements
    //printf("\nLowest Match\n");
    //printf("Substr: %s\n", substr);
    //printf("length of substr: %d\n", len_substr);
    //printf("\n");
    /*
    for (int f = 0; f < len_substr; f++){
        printf("%c\n", substr[f]);
    }
    */
    //print statements

    while(lower <= upper){
        int mid = (lower + upper) / 2;
        //printf("lower: %d, term: %s\n", lower, (terms)[lower].term);
        //printf("mid: %d, term: %s\n", mid, (terms)[mid].term);
        //printf("upper: %d, term: %s\n", upper, (terms)[upper].term);
        //printf("\n");
        //char lower_term[200]= "";
        if (strncmp((terms)[mid].term , substr, len_substr) < 0){
            lower = mid + 1;
            //lower = mid ;
            //char *lower_term= (terms)[mid].term;

            //strcpy(lower_term, (terms)[lower].term); 
            //printf("lower term: %s\n", lower_term);
        }
        else if (strncmp((terms)[mid].term , substr, len_substr) >= 0){
            upper = mid - 1 ;
            //upper = mid  ;
            //char *upper_term = (terms)[mid].term;
        }
    }

    //printf("LOWER index: %d\n", lower);
    //printf("match: %s\n", (terms)[lower].term );
    //printf("\n");

    return lower;
}
//*/

///*
int highest_match(struct term *terms, int nterms, char *substr){
    //this is like binary_search_first
    //with strncmp
    int lower = 0 ;
    int upper = nterms - 1;
    
    int len_substr = length(substr);
    //printf("\nHighest Match\n");
    //printf("Substr: %s\n", substr);
    //printf("length of substr: %d\n", len_substr);


    while(lower <= upper){
        int mid = (lower + upper) / 2;
        if (strncmp((terms)[mid].term , substr, len_substr) > 0){
            upper = mid - 1;
        }
        else if (strncmp((terms)[mid].term , substr, len_substr) <= 0){
            lower = mid + 1 ;
        }
    //printf("index: %d\n", lower);
    //printf("match: %s\n", (terms)[lower].term );
    //return lower;
    }

    //printf("UPPER index: %d\n", upper);
    //printf("match: %s\n", (terms)[upper].term );

    return upper;
    //return 0;
}
//*/

int sort_weight(const void *p_1 , const void *p_2){
    //sort by city name
    term *p_to_1 = (term *)p_1;
    term *p_to_2 = (term *)p_2;
    int diff;
    //printf("1st weight: %.0f\n" ,(*p_to_1).weight);
    //printf("2nd weight: %.0f\n" ,(*p_to_2).weight);

    diff = (*p_to_1).weight - (*p_to_2).weight;
    if (diff <0){
        return 1;
    }
    else if (diff ==0){
        return 0;
    }
    else{
        return -1;
    }
    //printf("difference: %d\n" ,diff);
    //negative if 1<2
    //0 if they are equal 
    //positive if 1>2
    //i switched the -1 and the +1 for non-increasing order
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    //printf("\nSearch Term: %s\n", substr);
    int lowest = lowest_match(terms, nterms, substr);
    //printf("lowest index: %d\n", lowest);
    int highest = highest_match(terms, nterms, substr);
    //printf("highest index: %d\n", highest);
    *n_answer = (highest - lowest)+1 ;
    //printf("n_answer: %d\n", *n_answer);
    /*
    for (int k = 84000; k < 84300; k++)
        printf("%s %.0f\n", (*terms)[k].term, (*terms)[k].weight);
    */
   //allocating memory
    * answer = (term*) malloc((*n_answer) * sizeof(term)); //find out why it is only one star
    ///*
    int i = 0;
    //char saved_k_char[190]= "";
    //int new_weight = 0;
    for (int k = lowest; k < (highest + 1); k++){
        //int i = 0;
        //printf("%s %.0f\n", (terms)[k].term, (terms)[k].weight);
        //strcpy(saved_k_char,(terms)[k].term );
        strcpy((*answer)[i].term, (terms)[k].term );
        //printf("(*answer)[i].term: %s\n", (*answer)[i].term);
        //new_weight = new_weight + (terms)[k].weight;
        (*answer)[i].weight = (terms)[k].weight;
        //printf("(*answer)[i].weight %f\n", (*answer)[i].weight);
        //printf("i: %d\n", i);
        i++;
    }
    //*/

    //printf("OTHER LOOP NOW\n"); 
    /*
    for (int m = 0; m < 3 ; m++){
        printf("%s %.0f\n", (*answer)[m].term, (*answer)[m].weight);
    }
    */
    //printf("answer[0]: %.0f\n", (*answer)[0].weight);
    //printf("answer[2]: %.0f\n", (*answer)[2].weight);
    //int test1 = sort_weight(&((*answer)[0]), &((*answer)[1]));

    //printf("test: %d\n", test1);
    qsort(*answer, *n_answer, sizeof(term), sort_weight);
    /*
    for (int m = 0; m < *n_answer ; m++){
        printf("%s %.0f\n", (*answer)[m].term, (*answer)[m].weight);
        //printf("%s\n", (*answer)[m].term);
    }
    */
}


/*
int main(void){
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "c2.txt");
    //lowest_match(terms, nterms, "Tor");
    //highest_match(terms, nterms, "Tor");
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "İstanbul");
    //free allocated blocks here -- not required for the project, but good practice
    return 0;
}
*/