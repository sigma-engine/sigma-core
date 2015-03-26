#ifndef SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
#define SIGMAFIVE_GAME_ENTITY_MANAGER_HPP

#include <game/entity.hpp>
#include <game/component.hpp>
#include <game/component_pool.hpp>
#include <game/component_system.hpp>

#include <utility>
#include <vector>
#include <memory>
#include <boost/iterator/filter_iterator.hpp>

namespace sigmafive {
	namespace game {
		class entity_manager;

		template<typename Iter,typename ... Comps>
		class entities_with_components_adaptor {
		public:
			struct entity_has_components {
				bitset bs;
				inline entity_has_components(bitset bs) :bs(bs) {}
				inline bool operator()(entity e) const { return (e.component_mask() & bs) == bs; }
			};

			typedef boost::filter_iterator<entity_has_components,Iter> iterator;

			inline entities_with_components_adaptor(Iter begin,Iter end) : begin_(begin), end_(end), pred_(detail::bitset_builder<Comps...>{}()) {}

			inline iterator begin() noexcept {
				return iterator(pred_,begin_,end_);
			}

			inline iterator end() noexcept {
				return iterator(pred_,end_,end_);
			}
		private:
			Iter begin_;
			Iter end_;
			entity_has_components pred_;
		};

		class entity_manager {
		private:
			struct is_entity_alive {
				inline bool operator()(entity e) { return e.id().index != std::uint32_t(-1); }
			};
		public:
			typedef std::vector<entity> 											  container;
			typedef container::reference                      						  reference;
			typedef container::const_reference                						  const_reference;
			typedef boost::filter_iterator<is_entity_alive,container::iterator> 	  iterator;
			typedef boost::filter_iterator<is_entity_alive,container::const_iterator> const_iterator;
			typedef container::size_type                         					  size_type;
			typedef container::difference_type                          			  difference_type;
			typedef container::value_type                               			  value_type;
			typedef container::allocator_type                           			  allocator_type;
			typedef container::pointer       										  pointer;
			typedef container::const_pointer 										  const_pointer;
			typedef std::reverse_iterator<iterator>                     			  reverse_iterator;
			typedef std::reverse_iterator<const_iterator>               			  const_reverse_iterator;

			entity_manager();

			~entity_manager();

			entity_manager(const entity_manager &) = delete;

			entity_manager&operator=(const entity_manager &) = delete;

			entity create();

			bool is_alive(entity e) const;

			template<class T>
			T *add_component(entity e) {
				assert(is_alive(e) && "entity must be alive to add a component to it.");
				assert(!has_component<T>(e) && "can not add component twice to a entity.");

				if(T::ID >= components_by_type_.size())
					components_by_type_.resize(T::ID + 1);

				if(!components_by_type_[T::ID])
					components_by_type_[T::ID] = std::unique_ptr<component_pool_base>(new component_pool<T>{});

				entity old = entities_[e.id().index];
				entities_[e.id().index].component_mask_.set(T::ID,true);
				auto c = components_by_type_[T::ID]->add_component(e.id());

				for(const auto &sys:component_systems_) {
					if(sys) {
						if (!sys->is_intrested(old) && sys->is_intrested(entities_[e.id().index]))
							sys->entity_added(entities_[e.id().index]);
					}
				}

				return static_cast<T*>(c);
			}

			template<class T>
			bool has_component(entity e) const {
				assert(is_alive(e) && "entity must be alive to test if it has a component.");
				bitset b;
				b.set(T::ID,true);
				return (entities_[e.id().index].component_mask_ & b) == b;
			}

			template<class T>
			T *get_component(entity e) {
				assert(is_alive(e) && "entity must be alive to get a component for it.");
				assert(has_component<T>(e) && "can not get a component for an entity if it does not have it.");
				return static_cast<T*>(components_by_type_[T::ID]->get_component(e.id()));
			}

			template<typename ... Comps>
			inline entities_with_components_adaptor<entity_manager::const_iterator,Comps...> entities_with_components() const {
				return entities_with_components_adaptor<entity_manager::const_iterator,Comps...>(begin(),end());
			}

			template<class T>
			void remove_component(entity e) {
				assert(is_alive(e) && "entity must be alive to remove a component from it.");
				assert(has_component<T>(e) && "can not remove a component from an entity that does not have one.");

				entity newE = entities_[e.id().index];
				newE.component_mask_.set(T::ID,false);
				for(const auto &sys:component_systems_) {
					if(sys) {
						if (sys->is_intrested(entities_[e.id().index]) && ! sys->is_intrested(newE))
							sys->entity_removed(entities_[e.id().index]);
					}
				}

				components_by_type_[T::ID]->remove_component(entities_[e.id().index].id());
				entities_[e.id().index].component_mask_.set(T::ID,false);
			}

			void destroy(entity e);

			template<typename T,typename ... ARGS>
			T *add_system(ARGS&& ... args) {
				if(T::ID >= component_systems_.size())
					component_systems_.resize(T::ID+1);
				component_systems_[T::ID] = std::unique_ptr<T>(new T(std::forward<ARGS>(args)...));
				for (entity e:*this) {
					if (component_systems_[T::ID]->is_intrested(e))
						component_systems_[T::ID]->entity_added(e);
				}
				return static_cast<T*>(component_systems_[T::ID].get());
			}

			template<typename T>
			T *get_system() {
				if(T::ID >= component_systems_.size())
					return nullptr;
				return component_systems_[T::ID];
			}

			template<typename T>
			void remove_system() {
				if(T::ID >= component_systems_.size())
					return;
				if(component_systems_[T::ID]) {
					for (entity e:*this) {
						if (component_systems_[T::ID]->is_intrested(e))
							component_systems_[T::ID]->entity_removed(e);
					}
					component_systems_[T::ID] = nullptr;
				}
			}

			iterator begin();

			iterator end();

			const_iterator begin() const;

			const_iterator end() const;

			reverse_iterator rbegin();

			reverse_iterator rend();

			const_reverse_iterator rbegin() const;

			const_reverse_iterator rend() const;

			const_iterator cbegin();

			const_iterator cend();

			const_reverse_iterator crbegin() const;

			const_reverse_iterator crend() const;
		private:
			container entities_;
			std::vector<std::uint32_t> free_entities_;
			std::vector<std::unique_ptr<component_pool_base>> components_by_type_;
			std::vector<std::unique_ptr<component_system>> component_systems_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
