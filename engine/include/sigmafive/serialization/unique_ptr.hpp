#ifndef SIGMAFIVE_SERIALIZATION_UNIQUE_PTR_HPP
#define SIGMAFIVE_SERIALIZATION_UNIQUE_PTR_HPP

#include <memory>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/wrapper.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/serialization.hpp>

namespace boost {
	namespace serialization {

		template<class Archive, class T>
		inline void save(Archive  &ar, const std::unique_ptr<T> &ptr, const unsigned int) {
			T *raw_ptr = ptr.get();
			ar << BOOST_SERIALIZATION_NVP(raw_ptr);
		}

		template<class Archive, class T>
		inline void load(Archive & ar, std::unique_ptr<T> &ptr, const unsigned int) {
			T *raw_ptr = nullptr;
			ar >> BOOST_SERIALIZATION_NVP(raw_ptr);
			ptr.reset(raw_ptr);
		}

		template<class Archive, class T>
		inline void serialize(Archive & ar, std::unique_ptr<T> &ptr, const unsigned int format_version) {
			boost::serialization::split_free(ar, ptr, format_version);
		}
	}
}

#endif //SIGMAFIVE_SERIALIZATION_UNIQUE_PTR_HPP