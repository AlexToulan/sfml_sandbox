#pragma once

/// @brief Generic member function Delegate for storage and explicit variadic execution.
struct GDelegate
{
  /// @brief Factory method type converting to Delegate
  /// @tparam TReturn Delegate return type.
  /// @tparam TObj Object pointer type.
  /// @tparam ...VArgs Parameter pack types.
  /// @param objPtr Pointer to the subscriber instance.
  /// @param func Member function pointer to call.
  /// @return Underlying delegate return value.
  template<class TReturn, class TObj, class... VArgs>
  static GDelegate create(TObj* objPtr, TReturn(TObj::* func)(VArgs...))
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
    TReturn(TObj::* func)(VArgs...) = reinterpret_cast<TReturn(TObj::*)(VArgs...)>(_func);
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

private:
  /// @brief Generic placeholder object type
  class Object{};

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
