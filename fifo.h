/*!
   \@file Fifo.h
   \@brief header file for spi fifo buffer
   \@author Jason Berger
   \@date 05/03/2017
*/

#pragma once

#include <stdint.h>

#define FIFO_OK 0
#define FIFO_OVERFLOW -1
#define FIFO_UNDERFLOW -2
/**
 *@brief creates fifo
 *@param x : name of fifo buffer
 *@param y : max number of elements in buffer
 *@param z : datatype
 *@return "Return of the function"
 */
#define FIFO_DEF(x,y,z)             \
  z x##_data[y];    \
  fifo_t x = {                      \
        .mBuffer = x##_data,        \
        .mHead = 0,                 \
        .mTail = 0,                 \
        .mMaxLen = y,               \
        .mCount = 0,                \
        .mObjSize = sizeof(z)	      \
    };

typedef struct {
    uint8_t *  mBuffer; //pointer to data buffer
    int mHead;      //next address to write to
    int mTail;      //next address to read from
    int mCount;     //number of items in fifo
    int mObjSize;   //size of obj in buffer
    int mMaxLen;
    uint8_t mLock;     //lock for fifo that is accessed by multiple threads
} fifo_t;


fifo_t* new_fifo( int objSize, int len);
void fifo_init(fifo_t* pFifo, int depth, int width);
void fifo_destroy(fifo_t* pFifo);
int fifo_push(fifo_t* pFifo, void* data);
int fifo_pop(fifo_t* pFifo, void* data);
int fifo_push_buf(fifo_t* pFifo, void* data, int len);
int fifo_pop_buf(fifo_t* pFifo, void* data, int len);
int fifo_clear(fifo_t* pFifo, int len);
int fifo_peek(fifo_t* pFifo, void* data, int idx);
int fifo_peek_buf(fifo_t* pFifo, void* data, int len);
int fifo_checksum(fifo_t* pFifo, int offset, int len);
