#ifndef SIGMAFIVE_GAME_COMPONENT_HPP
#define SIGMAFIVE_GAME_COMPONENT_HPP

#include <bitset>
#include <cinttypes>
#include <boost/preprocessor.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#define SIGMAFIVE_COMPONENT_()
#define SIGMAFIVE_COMPONENT_FOR_EACH_BODY(r, data, elem) BOOST_PP_CAT(data,elem)()
#define SIGMAFIVE_COMPONENT(...) public: static const std::uint32_t ID; BOOST_PP_SEQ_FOR_EACH(SIGMAFIVE_COMPONENT_FOR_EACH_BODY,SIGMAFIVE_COMPONENT_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) private:
#define SIGMAFIVE_IMPLEMENT_COMPONENT(C) const std::uint32_t C::ID = sigmafive::game::detail::create_component_id(); BOOST_CLASS_EXPORT(C);

#define SIGMAFIVE_SERIALIZE_BASE(base) boost::serialization::make_nvp("base_object",boost::serialization::base_object<base>(*this));

namespace sigmafive {
	namespace game {
		//TODO create a dynamic bitset to avoid this max
		static constexpr const std::uint32_t MAX_NUM_COMPONENTS = 64;
		typedef std::bitset<MAX_NUM_COMPONENTS> bitset;

		class component {
			SIGMAFIVE_COMPONENT();
		public:
			component() = default;

			component(const component &) = default;

			component(component &&) = default;

			virtual ~component() = default;

			component &operator=(const component &) = default;

			component &operator=(component &&) = default;

			template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
			}
		};

		namespace detail {
			std::uint32_t create_component_id();
		}
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_HPP
