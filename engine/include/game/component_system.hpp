#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP

#include <game/entity.hpp>
#include <game/component.hpp>
#include <boost/preprocessor.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>

#define SIGMAFIVE_COMPONENT_SYSTEM_()
#define SIGMAFIVE_COMPONENT_SYSTEM_FOR_EACH_BODY(r, data, elem) BOOST_PP_CAT(data,elem)()
#define SIGMAFIVE_COMPONENT_SYSTEM(...) public: static const std::uint64_t ID; BOOST_PP_SEQ_FOR_EACH(SIGMAFIVE_COMPONENT_SYSTEM_FOR_EACH_BODY,SIGMAFIVE_COMPONENT_SYSTEM_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) private:
#define SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(C) const std::uint64_t C::ID = sigmafive::game::detail::create_component_system_id(); BOOST_CLASS_EXPORT(C);

namespace sigmafive {
	namespace game {
		class entity_system;

		class component_system {
			SIGMAFIVE_COMPONENT_SYSTEM();
		public:
			virtual ~component_system();

			virtual bool is_intrested(entity e) const = 0;

			virtual void entity_added(entity e) = 0;

			virtual void entity_removed(entity e) = 0;

			virtual void process(entity_system &entity_system) = 0;
		private:
			friend class boost::serialization::access;
			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version) {
			}
		};

		namespace detail {
			std::uint64_t create_component_system_id();
		}
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
