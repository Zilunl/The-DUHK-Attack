/* DUKH Attack 
 * COMP10002 Foundations of Algorithms, Semester 1, 2021
 * Skeleton code written by Shaanan Cohney, April 2021
 * 
 * Full Name: Zilun Li      
 * Student Number: 1135273 
 * Date: 26/4/2021           
 */

/****** Include libraries ******/

#include <stdio.h>
#include <stdlib.h>
/* Do NOT use the following two libraries in stage 1! */
#include <string.h>
#include <ctype.h>

/* Provides functions AES_encrypt and AES_decrypt (see the assignment spec) */
#include "aes.h"
/* Provides functions to submit your work for each stage.
 * See the definitions in a1grader.h, they are all available to use.
 * But don't submit your stages more than once... that's weird! */
#include "a1grader.h"

/****** Definitions of constants ******/

#define BOOK_LENGTH 1284         /* The maximum length of a cipher book */
#define MAX_MSG_LENGTH 1024      /* The maximum length of an encrypted message */
#define BLOCKSIZE 16             /* The length of a block (key, output) */
#define N_TIMESTEPS 20           /* number of timesteps */
#define N_OUTPUT_BLOCKS 2        /* number of output blocks */

// TODO Add your own #defines here, if needed
#define O_TIMESTEPS 11 /* timestep compare with next konwn output(011) used in stage3 */


/****** Type definitions ******/
/* Recall that these are merely aliases, or shortcuts to their underlying types.
 * For example, block_t can be used in place of an array, length 16 (BLOCKSIZE)
 * of unsigned char, and vice versa. */

typedef char book_t[BOOK_LENGTH];     /* A cipherbook (1284 bytes) */
typedef unsigned char byte_t;         /* A byte (8 bits) */
typedef byte_t block_t[BLOCKSIZE];    /* A cipher bitset (block) (16 bytes) */
typedef byte_t msg_t[MAX_MSG_LENGTH]; /* An encrypted message (l bytes) */

// TODO Add your own type definitions here, if needed



/****** Function Prototypes ******
 * There are more functions defined in aes.h and grader.h */
// Scaffold

int read_hex_line(byte_t output[], int max_count, char *last_char);

// Hint: Variables passed by pointers should be modified in your stages' implementation!

void stage0(msg_t ciphertext, int *ciphertext_length, 
            block_t outputs[N_OUTPUT_BLOCKS], block_t timesteps[N_TIMESTEPS], 
            book_t cipherbook);
void stage1(book_t cipherbook, int *book_len);
void stage2(byte_t codebook[], int book_len, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], block_t key2);
void stage3(block_t key2, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], byte_t key1[], int cipher_length);
void stage4(byte_t key1[], byte_t ciphertext[], int cipher_length, 
            byte_t plaintext[]);

// TODO: Put your own function prototypes here! Recommended: separate into stages.
void XOR_operation(byte_t num1[], byte_t num2[], byte_t result[]);


/* The main function of the program */
// It is strongly suggested you do NOT modify this function.
int main(int argc, char *argv[])
{   
    //// Stage 0
    /* These will store our input from the input file */
    msg_t ciphertext;                  // encrypted message, to be decrypted in the attack
    int ciphertext_length = 0;         // length of the encrypted message
    book_t cipherbook;                 // book used to make key k2
    block_t timesteps[N_TIMESTEPS];    // timesteps used to generate outputs (hex)
    block_t outputs[N_OUTPUT_BLOCKS];  // outputs from the random number generator (hex)

    // Run your stage 0 code
    stage0(ciphertext, &ciphertext_length, outputs, timesteps, cipherbook);
    // And submit the results.  Don't delete this...
    submit_stage0(ciphertext_length, ciphertext, outputs, timesteps, cipherbook);
    
    //// Stage 1
    int book_len = 0;    // length of the cipher book after having removed punctuation
    stage1(cipherbook, &book_len);
    submit_stage1(cipherbook, book_len);

    //// Stage 2
    block_t key2;        // the key k2 (hexadecimal)
    stage2((byte_t *) cipherbook, book_len, outputs, timesteps, key2);
    submit_stage2(key2);

    //// Stage 3
    byte_t key1[MAX_MSG_LENGTH];       // the key k2 (hexadecimal)
    stage3(key2, outputs, timesteps, key1, ciphertext_length);    
    submit_stage3(key1);

    //// Stage 4
    byte_t plaintext[MAX_MSG_LENGTH];  // the plaintext output
    stage4(key1, ciphertext, ciphertext_length, plaintext);
    submit_stage4(plaintext);

    return 0;
}

