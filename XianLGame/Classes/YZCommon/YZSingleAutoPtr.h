#ifndef __YZ_AotuPtr_H__
#define __YZ_AotuPtr_H__

#include <assert.h>

namespace YZ {

	template <class T>
	class YZAutoPtr {
	public:
		YZAutoPtr(T * t): _t(t) {}
		
		~YZAutoPtr() 
		{
			delete _t;
		}

		T& operator *()
		{
			return *_t;
		}

		const T& operator *() const
		{
			return *_t;
		}

		T* operator->()
		{
			return _t;
		}

		const T* operator->() const
		{
			return _t;
		}

	private:
		T* _t;
	};


	template <class T>
	class YZPointerArray {
		T* t;
	public:
		YZPointerArray(T * t): t(t) {
		}

		~YZPointerArray() {
			delete [] t;
			t = nullptr;
		}

		T& operator [] (int i)
		{
			assert(i >= 0);
			return t[i];
		}

		const T& operator [] (int i) const
		{
			assert(i >= 0);
			return t[i];
		}

		operator T* ()
		{
			return t;
		}

		operator T* () const
		{
			return t;
		}
	};

};

#endif	//__YZ_AotuPtr_H__
