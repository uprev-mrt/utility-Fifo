/*!
   \@file Fifo_c.case
   \@brief pure C implementation of fifo
   \@author Jason Berger
   \@date 05/03/2017
*/


#include "Fifo.h"



#define FIFO_LOCK pFifo->mLock =1//while(pFifo->lock){delay_ms(1);} pFifo->lock = 1
#define FIFO_UNLOCK pFifo->mLock = 0

void fifo_init(fifo_t* pFifo, int depth, int width)
{
  pFifo->mBuffer = (uint8_t*) malloc(depth * width);
  pFifo->mHead = 0;
  pFifo->mTail = 0;
  pFifo->mMaxLen = depth;
  pFifo->mCount = 0;
  pFifo->mObjSize = width;
}

void fifo_deinit(fifo_t* pFifo)
{
  free(pFifo->mBuffer);
}

int fifo_push( fifo_t* pFifo, void* data)
{
	FIFO_LOCK;

    // next is where head will point to after this write.
    int next = pFifo->mHead + 1;

    if (next > pFifo->mMaxLen)
    {
        next = 0;
    }
    if (next == pFifo->mTail) // check if circular buffer is full
    {
		FIFO_UNLOCK;
        return -1;
    }

    pFifo->mCount++;

    int addr = pFifo->mHead * pFifo->mObjSize;
    memcpy(&pFifo->mBuffer[addr], data, pFifo->mObjSize); // Load data and then move


    pFifo->mHead = next;            // head to next data offset.

    FIFO_UNLOCK;

    return 0;  // return success to indicate successful push.
}

int fifo_pop( fifo_t* pFifo, void *data)
{
	FIFO_LOCK;
    // if the head isn't ahead of the tail, we don't have any characters
    if (pFifo->mHead == pFifo->mTail) // check if circular buffer is empty
    {
		FIFO_UNLOCK;
        return -1;          // and return with an error
    }

    // next is where tail will point to after this read.
    int next = pFifo->mTail + 1;
    if (next > pFifo->mMaxLen)
    {
        next = 0;
    }
    int addr = pFifo->mTail * pFifo->mObjSize;
    memcpy(data,&pFifo->mBuffer[addr],pFifo->mObjSize);
    pFifo->mTail = next;             // tail to next data offset.

    if(pFifo->mCount > 0)
    {
        pFifo->mCount--;
    }
    FIFO_UNLOCK;

    return 0;  // return success to indicate successful push.
}

int fifo_push_buf( fifo_t* pFifo, void * data, int len)
{
  int result = 0;
  for(int i=0; i < len; i++)
  {
    result = fifo_push(pFifo,&data[i * pFifo->mObjSize]);
  }
  return result;
}

int fifo_pop_buf( fifo_t* pFifo, void* data, int len)
{
  int result = 0;
  for(int i=0; i < len; i++)
  {
    result = fifo_pop(pFifo, &data[i * pFifo->mObjSize]);
  }
  return result;
}

int fifo_clear( fifo_t* pFifo)
{
  //create trash bin based on objsize
  uint8_t* trash = malloc(pFifo->mObjSize);

  //there are more effecient ways to do this
  //but it should be a rare case and this utilizes locks in place
  while(pFifo->mCount > 0)
  {
    fifo_pop(pFifo,trash);
  }
  free(trash);
}

int fifo_peek( fifo_t* pFifo, void* data, int idx)
{
  FIFO_LOCK;
  if(pFifo->mCount <= idx)
  {
        FIFO_UNLOCK;
        return -1;
  }

  int addr = pFifo->mTail + idx; //

  //handle buffer wrapping
  if(addr > pFifo->mMaxLen)
  {
      addr -= pFifo->mMaxLen;
  }

  memcpy(data,&pFifo->mBuffer[addr* pFifo->mObjSize],pFifo->mObjSize);
  FIFO_UNLOCK;
  return 0;  // return success to indicate successful push.
}
