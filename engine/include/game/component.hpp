#ifndef SIGMAFIVE_GAME_COMPONENT_HPP
#define SIGMAFIVE_GAME_COMPONENT_HPP

#include <bitset>
#include <cstdint>
#include <type_traits>
#include <boost/preprocessor.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>

#define SIGMAFIVE_COMPONENT_()
#define SIGMAFIVE_COMPONENT_FOR_EACH_BODY(r, data, elem) BOOST_PP_CAT(data,elem)()
#define SIGMAFIVE_COMPONENT(...) public: static const std::uint64_t ID; BOOST_PP_SEQ_FOR_EACH(SIGMAFIVE_COMPONENT_FOR_EACH_BODY,SIGMAFIVE_COMPONENT_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) private:
#define SIGMAFIVE_IMPLEMENT_COMPONENT(C) const std::uint64_t C::ID = sigmafive::game::detail::create_component_id(); BOOST_CLASS_EXPORT(C);

namespace sigmafive {
	namespace game {
		//TODO create a dynamic bitset to avoid this max
		static constexpr const std::uint64_t MAX_NUM_COMPONENTS = 64;
		typedef std::bitset<MAX_NUM_COMPONENTS> bitset;

		class component {
			SIGMAFIVE_COMPONENT();
		public:
			virtual ~component();
		};

		namespace detail {
			std::uint64_t create_component_id();

			template<typename C,typename ... Rest>
			struct bitset_builder {
				bitset operator()() const {
					return bitset_builder<C>{}() | bitset_builder<Rest...>{}();
				}
			};

			template<typename C>
			struct bitset_builder<C> {
				bitset operator()() const {
					bitset b;
					b.set(C::ID,true);
					return b;
				}
			};

		}
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_HPP
