Fifo
====

This module provides a Generic 'type' fifo in pure C. It is not as effecient as a typed fifo, but it provides the flexibity of storing different types and structs. 

Fifos can be defined staticly or initiallized dynamicly 

dynamic example:

.. code-block:: C

    #include "Modules/Utilities/fifo.h"

    fifo_t myFifo;

    uint16_t myBuf[64];

    int main(void)
    {
        //creates a fifo that can store 64 uin16_t
        fifo_init(&myFifo, 64, sizeof(uint16_t));   
        
        uint16_t myData = 0;
        for(int i =0; i < 64; i++)
        {
            myData++;
            fifo_push(&myData); //
        }

        fifo_pop_buf(&myFifo, myBuf, 64);

        return 0;
    }



static example:

.. code-block:: C 

    #include "Modules/Utilities/fifo.h"

    FIFO_DEF(myFifo, 64, uint16_t); //Expands to:
    /*
    uint16_t myFifo_data[64];
    fifo_t myFifo = {                      
        .mBuffer = myFifo_data,     
        .mHead = 0,                 
        .mTail = 0,                 
        .mMaxLen = 64,             
        .mCount = 0,                
        .mObjSize = sizeof(uin16_t)	
    };
    */

    uint16_t myBuf[64];

    int main(void)
    {
        uint16_t myData = 0;
        for(int i =0; i < 64; i++)
        {
            myData++;
            fifo_push(&myData); //
        }

        fifo_pop_buf(&myFifo, myBuf, 64);

        return 0;
    }

.. note:: The main benefit of the static define is that it uses an array of 'type' to hold the data. This can help with debugging when the type is a struct. 
