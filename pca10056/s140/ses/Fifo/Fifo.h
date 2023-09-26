#pragma once

#include <cstdint>

template<typename T>
class Fifo
{
    public:

    void Write(const T data)
    {
        mBuffer[mWriteIdx++] = data;
        mWriteIdx %= SIZE;
        mCount++;
    }

    T Read()
    {
        if (!IsEmpty())
        {
	  T data = mBuffer[mReadIdx++];
	  mReadIdx %= SIZE;
	  mCount--;
	  return data;    
        }
        return 0;	// TODO: look for alternate ways. would have returned an optional
    }

    bool IsEmpty() const
    {
        return mCount == 0;
    }

    private:

    uint8_t mReadIdx = 0;
    uint8_t mWriteIdx = 0;
    uint8_t mCount = 0;

    static constexpr uint8_t SIZE = 10;
    T mBuffer[SIZE] = {0};
};

