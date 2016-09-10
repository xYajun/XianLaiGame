#ifndef __YZ_YZUIDelegateQueue_H__
#define __YZ_YZUIDelegateQueue_H__

#include "YZCommonMarco.h"
#include <unordered_map>
#include <assert.h>

namespace YZ
{
	template <class T>
	class YZUIDelegateQueue
	{
		typedef typename std::unordered_map<T, T> DelegateQueue;
		typedef typename std::unordered_map<T, T>::iterator ITERATOR;

	public:
		YZUIDelegateQueue()
		{
			_uiQueue = new DelegateQueue();
		}

		~YZUIDelegateQueue()
		{
			YZ_SAFE_DELETE(_uiQueue);
		}

	public:
		void addObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto Value = std::make_pair(delegate, delegate);
				_uiQueue->insert(Value);
			}
		}

		void removeObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto iter = _uiQueue->find(delegate);
				if (iter != _uiQueue->end())
				{
					_uiQueue->erase(iter);
				}
			}
		}

		ITERATOR begin()
		{
			return _uiQueue->begin();
		}

		ITERATOR end()
		{
			return _uiQueue->end();
		}

	private:
		DelegateQueue * _uiQueue;
	};
}

#endif	//__YZ_YZUIDelegateQueue_H__

