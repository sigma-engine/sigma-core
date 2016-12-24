#ifndef SIGMA_ENGINE_META_DATA_INDEXER
#define SIGMA_ENGINE_META_DATA_INDEXER

#include <boost/filesystem/path.hpp>
#include <sigma/reflect/reflect.hpp>

void print(const enum_delecration& parent, std::size_t level = 0);

void print(const property_delecration& parent, std::size_t level = 0);

void print(const class_delecration& parent, std::size_t level = 0);

void print(const namespace_delecration& parent, std::size_t level = 0);

void print(const translation_unit& parent, std::size_t level = 0);

class meta_data_indexer {
public:
    bool index(boost::filesystem::path path);

    void print();

private:
    translation_unit global_translation_unit_;
};

#endif //SIGMA_ENGINE_META_DATA_INDEXER
