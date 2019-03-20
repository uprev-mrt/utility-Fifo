/**
  *@file FifoTest.cpp
  *@brief unit tests for the Fifo class
  *@author Jason Berger
  *@date 02/16/2019
  */
#ifdef UNIT_TESTING_ENABLED

extern "C"{

  #include "fifo.c"
}
#include <gtest/gtest.h>


int c_testSize = 64;
fifo_t* test_fifo_c;
int c_comp;


//C tests
//Test pushing data to Fifo
TEST(FifoTest_c, Push_c)
{
  test_fifo_c = new_fifo(sizeof(int), c_testSize);
  //push 64 bytes into fifo
  for(int i=0; i < c_testSize; i++)
  {
    //verify count
    ASSERT_EQ(i, test_fifo_c->mCount);

    //make sure we get a status FIFO_OK for each push
    ASSERT_EQ(FIFO_OK , fifo_push(test_fifo_c, &i));
  }
}

//Verify Overflow handling
TEST(FifoTest_c, Overflow_c)
{
  ASSERT_EQ(FIFO_OVERFLOW , fifo_push(test_fifo_c, &c_comp));
  ASSERT_EQ(c_testSize, test_fifo_c->mCount);
}

//Verify Overflow handling
TEST(FifoTest_c, PeekBuffer_c)
{
  int peekBuf[10];

  int tp = fifo_peek_buf(test_fifo_c, peekBuf, 10);

  ASSERT_EQ(10 * sizeof(int), tp);

  for(int i =0; i < 10; i++)
  {
    ASSERT_EQ(i, peekBuf[i]);
  }

}

//Test popping data from fifo
TEST(FifoTest_c, Pop_c)
{
  //push 64 bytes into fifo
  for(int i=0; i < c_testSize; i++)
  {
    //make sure we get a status FIFO_OK
    ASSERT_EQ(c_testSize-i, test_fifo_c->mCount);
    ASSERT_EQ(FIFO_OK , fifo_pop(test_fifo_c, &c_comp));
    ASSERT_EQ(i, c_comp);
  }
}

//Verify Underflow handling
TEST(FifoTest_c, Underflow_c)
{
    ASSERT_EQ(FIFO_UNDERFLOW , fifo_pop(test_fifo_c, &c_comp));
    ASSERT_EQ(0, test_fifo_c->mCount);
}

#endif
