#ifndef __YZ_YZSocketSelector_H__
#define __YZ_YZSocketSelector_H__

#include "YZCommon/YZCommonMarco.h"
#include "YZSocketMessage.h"
#include <unordered_map>

namespace YZ
{

	template <class KEY>
	class YZSocketSelector
	{
		typedef typename std::unordered_map<KEY, YZSocketSelectorItem> SOCKET_OBSERVERS;
		typedef typename std::unordered_map<KEY, YZSocketSelectorItem>::iterator ITERATOR;

	public:
		YZSocketSelector()
		{
		}

		~YZSocketSelector()
		{
			clear();
		}

	public:
		void addSelector(KEY key, SEL_SocketMessage selector)
		{
			YZ_ASSERT(nullptr != selector, "selector is nullptr");
			if (nullptr == selector) return;

			auto tmp = _observers.find(key);
			if (tmp != _observers.end())
			{
				tmp->second = selector;
			}
			else
			{
				_observers.insert(std::make_pair(key, YZSocketSelectorItem(selector)));
			}
		}

		void removeSelector(KEY key)
		{
			auto selector = _observers.find(key);
			if (selector != _observers.end())
			{
				_observers.erase(key);
			}
		}

		bool executeSelector(KEY key, YZSocketMessage* socketMessage)
		{
			auto selector = _observers.find(key);
			if (selector != _observers.end())
			{
				selector->second.doCallSelector(socketMessage);
				return true;
			}
			return false;
		}

		ITERATOR begin()
		{
			return _observers.begin();
		}

		ITERATOR end()
		{
			return _observers.end();
		}

		bool empty()
		{
			return _observers.empty();
		}

		void clear() 
		{
			_observers.clear();
		}
	private:
		SOCKET_OBSERVERS _observers;
	};
}

#endif	//__YZ_YZSocketSelector_H__

