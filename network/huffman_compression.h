//
// Created by xun on 05/03/2022.
//

#ifndef TORUS_HUFFMAN_COMPRESSION_H
#define TORUS_HUFFMAN_COMPRESSION_H

#include <library/types.h>

#define COMPRESS_TREE_SIZE (256+1)

class HuffmanCompression {
    static const word_t compression_table[COMPRESS_TREE_SIZE];

    HuffmanCompression(const HuffmanCompression& copy);
    HuffmanCompression& operator=(const HuffmanCompression& other);
public:
    HuffmanCompression() = default;
    static udword_t compress(t_byte* pOutput, const t_byte* pInput, udword_t outLen, udword_t inLen);

};


#endif //TORUS_HUFFMAN_COMPRESSION_H
