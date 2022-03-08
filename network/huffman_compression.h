//
// Created by xun on 05/03/2022.
//

#ifndef TORUS_HUFFMAN_COMPRESSION_H
#define TORUS_HUFFMAN_COMPRESSION_H

#include <library/types.h>

#define COMPRESS_TREE_SIZE (256+1)

class HuffmanCompression {
    static const uword_t compression_table[COMPRESS_TREE_SIZE];

    HuffmanCompression(const HuffmanCompression& copy);
    HuffmanCompression& operator=(const HuffmanCompression& other);
public:
    HuffmanCompression() = default;
    static udword_t compress(uint8_t* pOutput, const uint8_t* pInput, udword_t outLen, udword_t inLen);
    //static udword_t compress(uint8_t* pOutput, const t_byte* pInput, unsigned int outLen, unsigned int inLen);

};


#endif //TORUS_HUFFMAN_COMPRESSION_H