/********* Scaffold Functions *********/

/* Reads a line in from stdin, converting pairs of hexadecimal (0-F) chars to
 * byte_t (0-255), storing the result into the output array, 
 * stopping after max_count values are read, or a newline is read.
 *
 * Returns the number of *bytes* read.
 * The last char read in from stdin is stored in the value pointed to by last_char.
 * If you don't need to know what last_char is, set that argument to NULL
 */
int read_hex_line(byte_t output[], int max_count, char *last_char)
{
    char hex[2];
    int count;
    for (count = 0; count < max_count; count++)
    {
        /* Consider the first character of the hex */
        hex[0] = getchar();
        if (hex[0] == '\n')
        {
            if (last_char)
            {
                *last_char = hex[0];
            }
            break;
        }
        /* Now the second */
        hex[1] = getchar();
        if (last_char)
        {
            *last_char = hex[0];
        }
        if (hex[1] == '\n')
        {
            break;
        }

        /* Convert this hex into an int and store it */
        output[count] = hex_to_int(hex); // (defined in aes.h)
    }

    return count - 1;
}

/********* Stage 0 Functions *********/

void stage0(msg_t ciphertext, int *ciphertext_length, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], book_t cipherbook) 
{
    // TODO: Implement stage 0!
    
    /* algorithms are awesome */
    /* Just for fun mentioned in COMP10002 Feedback: XXUSERNAMEXX :) +0.5point*/
    
    /* Store the first line */
    scanf("%d\n", &*ciphertext_length);
    
    /* Read and store the second line from the input file */
    read_hex_line(ciphertext, *ciphertext_length, NULL);
    getchar(); /* Change to the newline, same below */
    
    /* Read and store the third line */
    for (int i=0; i<N_OUTPUT_BLOCKS; i++){
        read_hex_line(outputs[i], BLOCKSIZE, NULL);
    }
    getchar();
    
    /* Read and store the fourth line */
    for (int i=0; i<N_TIMESTEPS; i++){
        read_hex_line(timesteps[i], BLOCKSIZE, NULL);
    }
    getchar();
    
    /* Read and store the fifth line to EOF */
    char c; /* characters in line5 of input file */
    int book_index = 0;
    while ((c=getchar()) != EOF){
        cipherbook[book_index] = c;
        book_index++;
    }
    
    /* !! Submission Instructions !! Store your results in the variables:
     *      ciphertext, ciphertext_length, outputs, timesteps, cipherbook
     * These are passed to submit_stage0 for some useful output and submission. */
}

// TODO: Add functions here, if needed.

/********* Stage 1 Functions *********/
// Reminder: you *cannot* use string.h or ctype.h for this stage!

void stage1(book_t cipherbook, int *book_len) 
{
    // TODO: Implement stage 1!
    
    /* Change characters to ASCII number */
    for (int i=0; i<BOOK_LENGTH; i++){
        int ASCII_num = cipherbook[i];
        
        /* Cheak the characters if letters or not 
           and count the letters*/
        if ((65 <= ASCII_num && ASCII_num <= 90) ||
            (97 <= ASCII_num && ASCII_num <= 122)){
            cipherbook[*book_len] = cipherbook[i];
            *book_len = *book_len+1;
        }
    }
    
    /* !! Submission Instructions !! Store your results in the variables:
     *      cipherbook, book_len
     * These are passed to submit_stage1 for some useful output and submission. */
}

// TODO: Add functions here, if needed.

/********* Stage 2 Functions *********/

