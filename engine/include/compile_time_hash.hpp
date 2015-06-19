#ifndef SIGMAFIVE_COMPILE_TIME_HASH_HPP
#define SIGMAFIVE_COMPILE_TIME_HASH_HPP

//http://www.cse.yorku.ca/~oz/hash.html
constexpr const unsigned long compile_time_hash(const char *input) {
	return *input ? static_cast<unsigned long>(*input) + 33 * compile_time_hash(input + 1) : 5381;
}

#endif //SIGMAFIVE_COMPILE_TIME_HASH_HPP