#ifndef _CBVECTOR_H
#define _CBVECTOR_H

#include <cstring>

template <class T>
class CBVector
{
public:
	CBVector();
	~CBVector();

	/* Capacity */
	size_t Size() { return mSize; }
	size_t Capacity() { return mSize; }
	void Resize(size_t newCapacity); /* Change capacity of container. */
	bool IsEmpty() { return mSize == 0; }



	T& at(size_t index) { return mData[index]; }
	T& operator[](size_t index) { return mData[index]; }
	// This is always a copy construct.
	void Push_back(const T &toPush);
	bool Pop_back();

	bool Insert(size_t index, const T &value);
	bool Erase(size_t index);

	void Clear();

    T* begin()
    {
        return &mData[0];
    }

    T* end()
    {
        return &mData[mSize - 1];
    }

private:
	// Make array larger.
	void Grow_capacity();
	bool IsValidIndex(size_t index);

	T* mData;
	size_t mCapacity;
	size_t mSize;


};

template <class T>
void CBVector<T>::Clear()
{
	// Please note that this does not set capacity to 0.
	// The idea is that when you clear an array, it's most
	// likely you are going to put new stuff in.

	while(!IsEmpty()())
		Pop_back();
}

template <class T>
bool CBVector<T>::IsValidIndex(size_t index)
{
	return index >= 0 && index < mSize;
}

template <class T>
bool CBVector<T>::Insert(size_t index, const T &value)
{
	if (index < 0 || index > mSize) // can insert at mSize (push_back)
		return false;

	if (mSize + 1 > mCapacity)
		Grow_capacity();

	// Shift. Make some space.
	if(mSize > 0)
		for (size_t i = mSize - 1; i >= index; --i)
		{
			mData[i + 1] = mData[i];

			if(i == 0) break; // underflow guard.
		}

	mData[index] = value;
	++mSize;

	return true;
}

template <class T>
bool CBVector<T>::Erase(size_t index)
{
	if (!IsValidIndex(index))
		return false;

	// shift
	for (size_t i = index + 1; i < mSize; ++i)
	{
		mData[i - 1] = mData[i];
	}

	mData[mSize - 1].~T();
	mData[mSize - 1] = NULL;
	--mSize;

	return true;
}

template <class T>
bool CBVector<T>::Pop_back()
{
	return Erase(mSize - 1);
}

template <class T>
void CBVector<T>::Resize(size_t newCapacity)
{
	if (newCapacity == mCapacity) // wtf?
		return;
	else if (newCapacity < mSize) // shrink
	{
		// No need to shrink the actual array,
		// since that introduces performance 
		// overhead. Just destroy elements out 
		// of bound and update bookkeepings.
		for (size_t i = newCapacity; i < mSize; ++i)
		{
			mData[i].~T();
			mData[i] = NULL;
		}

		mCapacity = newCapacity;
		mSize = newCapacity;
	}
	else // grow
	{
		mCapacity = newCapacity;
		// Create new array
		T *newData = new T[mCapacity];

		if (!IsEmpty())
		{
			memcpy(newData, mData, mSize * sizeof(T));
			delete[] mData;
		}

		mData = newData;
	}
}

template <class T>
CBVector<T>::CBVector() :
	mSize(0),
	mCapacity(0)
{
}

template <class T>
CBVector<T>::~CBVector()
{
	delete[] mData;
}

template <class T>
void CBVector<T>::Push_back(const T &toPush)
{
	Insert(mSize, toPush);
}


template <class T>
void CBVector<T>::Grow_capacity()
{
	if (mCapacity == 0)
	{
		Resize(1);
	}
	else
		Resize(mCapacity * 2);
}


#endif
