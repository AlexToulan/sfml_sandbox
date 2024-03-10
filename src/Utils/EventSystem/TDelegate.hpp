#pragma once

template<class T, class P>
class TDelegate
{
public:
  TDelegate(const T* objectPtr, void(T::* func)(P))
    : _objectPtr(const_cast<T*>(objectPtr))
    , _func(func)
  {
  }

  TDelegate(const TDelegate& other)
    : _objectPtr(other._objectPtr)
    , _func(other._func)
  {
  }

  TDelegate(const TDelegate&& other)
  {
    _objectPtr = other._objectPtr;
    _func = other._func;

    other._objectPtr = nullptr;
    other._func = nullptr;
  }

  void exec(const P& e)
  {
    (_objectPtr->*_func)(e);
  }

  template<typename I, typename J>
  void execAs(const J& e)
  {
    (static_cast<I*>(_objectPtr)->*(reinterpret_cast<void(I::*)(J)>(_func)))(e);
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

  bool isCaller(const T* caller) const
  {
    return _objectPtr == caller;
  }

  template<typename I, typename J>
  TDelegate<I, J> as()
  {
    return TDelegate<I, J>(static_cast<I*>(_objectPtr), reinterpret_cast<void(I::*)(J)>(_func));
  }

private:
  T* _objectPtr;
  void(T::* _func)(P);
};
