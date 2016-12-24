#include <chrono>

#include <boost/algorithm/string.hpp>

#include <meta_data_indexer.hpp>
#include <meta_data_parser.hpp>

int main(int argc, char const* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    meta_data_indexer indexer;
    for (int i = 1; i < argc; ++i) {
        try {
            if (!boost::algorithm::ends_with(argv[i], "reflect/reflect.hpp"))
                indexer.index(argv[i]);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    indexer.print();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;
    return 0;
}
