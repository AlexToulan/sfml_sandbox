#pragma once

template<class T, class P>
struct TDelegate
{
  TDelegate(const T* objectPtr, void(T::* func)(const P&))
    : _objectPtr(const_cast<T*>(objectPtr))
    , _func(func)
  {
  }

  void exec(const P& e)
  {
    (_objectPtr->*_func)(e);
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

  bool isOwner(const T* objectPtr) const
  {
    return _objectPtr == objectPtr;
  }

  /*template<typename I, typename J>
  TDelegate as()
  {
    return TDelegate(static_cast<I*>(_objectPtr_), static_cast<void(I::*)(const J&)>(_func));
  }*/
private:
  T* _objectPtr;
  void(T::* _func)(const P&);
};
