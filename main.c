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

    uint8_t ipcnt = transaction->input_count[0];
    transaction->input = (unsigned_txn_input*)malloc( ipcnt * sizeof(unsigned_txn_input) ); //Memory allocation
    int ip=0;

    for (ip=0; ip<ipcnt; ip++)
        memcpy(transaction->input, &unsigned_txn_bytearr[5 + ip*66], 66);

    //For output_count
    memcpy(transaction->output_count, &unsigned_txn_bytearr[67+ (ip-1)*66 + 4], 1);

    uint8_t opcnt = transaction->output_count[0];   
    transaction->output = (txn_output*)malloc( opcnt * sizeof(txn_output) );    //Memory allocation
    int op=0;

    //For output transaction
    for (op=0; op<opcnt; op++)
        memcpy(transaction->input, &unsigned_txn_bytearr[72 + (ip-1)*66 + op*34], 34);

    //For locktime and sighash
    memcpy(transaction->locktime,&unsigned_txn_bytearr[106 + (ip-1)*66 + (op-1)*34], 4);
    memcpy(transaction->sighash,&unsigned_txn_bytearr[110 + (ip-1)*66 + (op-1)*34], 4);


    // for(int i=0; i<4; i++)
    //     printf("%d ",transaction->locktime[i]);

}   //main ends here


    // memcpy(transaction->network_version, unsigned_txn_bytearr, 4);
    // memcpy(transaction->input_count, &unsigned_txn_bytearr[4], 1);
    // memcpy(transaction->input->previous_txn_hash, &unsigned_txn_bytearr[5], 32);
    // memcpy(transaction->input->previous_output_index, &unsigned_txn_bytearr[37], 4);
    // memcpy(transaction->input->script_length, &unsigned_txn_bytearr[41], 1);
    // memcpy(transaction->input->script_public_key, &unsigned_txn_bytearr[42], 25);
    // memcpy(transaction->input->sequence, &unsigned_txn_bytearr[67], 4);
    // memcpy(transaction->output_count, &unsigned_txn_bytearr[71], 1);
    // memcpy(transaction->output->value, &unsigned_txn_bytearr[72], 8);
    // memcpy(transaction->output->script_length, &unsigned_txn_bytearr[80], 1);
    // memcpy(transaction->output->script_public_key,&unsigned_txn_bytearr[81], 25);
    // memcpy(transaction->locktime,&unsigned_txn_bytearr[106], 4);
    // memcpy(transaction->sighash,&unsigned_txn_bytearr[110], 4);
