/*



  Author: Jared Blanco
  Email: JBlanco2018@my.fit.edu
  Course: CSE 2010
  Section: 03

  Description:

    To run program: gcc *.c -o hw3 && ./hw3 hw2in2.txt


    Program will scan a list of encrypted strings into a linked list.

    Than recursivly gernerats all combinations of lower case letter words in the 'word_Generator' function,
      will compare every generated word with the linked list of encrypted_words until we have found all
      unencrypted versions of these strings using string comparison.

    Finally will print the list of unencrypted words

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hw2_crypto.h"

//data type for my linked list of scanned in encrpted words
typedef struct encrypted_t{
  //scaned in from the file
  char encrypted_string[60];
  //once unencrypted we will cpy to word into this section.
  char unencrypted_string[60];
  //going to make this flag a 1 if we have unencrypted the string
  int flag;
  //linked list
  struct encrypted_t *next;
}encrypted_t;


//all functions and Descriptions are under main for organizational purposes
int scan_encrypted(encrypted_t **list, char *file_name);
void add_node(encrypted_t **list, char enencrypted_input[60]);
int compare_encrypted(encrypted_t **list, char generated_word[50]);
void print_list(encrypted_t* list);
void freeList(encrypted_t** head);
void word_Generator(int max_length, char generated_word[max_length], int current_length, int next_letter, encrypted_t **list);
/* --------------------------- */
int main(int argc, char* argv[]){


  //The string is declared at mazimum length, but we need string length of the current word for the 'word gernerator' function to be able to insert new letters properly
  int current_length = 0;

  //starting decimal value of the lowercase letter 'a' from the ascii table
  int decimal_value = 97;

  //the list of of encrypted words
  encrypted_t *list = NULL;

  //max length for a password is given from our input, we +1 to make room for our NULL char
  int max_length = (scan_encrypted(&list, argv[1]) + 1);

  //string that we will pass into our 'word gernerator' function.
  char generated_word[max_length];

  /*
  Generates ever possible words combination, than encrypts it, than compares it with all input to find all passwords.
  function takes in an empty array, with a 0 current lenth, 97 is the letter 'a' in ASCII and than a poitner to my list of passwords to crack  */
  word_Generator(max_length, generated_word, current_length, 97, &list);

  //prints list of unencrypted passwords
  print_list(list);

  //free's list that was created from scanning in passwords
  freeList(&list);


}



/* --------------------------- */
//Recisive way of generating every combination of lower case letters, than calls the 'compare_encrypted' function
void word_Generator(int max_length, char generated_word[max_length], int current_length, int next_letter, encrypted_t **list){
  //this will be flase, endless 'compare_encrypted' function says all of the passwords have been cracked
  int all_passwords_cracked = 0;

  //first base case: we hit the maximum number of characters a password would be
  if(max_length - 1 == current_length){
    //max length -1 beucase we made max length one grater than input to make room for the NULL char
    return;
  }

  //second base case:  we are inserting letters from a - z, and the 'next_letters' value has passed z on the ascii table
  if(next_letter > 122){
    return;
  }


  //adds the reqested letter into the propper position, followed by adding a null char to the end to maintain our string
  generated_word[current_length] = next_letter;
  generated_word[current_length + 1] = '\0';


  //we send in every word we generate, this function will encrypt our word than compare it to input. If all password are cracked after this, 'all_passwords_cracked' will return true
  all_passwords_cracked = compare_encrypted(&(*list), generated_word);

  //last base case: all passwords have been cracked
  if(all_passwords_cracked){
    return;
  }
  printf("%s\n", generated_word);


  //will create a new leaf with the current work (IE: zzz) and make it (zzza) where 97 is the decimal value of lower case a
  word_Generator(max_length, generated_word, (current_length + 1), (97), &(*list));

  //this will change the last char of the current sting for the next comparison. IE: (zzza) to (zzzb)
  word_Generator(max_length, generated_word, (current_length), (next_letter + 1), &(*list));
}

