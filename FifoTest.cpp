
#ifdef UNIT_TESTING_ENABLED

#include "Fifo.cpp"
#include <gtest/gtest.h>

using namespace Utilities;

int testSize = 64;
Fifo testFifo(sizeof(int), 64);
fifo_t* test_fifo_c;
int comp;

//Test pushing data to Fifo
TEST(FifoTest, Push)
{
  //push 64 bytes into fifo
  for(int i=0; i < testSize; i++)
  {
    //verify count
    ASSERT_EQ(i, testFifo.Count());

    //make sure we get a status FIFO_OK for each push
    ASSERT_EQ(FIFO_OK , testFifo.push(&i));
  }
}

//Verify Overflow handling
TEST(FifoTest, Overflow)
{
  ASSERT_EQ(FIFO_OVERFLOW , testFifo.push(&comp));
  ASSERT_EQ(testSize, testFifo.Count());
}

//Test popping data from fifo
TEST(FifoTest, Pop)
{
  //push 64 bytes into fifo
  for(int i=0; i < testSize; i++)
  {
    //make sure we get a status FIFO_OK
    ASSERT_EQ(testSize-i, testFifo.Count());
    ASSERT_EQ(FIFO_OK , testFifo.pop(&comp));
    ASSERT_EQ(i, comp);
  }
}

//Verify Underflow handling
TEST(FifoTest, Underflow)
{
    ASSERT_EQ(FIFO_UNDERFLOW , testFifo.pop(&comp));
    ASSERT_EQ(0, testFifo.Count());
}

//C tests
//Test pushing data to Fifo
TEST(FifoTest_c, Push_c)
{
  test_fifo_c = new_fifo(sizeof(int), testSize);
  //push 64 bytes into fifo
  for(int i=0; i < testSize; i++)
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
  ASSERT_EQ(FIFO_OVERFLOW , fifo_push(test_fifo_c, &comp));
  ASSERT_EQ(testSize, test_fifo_c->mCount);
}

//Test popping data from fifo
TEST(FifoTest_c, Pop_c)
{
  //push 64 bytes into fifo
  for(int i=0; i < testSize; i++)
  {
    //make sure we get a status FIFO_OK
    ASSERT_EQ(testSize-i, test_fifo_c->mCount);
    ASSERT_EQ(FIFO_OK , fifo_pop(test_fifo_c, &comp));
    ASSERT_EQ(i, comp);
  }
}

//Verify Underflow handling
TEST(FifoTest_c, Underflow_c)
{
    ASSERT_EQ(FIFO_UNDERFLOW , fifo_pop(test_fifo_c, &comp));
    ASSERT_EQ(0, test_fifo_c->mCount);
}

#endif
