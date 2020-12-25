#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
//#include "Application_wallet_btc.h"

typedef struct
{
    uint8_t previous_txn_hash[32];
    uint8_t previous_output_index[4];
    uint8_t script_length[1];
    uint8_t script_public_key[25];
    uint8_t sequence[4];
} unsigned_txn_input;

typedef struct
{
    uint8_t value[8];
    uint8_t script_length[1];
    uint8_t script_public_key[25];
} txn_output;

typedef struct
{
    uint8_t network_version[4];
    uint8_t input_count[1];
    unsigned_txn_input* input;
    uint8_t output_count[1];
    txn_output* output;
    uint8_t locktime[4];
    uint8_t sighash[4];
} unsigned_txn;

uint8_t hextodec(char hex[])
{
    int len = strlen(hex);
    uint8_t result= 0;
    uint8_t base = 1;

    for (int i=len-1; i>=0; i--)
    {
        if( hex[i]>='0' && hex[i]<='9')
            result += base * (hex[i] - 48);
        else if( hex[i]>='a' && hex[i]<='f')
            result += base * (hex[i] - 87);

        base = base*16;  
    }
    return result;
}

int main()
{
    const char *received_unsigned_txn_string = "0200000001748dccb662fd73e8f0d8435132b8528dd3739f55388a15795c7e7afe4f555f9f010000001976a9140ce400ffe51ab038f6134beeb14ef56c683ce00088acfdffffff02204e0000000000001976a914d46d05e6ac27683aa5d63a6efc44969798acf13688ac28b30000000000001976a914dacc24d8b195ce046a40caedd5e2e649beee4e3388ac49211a0001000000";
    int byte_arr_size = strlen(received_unsigned_txn_string) / 2;
    uint8_t unsigned_txn_bytearr[byte_arr_size]; 

    for (int i=0, j=0; i<byte_arr_size; i++, j+=2)
    {
        char hex[3] = "";
        hex[0] = received_unsigned_txn_string[j];
        hex[1] = received_unsigned_txn_string[j+1];
        unsigned_txn_bytearr[i] = (uint8_t)hextodec(hex);
        //printf("%d %d \n", i, unsigned_txn_bytearr[i]);
    }
    unsigned_txn *transaction = (unsigned_txn*)malloc(sizeof(unsigned_txn));

    //For network_version and input_count
    memcpy(transaction, unsigned_txn_bytearr, 5*sizeof(uint8_t));   

    //For input structure
    uint8_t ipcnt = transaction->input_count[0];
    transaction->input = (unsigned_txn_input*)malloc( ipcnt * sizeof(unsigned_txn_input) ); //Memory allocation
    memcpy(transaction->input, &unsigned_txn_bytearr[5], 66*ipcnt);

    //For output_count
    memcpy(transaction->output_count, &unsigned_txn_bytearr[5 + 66*ipcnt], 1);

    //For output structure
    uint8_t opcnt = transaction->output_count[0];   
    transaction->output = (txn_output*)malloc( opcnt * sizeof(txn_output) );    //Memory allocation
    memcpy(transaction->output, &unsigned_txn_bytearr[5 + 66*ipcnt + 1], 34*(opcnt) ); 

    //For locktime and sighash
    memcpy(transaction->locktime,&unsigned_txn_bytearr[5 + 66*ipcnt + 1 + 34*opcnt], 4);
    memcpy(transaction->sighash,&unsigned_txn_bytearr[5 + 66*ipcnt + 1 + 34*opcnt + 4], 4);

}   //main ends here