/* --------------------------- */
//this will take the gerated word and compare it with ever word in the linked list, the linked list contains the encrypted input. Than if we have the correct answer we add it to the list
//lastly, this function retunrs a booleans expression stating 'if all of the passwords have been cracked'
int compare_encrypted(encrypted_t **list, char generated_word[50]){
  //we compare all of out flags inside of out linked list, if all of our passwords are decrypted than our flag will stay true
  int flag_reader = 1;

  char encrypted_string[50];
  encrypted_t *temp = *list;

  //this will take the generated word and encrypt it for comparison
  strcpy(encrypted_string, encrypt(generated_word));

  //will go through the entire linked lsit of encrypted_words and compare by geerated word to it
  while(temp != NULL){

    //if my encrypted_word is the same as one taken in from input, I add the unencrypted_string to my linked list
    if(strcmp(encrypted_string, temp->encrypted_string) == 0){

      //puts the origional word(before encryting) into the correct spot in my linked list
      strcpy(temp->unencrypted_string, generated_word);

      //this flag wil say 'we have the unencrypted_string'
      temp->flag = 1;
    }

    //if a word has yet to be decrypted, this will stay flase
    if(temp->flag == 0){
      flag_reader = 0;
    }

    //scrolls through the rest of my list
    temp = temp->next;
  }

  return flag_reader;
}

/* --------------------------- */
//scanns all lines of code from inut into a linked list and returns the max_char's a password can be
int scan_encrypted(encrypted_t **list, char *file_name){
  char temp_encrypted[51];

  //read in by the first line of the file, max char's of a password
  int max_char;

  //opens the file
  FILE *fin = fopen(file_name, "r");


  //the first line of the program is the ammout of charectes.
  fgets(temp_encrypted, 50, fin);
  //convert the read in string to an int using 'atoi'
  max_char = atoi(temp_encrypted);

  //the while loops boolean condition is 'fgets' beucase fgets returns NULL at EOF
  while(fgets(temp_encrypted, 50, fin)){
    //removes \n char that gets scanned in with fgets
    temp_encrypted[40] = '\0';

    //adds the scanned in string to the linked list
    add_node(&(*list), temp_encrypted);
  }


  fclose(fin);
  //max chars that a password can be from input
  return max_char;
}

/* --------------------------- */
//takes in the encrypted string from 'scan_encrypted' function and adds it to my linked list of words
void add_node(encrypted_t **list, char enencrypted_input[60]){
  //temp that points to head of list
  encrypted_t *temp = *list;


  //declaring my new_node and allocating it memory
  encrypted_t *new_node = NULL;
  new_node = (encrypted_t*)malloc(sizeof(encrypted_t));

  //adds the scanned in line to my linked list of encryted words
  strcpy(new_node->encrypted_string, enencrypted_input);
  //just a safty, if i didnt decode this word than i print: NOT FOUND at the end of the program
  strcpy(new_node->unencrypted_string, "NOT FOUND");
  //set's all of my flags to flase, 1 is true and flag is flipped if we decode the string
  new_node->flag = 0;
  new_node->next = NULL;


  //bellow just connects the node to my list

  //if no list, we add node to the front
  if(temp == NULL){
    *list = new_node;
  }else{
    //scroll to the end and add a node
    while(temp->next != NULL){
      temp = temp->next;
    }

    temp->next = new_node;
  }



}

/* --------------------------- */
//recursive way of printing the list, prints a node, than calls funciton again with the next node
void print_list(encrypted_t* list){

  //base case: if there is no list, do nothing
  if(list == NULL){
    return;
  }

  //prints the unencrypted_string
  printf("%s\n", list->unencrypted_string);

  //send the next node to print
  print_list(list->next);
}

/* --------------------------- */
//recursive way to free my linked list
void freeList(encrypted_t** head){
  //base case: if we have no list, don't do anything
  if(*head == NULL){
    return;
  }
  //calls the same function with the next node
  freeList(&(*head)->next);

  //free's the current node
  free(*head);

}
