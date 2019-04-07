#ifndef SIGMA_TOOLS_PACKAGER_HPP
#define SIGMA_TOOLS_PACKAGER_HPP

#include <sigma/tools/json_conversion.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/util/filesystem.hpp>


#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace sigma {
namespace tools {

    struct build_settings {
        static constexpr const char* GROUP = "build";
        std::filesystem::path build_directory;
        std::vector<std::filesystem::path> source_directories;

        void load_settings(std::shared_ptr<context> context, const Json::Value &settings)
        {
            json::from_json(context, settings["build_directory"], build_directory);
            json::from_json(context, settings["source_directories"], source_directories);
        }
    };

    class resource_loader {
    public:
        resource_loader(std::shared_ptr<context> ctx) {}

        virtual ~resource_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const = 0;

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) = 0;
    };

    class packager {
    public:
        packager(std::shared_ptr<context> ctx)
            : context_(ctx)
        {
        }

        template <class Loader>
        void add_loader()
        {
            this->loaders_.push_back(std::move(std::make_unique<Loader>(context_)));
        }

        void scan()
        {
            auto settings = context_->settings<build_settings>();
            for (const auto& loader : loaders_) {
                for (const auto& source_directory : settings->source_directories) {
                    std::filesystem::recursive_directory_iterator it{ source_directory };
                    std::filesystem::recursive_directory_iterator end;
                    for (; it != end; ++it) {
                        auto path = it->path();
                        if (sigma::filesystem::is_hidden(path)) {
                            if (std::filesystem::is_directory(path))
                                it.disable_recursion_pending();
                            continue;
                        }

                        auto ext = sigma::util::to_lower_copy(path.extension().string());
                        if (std::filesystem::is_regular_file(path) && loader->supports_filetype(ext)) {
                            try {
                                loader->load(source_directory, ext, path);
                            } catch (const std::exception& e) {
                                std::cerr << "error: " << e.what() << '\n';
                            }
                        }
                    }
                }
            }
            context_->write_database();
        }

    private:
        std::shared_ptr<context> context_;
        std::vector<std::unique_ptr<resource_loader>> loaders_;
    };
}
}

#endif // SIGMA_TOOLS_PACKAGER_HPP
