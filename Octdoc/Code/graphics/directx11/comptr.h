#pragma once

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			template <typename T>
			class COM_Ptr
			{
				T* m_ptr;

			public:
				COM_Ptr() :m_ptr(nullptr) {}
				COM_Ptr(T *ptr) :m_ptr(ptr) {}
				COM_Ptr(const COM_Ptr& ptr) :m_ptr(ptr.m_ptr)
				{
					if (m_ptr)
						m_ptr->AddRef();
				}
				~COM_Ptr() { Release(); }
				void Release()
				{
					if (m_ptr)
					{
						m_ptr->Release();
						m_ptr = nullptr;
					}
				}
				T* Unbind()
				{
					T *ptr = m_ptr;
					m_ptr = nullptr;
					return ptr;
				}
				operator T*() { return m_ptr; }
				T** operator&() { return &m_ptr; }
				T* operator->() { return m_ptr; }
				T* get() { return m_ptr; }
				T** getAddress() { return &m_ptr; }
				template<typename CAST_TYPE>
				CAST_TYPE* As() { return (CAST_TYPE*)m_ptr; }
				template<typename CAST_TYPE>
				CAST_TYPE** AddressAs() { return (CAST_TYPE**)&m_ptr; }
				bool operator==(T* ptr) { return m_ptr == ptr; }
				bool operator!=(T* ptr) { return m_ptr != ptr; }
				bool operator==(COM_Ptr<T>& ptr) { return m_ptr == ptr.m_ptr; }
				bool operator!=(COM_Ptr<T>& ptr) { return m_ptr != ptr.m_ptr; }
				operator bool() { return m_ptr != nullptr; }
				COM_Ptr<T>& operator=(T* ptr) 
				{
					if (m_ptr != ptr)
					{
						Release();
						m_ptr = ptr;
					}
					return *this;
				}
				COM_Ptr<T>& operator=(COM_Ptr<T>& ptr)
				{
					if (m_ptr != ptr.m_ptr)
					{
						Release();
						m_ptr = ptr.m_ptr;
						if (m_ptr)
							m_ptr->AddRef();
					}
					return *this;
				}
			};
		}
	}
}