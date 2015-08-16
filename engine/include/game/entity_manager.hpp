#ifndef SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
#define SIGMAFIVE_GAME_ENTITY_MANAGER_HPP

#include <game/entity.hpp>

#include <vector>
#include <boost/iterator/filter_iterator.hpp>

namespace sigmafive {
	namespace game {
		class entity_manager {
		private:
			struct is_entity_alive {
				inline bool operator()(entity e) { return e.index != std::uint32_t(-1); }
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

			entity create();

			bool is_alive(entity e) const;

			void destroy(entity e);

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
			std::vector<std::uint32_t> destroyed_entities_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