void stage2(byte_t codebook[], int book_len, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], block_t key2) 
{
    // TODO: Implement stage 2!
    
    block_t key;
    
    /* Used for the left hand side of the equation */
    block_t Decrypt_O10, Encrypt_T10, XOR_left, left_res;
    
    /* Used for the right hand side of the equation */
    block_t Encrypt_T9, right_res;
    
    /* Store each line to array key */
    for (int book_line=0; book_line<book_len/BLOCKSIZE; book_line++){

        for (int i=0; i<BLOCKSIZE; i++){
            key[i] = codebook[book_line*BLOCKSIZE+i];
        }
        
        /* Solve the left hand of the equation*/
        AES_decrypt(outputs[1], key, Decrypt_O10);
        AES_encrypt(timesteps[10], key, Encrypt_T10);
        XOR_operation(Decrypt_O10, Encrypt_T10, XOR_left);
        AES_decrypt(XOR_left, key, left_res);
         
        /* Solve the right hand of the equation */
        AES_encrypt(timesteps[9], key, Encrypt_T9);
        XOR_operation(outputs[0], Encrypt_T9, right_res);

        /* Chenk if the key satisfies the equation */    
        int count=0;
        for (int i=0; i<BLOCKSIZE; i++){
            /* Add the count number if one element sastisfies */
            if (left_res[i] == right_res[i]){
                count++;
            }
        }
        
        /* Find and store key2 if all pass the situation */
        if (count == BLOCKSIZE){
            for (int i=0; i<BLOCKSIZE; i++){
                key2[i] = key[i];
            }
        }


    }

    /* !! Submission Instructions !! Store your results in the variable:
     *      key2
     * These will be passed to submit_stage2 to let you see some useful output! */
}

// TODO: Add functions here, if needed.
void XOR_operation(block_t num1, block_t num2, block_t result){
    
    for (int i=0; i<BLOCKSIZE; i++){
        result[i] = num1[i] ^ num2[i];
    }
    
}
    
/********* Stage 3 Functions *********/

void stage3(block_t key2, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], byte_t key1[], int ciphertext_length) 
{
    // TODO: Implement stage 3!
    
    /* Find the first V(V10) */
    block_t Encrypt_T10, Encrypt_OT10, V10;
    AES_encrypt(timesteps[10], key2, Encrypt_T10);
    XOR_operation(outputs[1], Encrypt_T10, Encrypt_OT10);
    
    /* Left hand side of the V-equation */
    AES_encrypt(Encrypt_OT10, key2, V10);
    
    /* Find intermediate value and output with steps */
    int txt_line = ciphertext_length/BLOCKSIZE;
    block_t I[txt_line], V[txt_line], O[txt_line];
    block_t Encrypt_IV, Encrypt_OI;
    
    for (int line=0; line<(ciphertext_length/BLOCKSIZE); line++){
        AES_encrypt(timesteps[line+O_TIMESTEPS], key2, I[line]);
        
        /* For the first line use V10 */
        if (line == 0){
            XOR_operation(I[line], V10, Encrypt_IV);
        }
        /* Use V(i-1) for the rest lines */
        else{
            XOR_operation(I[line], V[line-1], Encrypt_IV);
        }
        AES_encrypt(Encrypt_IV, key2, O[line]);
        
        XOR_operation(O[line], I[line], Encrypt_OI);

        AES_encrypt(Encrypt_OI, key2, V[line]);
    }
    
    /* Store all O-output in key1 */
    for (int line=0; line<ciphertext_length/BLOCKSIZE; line++){
        /* Output for each line */
        for (int i=0; i<BLOCKSIZE; i++){
            key1[line*BLOCKSIZE+i] = O[line][i];
        }
    }
    
    /* !! Submission Instructions !! Store your results in the variable:
     *      key1
     * These will be passed to submit_stage3 to let you see some useful output! */
}

// TODO: Add functions here, if needed.

/********* Stage 4 Functions *********/
void stage4(byte_t key1[], byte_t ciphertext[], int cipher_length, byte_t plaintext[])
{
    // TODO: Implement stage 4!
    
    /* Using XOR to produce the original unencrypted message */
    for (int i=0; i<cipher_length; i++){
        plaintext[i] = key1[i] ^ ciphertext[i];
    }
    
    /* !! Submission Instructions !! Store your results in the variable:
     *      plaintext
     * These will be passed to submit_stage4 to let you see some useful output! */
}

// TODO: Add functions here, if needed.

/********* END OF ASSIGNMENT! *********/
/* If you would like to try the bonus stage, attempt it in a new file, bonus.c */
// Feel free to write a comment to the marker or the lecturer below...
