#pragma once

/// @brief Generic member function Delegate for storage and explicit variadic execution.
class GDelegate
{
public:
  /// @brief Default constructor.
  GDelegate()
    : _objPtr(nullptr)
    , _func(nullptr)
  {
  }

  GDelegate(const GDelegate& other)
    : _objPtr(other._objPtr)
    , _func(other._func)
  {
  }

  // GDelegate(const GDelegate&& other)
  // {
  //   _objPtr = other._objPtr;
  //   _func = other._func;

  //   other._objPtr = nullptr;
  //   other._func = nullptr;
  // }

  GDelegate& operator=(const GDelegate& other)
  {
    _objPtr = other._objPtr;
    _func = other._func;
    return *this;
  }

  /// @brief Template constructor for converting a VDelegate signature into a GDelegate instance.
  /// @tparam TReturn Delegate return type.
  /// @tparam TObj Caller type.
  /// @tparam ...VArgs Parameter pack to pass to the Delegate method.
  /// @param objPtr Object to call.
  /// @param func Method to call.
  // template<typename TReturn, typename TObj, typename... VArgs>
  // GDelegate(TObj* objPtr, TReturn(TObj::* func)(const VArgs&...))
  // {
  //   _objPtr = static_cast<void*>(objPtr);
  //   _func =  reinterpret_cast<void(Object::*)()>(func);
  // }

  template<class TReturn, class TObj, class... VArgs>
  static GDelegate create(TObj* objPtr, TReturn(TObj::* func)(const VArgs&...))
  {
    return GDelegate(static_cast<void*>(objPtr), reinterpret_cast<void(Object::*)()>(func));
  }

  /// @brief Explicit variadic execution of a generic GDelegate.
  /// @tparam TReturn Delegate return type.
  /// @tparam TObj Object pointer type.
  /// @tparam ...VArgs Parameter pack.
  /// @param ...vargs Parameter pack arguments.
  /// @return Return value returned by the underlying Delegate.
  template<class TReturn, class TObj, class... VArgs>
  TReturn exec(const VArgs&... vargs)
  {
    TObj* o = static_cast<TObj*>(_objPtr);
    TReturn(TObj::* func)(const VArgs&...) = reinterpret_cast<TReturn(TObj::*)(const VArgs&...)>(_func);
    return (o->*func)(vargs...);
  }

  /// @brief Equals operator.
  /// @param other Other instance to compare to.
  /// @return True if both objPtr and func are the same, false otherwise.
  bool operator==(const GDelegate& other) const
  {
    return _objPtr == other._objPtr &&  _func == other._func;
  }

  /// @brief Not-equals operator.
  /// @param other Other instance to compare to.
  /// @return True if objPtr or func are NOT the same, false otherwise.
  bool operator!=(const GDelegate& other) const
  {
    return _objPtr != other._objPtr || _func != other._func;
  }

  /// @brief Checks if this Delegate calls the incoming instance.
  /// @param caller Instance to check ownership against.
  /// @return True if the Delegate references the provided caller.
  template<class TObj>
  bool isCaller(TObj* caller) const
  {
    return _objPtr == static_cast<void*>(caller);
  }

private:
  /// @brief Generic placeholder object type
  class Object {};

  /// @brief Private default constructor.
  /// @param objPtr Generic object reference storage.
  /// @param func Generic method storage. 
  GDelegate(void* objPtr, void(Object::* func)())
    : _objPtr(objPtr)
    , _func(func)
  {
  }

  void* _objPtr;
  void(Object::* _func)();
};
