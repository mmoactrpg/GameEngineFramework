#pragma once

#include "stdafx.h"


//==========================================
// 功能描述：
// 创建者：JB
// 创建日期：[4/7/2012]
// 修改日期：[4/7/2012]
//-----------------------------------------------------------------------------

template <class T> class SmartPtr;

//==========================================
// 功能描述：	 IRefCount,
// 是应用计数的接口.
// 此类应该有自己的实现或者SmartPtr会提供它的IRefCount的内部实现
// 创建者：JB
// 创建日期：[4/7/2012]
// 修改日期：[4/7/2012]
//-----------------------------------------------------------------------------
template <class T> class IRefCount
{
	friend class SmartPtr<T>;
protected:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual T * GetPtr() const = 0;
};

//==========================================
// 功能描述：	 IRefCountImpl
// 是IRefCount的标准实现
// 要使用的话直接从其派生
// class MyObject : public IRefCountImpl<MyObject> { ... };
// 提醒：实现 IRefCount 是可选的，但这么做可以减少内存碎片
// 创建者：JB
// 创建日期：[4/7/2012]
// 修改日期：[4/7/2012]
//-----------------------------------------------------------------------------
template <class T> class IRefCountImpl
	: public IRefCount<T>
{
private:
	int m_Count;

protected:
	virtual void AddRef() { m_Count++; }
	virtual void Release()
	{
		assert( m_Count >= 0 );
		m_Count--;
		if( m_Count <= 0 )
		{
			Destroy();
		}
	}
	virtual T * GetPtr() const { return ((T *) this ); }
	virtual void Destroy() { if( GetPtr() != NULL ) delete GetPtr(); }

	IRefCountImpl() { m_Count = 0; }
};


//==========================================
// 功能描述：	 SmartPtr
// 创建者：JB
// 创建日期：[4/7/2012]
// 修改日期：[4/7/2012]
//-----------------------------------------------------------------------------
template <class T> class SmartPtr
{
private:
	IRefCount<T>  * m_RefCount;

	#pragma region 为没有实现IRefCount的类提供的内部实现

	//==========================================
	// 功能描述：	 RefCounter
	// 为没有实现IRefCount的类提供的内部实现
	// SmartPtr 会自动选择内部实现或者实现 IRefCount的类
	// 创建者：JB
	// 创建日期：[4/7/2012]
	// 修改日期：[4/7/2012]
	//-----------------------------------------------------------------------------
	class RefCounter : public IRefCountImpl<T>
	{
	private:
		T * m_Ptr;
	
	protected:
		virtual T * GetPtr() const { return m_Ptr; }
		virtual void Destroy() { delete this; }
	
	public:
		RefCounter(T * ptr) { m_Ptr = ptr; }
		virtual ~RefCounter() { IRefCountImpl<T>::Destroy(); }

	};
	#pragma endregion 为没有实现IRefCount的类提供的内部实现

	//如果T 没有实现 IRefCount，这个方法被调用
	void Assign(void * ptr)
	{
		if(NULL == ptr)
		{
			Assign( (IRefCount<T> *) NULL);
		}
		else
		{
			Assign(new RefCounter( static_cast<T *>(ptr) ));
		}
	}

	//如果实现了IRefCount，这个方法会被调用
	void Assign(IRefCount<T> * refcount)
	{
		if( refcount != NULL)
		{
			refcount->AddRef();
		}
		//
		IRefCount<T> * oldref = m_RefCount;
		m_RefCount = refcount;
		//
		if( oldref != NULL)
		{
			oldref->Release();
		}
	}

public:
	SmartPtr() { m_RefCount = NULL; }
	SmartPtr(T * ptr) { m_RefCount = NULL; Assign(ptr); }
	SmartPtr(const SmartPtr &sp) { m_RefCount = NULL; Assign(sp.m_RefCount); }
	virtual ~SmartPtr(void) { Assign( (IRefCount<T> *) NULL ); }

	T * GetPtr() const { return ( NULL == m_RefCount ) ? NULL : m_RefCount->GetPtr(); }

	//赋值重载
	SmartPtr & operator = (const SmartPtr &sp) { Assign( sp.m_RefCount ); return *this; }
	SmartPtr & operator = (T * ptr) { Assign(ptr); return *this; }

	// T 访问和 const 转换
	T * operator ->() { assert( GetPtr() != NULL); return GetPtr(); }
	operator T* () const { return GetPtr(); }

	//工具
	bool operator ! () { return GetPtr()== NULL; }
	bool operator == (const SmartPtr &sp ) { return GetPtr() ==  sp->GetPtr(); }
	bool operator !=  (const SmartPtr &sp) {return GetPtr() != sp->GetPtr(); }
};

