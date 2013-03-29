#pragma once

#include "stdafx.h"


//==========================================
// ����������
// �����ߣ�JB
// �������ڣ�[4/7/2012]
// �޸����ڣ�[4/7/2012]
//-----------------------------------------------------------------------------

template <class T> class SmartPtr;

//==========================================
// ����������	 IRefCount,
// ��Ӧ�ü����Ľӿ�.
// ����Ӧ�����Լ���ʵ�ֻ���SmartPtr���ṩ����IRefCount���ڲ�ʵ��
// �����ߣ�JB
// �������ڣ�[4/7/2012]
// �޸����ڣ�[4/7/2012]
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
// ����������	 IRefCountImpl
// ��IRefCount�ı�׼ʵ��
// Ҫʹ�õĻ�ֱ�Ӵ�������
// class MyObject : public IRefCountImpl<MyObject> { ... };
// ���ѣ�ʵ�� IRefCount �ǿ�ѡ�ģ�����ô�����Լ����ڴ���Ƭ
// �����ߣ�JB
// �������ڣ�[4/7/2012]
// �޸����ڣ�[4/7/2012]
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
// ����������	 SmartPtr
// �����ߣ�JB
// �������ڣ�[4/7/2012]
// �޸����ڣ�[4/7/2012]
//-----------------------------------------------------------------------------
template <class T> class SmartPtr
{
private:
	IRefCount<T>  * m_RefCount;

	#pragma region Ϊû��ʵ��IRefCount�����ṩ���ڲ�ʵ��

	//==========================================
	// ����������	 RefCounter
	// Ϊû��ʵ��IRefCount�����ṩ���ڲ�ʵ��
	// SmartPtr ���Զ�ѡ���ڲ�ʵ�ֻ���ʵ�� IRefCount����
	// �����ߣ�JB
	// �������ڣ�[4/7/2012]
	// �޸����ڣ�[4/7/2012]
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
	#pragma endregion Ϊû��ʵ��IRefCount�����ṩ���ڲ�ʵ��

	//���T û��ʵ�� IRefCount���������������
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

	//���ʵ����IRefCount����������ᱻ����
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

	//��ֵ����
	SmartPtr & operator = (const SmartPtr &sp) { Assign( sp.m_RefCount ); return *this; }
	SmartPtr & operator = (T * ptr) { Assign(ptr); return *this; }

	// T ���ʺ� const ת��
	T * operator ->() { assert( GetPtr() != NULL); return GetPtr(); }
	operator T* () const { return GetPtr(); }

	//����
	bool operator ! () { return GetPtr()== NULL; }
	bool operator == (const SmartPtr &sp ) { return GetPtr() ==  sp->GetPtr(); }
	bool operator !=  (const SmartPtr &sp) {return GetPtr() != sp->GetPtr(); }
};

