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
 *@param name : name of fifo buffer
 *@param len : max number of elements in buffer
 *@param type : datatype
 *@return "Return of the function"
 */
#define FIFO_DEF(name,len,type)     \
  type name##_data[len];            \
  fifo_t name = {                      \
        .mBuffer = name##_data,        \
        .mHead = 0,                 \
        .mTail = 0,                 \
        .mMaxLen = len,             \
        .mCount = 0,                \
        .mObjSize = sizeof(type)	\
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


/**
 * @brief initialize fifo
 * @param pFifo ptr to fifo_t
 * @param depth number of items to to store in fifo
 * @param width size of items in fifo (in bytes)
 */
void fifo_init(fifo_t* pFifo, int depth, int width);

/**
 * @brief deinitialize fifo
 * @param pFifo ptr to fifo
 */
void fifo_deinit(fifo_t* pFifo);

/**
 * @brief push a single 'object' into the fifo
 * @param pFifo ptr to fifo
 * @param data ptr to obj to add to fifo
 * @return FIFO_OK on success  
 */
int fifo_push(fifo_t* pFifo, void* data);

/**
 * @brief pops the next obj from the fifo
 * 
 * @param pFifo ptr to fifo
 * @param data ptr to store obj
 * @return FIFO_OK on success  
 */
int fifo_pop(fifo_t* pFifo, void* data);

/**
 * @brief push a buffer of objects into fifo
 * @param pFifo ptr to fifo
 * @param data ptr to objects to add to fifo
 * @param len number of objects to add to fifo
 * @return FIFO_OK on success 
 */
int fifo_push_buf(fifo_t* pFifo, void* data, int len);

/**
 * @brief retrieve a buffer of objects from the fifo
 * @param pFifo ptr to fifo
 * @param data ptr to store objects
 * @param len number of objects to retrieve
 * @return FIFO_OK on success 
 */
int fifo_pop_buf(fifo_t* pFifo, void* data, int len);

/**
 * @brief deletes objects from fifo
 * @param pFifo ptr to fifo
 * @param len number of objects to remove
 * @return int 
 */
int fifo_clear(fifo_t* pFifo, int len);


int fifo_peek(fifo_t* pFifo, void* data, int idx);
int fifo_peek_buf(fifo_t* pFifo, void* data, int len);
int fifo_checksum(fifo_t* pFifo, int offset, int len);
