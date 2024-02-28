#pragma once
#include <map>
#include <functional>

using EventKey = size_t;

template<typename ..._Args>
class Event
{
public:
	using Callback = std::function<void(_Args&...)>;
	template<class _Ty, typename _Ret>
	using MemberFunction = _Ret(_Ty::*)(_Args...);
	template<typename _Ret>
	using StaticFunction = _Ret(*)(_Args...);

	Event();
	~Event() = default;

	template<class _Ty, typename _Ret>
	EventKey Subscribe(_Ty* anInstance, MemberFunction<_Ty, _Ret> aFunction);
	template<typename _Ret>
	EventKey Subscribe(std::function<_Ret(_Args...)> aFunction);
	EventKey Subscribe(std::function<void(_Args...)> aFunction) { return Subscribe<void>(aFunction); };
	void Unsubscribe(EventKey anEventKey);
	void UnsubscribeAll();

	void operator()(_Args... someParameters) const;

private:
	std::map<EventKey, Callback> myCallbacks;
	EventKey myKeyCount;
};

template<typename ..._Args>
template<class _Ty, typename _Ret>
inline EventKey Event<_Args...>::Subscribe(_Ty* anInstance, MemberFunction<_Ty, _Ret> aFunction)
{
	EventKey key = myKeyCount++;
	Callback callback = [=](_Args&... someArgs) { (anInstance->*aFunction)(someArgs...); };
	myCallbacks.emplace(key, callback);
	return key;
}

template<typename ..._Args>
template<typename _Ret>
inline EventKey Event<_Args...>::Subscribe(std::function<_Ret(_Args...)> aFunction)
{
	EventKey key = myKeyCount++;
	//Callback callback = aFunction;
	Callback callback = [aFunction](_Args&... someArgs) { (aFunction)(someArgs...); };
	myCallbacks.emplace(key, callback);
	return key;
}

template<typename ..._Args>
inline Event<_Args...>::Event()
{
	myKeyCount = 0;
}

template<typename ..._Args>
inline void Event<_Args...>::Unsubscribe(EventKey anEventKey)
{
	myCallbacks.erase(anEventKey);
}

template<typename ..._Args>
inline void Event<_Args...>::UnsubscribeAll()
{
	myCallbacks.clear();
}

template<typename ..._Args>
inline void Event<_Args...>::operator()(_Args... someParameters) const
{
	for (auto& entry : myCallbacks)
	{
		entry.second(someParameters...);
	}
}
