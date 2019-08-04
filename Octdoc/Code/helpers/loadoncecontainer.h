#pragma once

#include <vector>
#include <memory>
#include <string>

namespace octdoc
{
	namespace hlp
	{
		template <typename T>
		class LoadOnceContainer
		{
			struct Pair
			{
				std::wstring name;
				std::weak_ptr<T> ptr;
				Pair* next;

				Pair(const wchar_t* name, std::shared_ptr<T> ptr) :name(name), ptr(ptr), next(nullptr) {}
			};
			Pair* m_container;

		public:
			LoadOnceContainer() :m_container(nullptr) {}
			~LoadOnceContainer()
			{
				while (m_container)
				{
					Pair* tmp = m_container->next;
					delete m_container;
					m_container = tmp;
				}
			}
			void Add(const wchar_t* name, std::shared_ptr<T> ptr)
			{
				Pair* tmp = m_container;
				m_container = new Pair(name, ptr);
				m_container->next = tmp;
			}
			std::shared_ptr<T> Find(const wchar_t* key)
			{
				for (Pair* p = m_container; p; p = p->next)
					if (key == p->name)
						return p->ptr.lock();
				return nullptr;
			}
			void Cleanup()
			{
				Pair* p = m_container;
				Pair** prev = &m_container;
				while (p)
				{
					Pair* tmp = p->next;
					if (p->ptr.expired())
					{
						*prev = p->next;
						delete p;
					}
					prev = &((*prev)->next);
					p = tmp;
				}
			}
		};
	}
}