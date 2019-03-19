/*!
   \@file fifo.c
   \@brief source for universal fifo
   \@author Jason Berger
   \@date 10/18/2017
*/

#include <stdlib.h>
#include "Fifo.h"


#ifndef MRT_MUTEX_LOCK
  #define MRT_MUTEX_LOCK mLock =1//while(mLock){delay_ms(1);} mLock = 1
  #define MRT_MUTEX_UNLOCK mLock = 0
#endif


namespace Utilities
{

Fifo::Fifo(int objSize, int len)
{
  mBuffer = (uint8_t*) malloc(objSize * len);
  mHead = 0;
  mTail = 0;
  mMaxLen = len;
  mCount = 0;
  mObjSize = objSize;
}

Fifo::~Fifo()
{
  free(mBuffer);
}

int Fifo::push( void* data)
{
	MRT_MUTEX_LOCK;

    // next is where head will point to after this write.
    int next = mHead + 1;

    if (next > mMaxLen)
    {
        next = 0;
    }
    if (next == mTail) // check if circular buffer is full
    {
		MRT_MUTEX_UNLOCK;
        return FIFO_OVERFLOW;
    }

    mCount++;

    int addr = mHead * mObjSize;
    memcpy(&mBuffer[addr], data, mObjSize); // Load data and then move


    mHead = next;            // head to next data offset.

    MRT_MUTEX_UNLOCK;

    return 0;  // return success to indicate successful push.
}

int Fifo::pop( void *data)
{
	MRT_MUTEX_LOCK;
    // if the head isn't ahead of the tail, we don't have any characters
    if (mHead == mTail) // check if circular buffer is empty
    {
		MRT_MUTEX_UNLOCK;
        return FIFO_UNDERFLOW;          // and return with an error
    }

    // next is where tail will point to after this read.
    int next = mTail + 1;
    if (next > mMaxLen)
    {
        next = 0;
    }
    int addr = mTail * mObjSize;
    memcpy(data,&mBuffer[addr],mObjSize);
    mTail = next;             // tail to next data offset.

    if(mCount > 0)
    {
        mCount--;
    }
    MRT_MUTEX_UNLOCK;

    return 0;  // return success to indicate successful push.
}

int Fifo::pushBuffer( void * data, int len)
{
  int result = 0;
  for(int i=0; i < len; i++)
  {
    result = push(data+= (i * mObjSize));
  }
  return result;
}

int Fifo::popBuffer( void* data, int len)
{
  int result = 0;
  for(int i=0; i < len; i++)
  {
    result = pop(data+= (i * mObjSize));
  }
  return result;
}

int Fifo::clear(int len)
{
  //create trash bin based on objsize
  uint8_t* trash = (uint8_t*)malloc(mObjSize);

  if(len > mCount)
    len = mCount;

  //there are more effecient ways to do this
  //but it should be a rare case and this utilizes locks in place
  while(len > 0)
  {
    pop(trash);
    len--;
  }
  free(trash);
}

int Fifo::peek( void* data, int idx)
{
  MRT_MUTEX_LOCK;
  if(mCount <= idx)
  {
        MRT_MUTEX_UNLOCK;
        return FIFO_UNDERFLOW;
  }

  int addr = mTail + idx; //

  //handle buffer wrapping
  if(addr > mMaxLen)
  {
      addr -= mMaxLen;
  }

  memcpy(data,&mBuffer[addr* mObjSize],mObjSize);
  MRT_MUTEX_UNLOCK;
  return 0;  // return success to indicate successful push.
}


int Fifo::peekBuffer(void* data, int len)
{
  MRT_MUTEX_LOCK;
  int addr = mTail;
  uint8_t* cast = (uint8_t*)data;

  if(mCount < len)
    len = mCount;

  len*=mObjSize;

  for(int i=0; i < len; i++)
  {
    cast[i] = mBuffer[addr++];

    //wrap;
    if(addr =mMaxLen)
    {
      addr =0;
    }
  }
  MRT_MUTEX_UNLOCK;
  return 0;  // return success to indicate successful push.
}

int Fifo::checksum(int offset, int len)
{
  MRT_MUTEX_LOCK;
  uint16_t sum = 0;
  int addr = mTail + offset;

  if(mCount < (len + offset))
  {
    MRT_MUTEX_UNLOCK;
    return -1;
  }

  for(int i=0; i < len; i++)
  {
    sum+= mBuffer[addr++];

    //wrap
    if(addr = mMaxLen)
    {
      addr =0;
    }
  }


  //we return as an int so we can send -1 to indicate there isnt enough data
  MRT_MUTEX_UNLOCK;
  return (int) sum;
}

extern "C"{

fifo_t* new_fifo( int objSize, int len)
{
  Fifo* p = new Fifo(objSize, len);
  return (Fifo*)p;
}

void fifo_destroy(fifo_t* pFifo)
{
  free(static_cast<Fifo*>(pFifo));
}

int fifo_push(fifo_t* pFifo, void* data)
{
  return static_cast<Fifo*>(pFifo)->push(data);
}

int fifo_pop(fifo_t* pFifo, void* data)
{
  return static_cast<Fifo*>(pFifo)->pop(data);
}

int fifo_push_buf(fifo_t* pFifo, void* data, int len)
{
  return static_cast<Fifo*>(pFifo)->pushBuffer(data, len);
}

int fifo_pop_buf(fifo_t* pFifo, void* data, int len)
{
  return static_cast<Fifo*>(pFifo)->pushBuffer(data, len);
}

int fifo_clear(fifo_t* pFifo, int len )
{
  return static_cast<Fifo*>(pFifo)->clear(len);
}

int fifo_peek(fifo_t* pFifo, void* data, int idx)
{
  return static_cast<Fifo*>(pFifo)->peek(data, idx);
}

int fifo_peek_buf(fifo_t* pFifo, void* data, int len)
{
  return static_cast<Fifo*>(pFifo)->peekBuffer(data, len);
}

int fifo_checksum(fifo_t* pFifo, int offset, int len)
{
  return static_cast<Fifo*>(pFifo)->checksum(offset, len);
}


}
}
