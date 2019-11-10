/*
    Author: Philip Chan

    encrypting a string (at most length 20 for hw2) into another string

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw2_crypto.h"

/**
 * This function creates an encrypted string of the word provided
 *
 * word is the string to be encrypted, assume to be at most 20 chars for hw2
 * return the encrypted string
 */

char* encrypt(char word[])
{
  srand(723637564);

  // if the word has fewer than 20 characters, append random printable characters
  char plain_text[21];
  int  word_len = strlen(word);
  int  ascii = (int) word[word_len - 1];  // of the last character

  strncpy(plain_text, word, word_len);
  for (int i=word_len; i < 20; i++)
    {
      plain_text[i] = (char) ((rand() / 2 + ascii) % 94) + 33;
      ascii += rand() % 200;
    }
  plain_text[20] = '\0';

  // generate an encrpted word of length 40 with printable characters
  // each plain character generates two random printable characters
  static char encrypted_word[41];

  ascii = (int) plain_text[19];
  for (int i = 0; i < 20 ; i++)
  {  
    ascii += (int) plain_text[i];

    encrypted_word[i*2] = (char) ((rand() / 2 + ascii) % 94) + 33;
    encrypted_word[i*2+1] = (char) ((rand() / 2 + ascii) % 94) + 33;
  }
  encrypted_word[40] = '\0';

  return encrypted_word;
}

/*
void main(int argc, char* argv[])
{
  printf("%s\n", encrypt("cat"));
  printf("%s\n", encrypt("cats"));
  printf("%s\n", encrypt("acat"));
  printf("%s\n", encrypt("catcat"));
  printf("%s\n", encrypt("at"));
  printf("%s\n", encrypt("dog"));
  printf("%s\n", encrypt("and"));
  printf("%s\n", encrypt("tan"));
  printf("%s\n", encrypt("on"));
  printf("%s\n", encrypt("arr"));
  printf("%s\n", encrypt("ace"));
  printf("%s\n", encrypt("hex"));
}
*/

