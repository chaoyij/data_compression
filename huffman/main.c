// this is still the draft
// later on i will separate the compress and decompress

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "file.h"
#include "freq_table.h"
#include "huffman_tree.h"
#include "compress.h"

int main(int argc, char *argv[]){

    if (argc == 0){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    }

    // open file
    FILE* fp;
    fp = open_file(argv[1]);

    // read file and create the freq table
    FreqTable* t = create_table();
    fill_table(fp, t);
    
    // print out the table
    print_table_freq(t);
    fprintf(stdout, "\n");


    // create the priority queue and the tree
    NodePQ* pq = create_pq(t);
    Tree* tr = create_tree(t);
    fill_tree(tr, pq);

    // print out the tree
    // post_order_traversal_test(tr);
    // fprintf(stdout, "\n");
    // post_order_traversal_test_no_recursion(tr);
    // fprintf(stdout, "\n");

    // now base on the tree, get the codeword and fill into freq_table
    // freq table has "codeword" in each bucket

    get_codeword(tr, t);
    // print_table_codeword(t);
    // fprintf(stdout, "\n");

    // compress
    char* out_file_name = compress(argv[1], t, tr);
    compress_stats(argv[1], out_file_name);


    fp = close_file(fp);
    t = destroy_table(t);
    pq = destroy_pq(pq);
    tr = destroy_tree(tr);
    
    return 0;
}