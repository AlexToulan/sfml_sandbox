#pragma once

template<class T, class P>
struct TDelegate
{
  TDelegate(T* objectPtr, void(T::* func)(const P&))
  {
    _objectPtr = objectPtr;
    _func = func;
  }
  void operator()(const P& e)
  {
    (_objectPtr->*_func)(e);
  }

  bool operator==(const TDelegate<T, P>& other) const
  {
    return _objectPtr == other._objectPtr &&  _func == other._func;
  }

  bool operator!=(const TDelegate<T, P>& other) const
  {
    return _objectPtr != other._objectPtr || _func != other._func;
  }

  bool isOwner(T* objectPtr) const
  {
    return _objectPtr == objectPtr;
  }
private:
  T* _objectPtr;
  void(T::* _func)(const P&);
};
