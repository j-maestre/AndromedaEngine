#pragma once

#include "base.h"

#include "Common/ID.h"
#include "Common/Entity.h"

namespace And
{
	template<typename T>
	class component_iterator;

	namespace internal
	{
		template<typename T>
		struct component
		{
			ID id;
			T value;

			bool operator ==(const component& other)
			{
				return id == other.id;
			}
		};

		struct component_list_base 
		{
			virtual void add_empty(uint64 id) = 0;
			virtual void remove(uint64 id) = 0;
			virtual size_t size() = 0;
		};

		template<typename T>
		struct component_list_imp : public component_list_base
		{
			virtual void add_empty(uint64 id) override
			{
				m_Components.push_back(component<T>{id, T()});
			}

			virtual void remove(uint64 id) override
			{
				//TODO: remove the component with the given id
				
			}

			virtual size_t size() override
			{
				return m_Components.size();
			}

			component_iterator<component<T>> begin()
			{
				return component_iterator<component<T>>(m_Components.data());
			}

			component_iterator<component<T>> end()
			{
				return component_iterator<component<T>>(m_Components.data() + m_Components.size());
			}

			std::vector<component<T>> m_Components;
		};

	}

	template<typename T>
	class component_iterator
	{
	public:
		component_iterator(T* ptr) : m_Ptr(ptr) {}
		component_iterator(const component_iterator& other) { m_Ptr = other.m_Ptr; }
		component_iterator(component_iterator&& other) { m_Ptr = other.m_Ptr; }
			
		~component_iterator() {}

		component_iterator& operator =(const component_iterator& other) { if (this != &other) { m_Ptr = other.m_Ptr; } return *this; }
		component_iterator& operator =(component_iterator&& other) { if (this != &other) { std::swap(m_Ptr, other.m_Ptr); } return *this; }

		T& operator *() { return *m_Ptr; }
		const T& operator *() const { return *m_Ptr; }

		T* operator ->() { return m_Ptr; }
		const T* operator ->() const { return m_Ptr; }
			
		component_iterator& operator++() { m_Ptr++; return *this; }
		component_iterator operator++(int) { component_iterator tmp = *this; ++(*this); return tmp; }

		bool operator ==(const component_iterator& other) { return m_Ptr == other.m_Ptr; }
		bool operator !=(const component_iterator& other) { return m_Ptr != other.m_Ptr; }

	private:
		T* m_Ptr;
	};

	class EntityComponentSystem
	{
		NON_COPYABLE_CLASS(EntityComponentSystem)
		NON_MOVABLE_CLASS(EntityComponentSystem)
	public:
		EntityComponentSystem() = default;

		~EntityComponentSystem() = default;

		template<typename comp_t>
		void add_component_class()
		{
			std::unique_ptr<internal::component_list_imp<comp_t>> comps = std::make_unique<internal::component_list_imp<comp_t>>();
			m_Components.insert({typeid(comp_t).hash_code(), std::move(comps)});
		}

		template<typename... comps_t>
		Entity new_entity(comps_t... comps)
		{
			Entity new_e;
			return new_e;
		}

		template<typename... comps_t>
		void get_component_iterator()
		{

		}

		void remove_entity(Entity e)
		{
			
		}

		template<typename comp_t>
		comp_t* get_entity_component(Entity e)
		{
			
			return nullptr;
		}

		template<typename comp_t>
		void add_entity_component(Entity e, comp_t& comp)
		{
			
		}

		template<typename comp_t>
		void remove_entity_component(Entity e)
		{
			
		}

	private:

		template<typename comp_t>
		void insert_comp(comp_t& comp, size_t index)
		{
			
		}

		std::unordered_map<size_t, std::unique_ptr<internal::component_list_base>> m_Components;
		uint64 m_CurrentId = 0;
	};

}