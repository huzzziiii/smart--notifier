#pragma once

#include <cstdint>
#include <cstddef>

template<typename T>
class Fifo
{
    public:

    void Write(T data)
    {
        mBuffer[mWriteIdx++] = data;
        mWriteIdx %= SIZE;
        mCount++;
    }

    void WriteElements(T* data, size_t length)
    {
        for (size_t idx = 0; idx < length; idx++)
        {
	  Write(*data++);
        }
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

    T Peek()
    {
        if (IsEmpty())
        {
	  return 0;       // TODO: look for alternate ways. would have returned an optional
        }
        return mBuffer[mReadIdx];
    }

    bool IsEmpty() const
    {
        return mCount == 0;
    }

    private:

    uint8_t mReadIdx = 0;
    uint8_t mWriteIdx = 0;
    uint8_t mCount = 0;

    static constexpr uint8_t SIZE = 100;
    T mBuffer[SIZE] = {0};
};

