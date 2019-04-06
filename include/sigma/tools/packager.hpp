#ifndef SIGMA_TOOLS_PACKAGER_HPP
#define SIGMA_TOOLS_PACKAGER_HPP

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
        static constexpr const char* GROUP = "package";

        BOOST_HANA_DEFINE_STRUCT(
            build_settings,
            (std::filesystem::path, build_directory),
            (std::vector<std::filesystem::path>, source_directories));
    };

    template <class ContextType>
    class packager;

    template <class ContextType>
    class resource_loader {
    public:
        resource_loader(build_settings& settings, ContextType& ctx) {}

        virtual ~resource_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const = 0;

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) = 0;
    };

    template <class... Resources, class... Settings>
    class packager<context<type_set<Resources...>, type_set<Settings...>>> {
    public:
        using resource_set_type = resource_set<Resources...>;
        using settings_set_type = settings_set<Settings...>;
        using context_type = context<resource_set_type, settings_set_type>;

        packager(context_type& ctx)
            : context_(ctx)
        {
        }

        template <class Loader>
        void add_loader()
        {
            this->loaders_.push_back(std::move(std::make_unique<Loader>(settings_, context_)));
        }

        void scan()
        {
            load_build_settings();

            for (const auto& loader : loaders_) {
                for (const auto& source_directory : settings_.source_directories) {
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

            (context_.template get_cache<Resources>().save(), ...);

            settings_set_type::for_each([&](auto tag) {
                using settings_type = typename decltype(tag)::type;
                if (json_settings_.isMember(settings_type::GROUP)) {
                    try {
                        json::from_json(context_, json_settings_[settings_type::GROUP], context_.template get_settings<settings_type>());
                    } catch (const std::exception& e) {
                        std::cerr << "error: " << e.what() << '\n';
                    }
                }
            });
            // TODO save the settings.
        }

    private:
        context_type& context_;
        Json::Value json_settings_;
        build_settings settings_;
        std::vector<std::unique_ptr<resource_loader<context_type>>> loaders_;

        void load_build_settings()
        {
            auto settings_path = context_.get_cache_path() / "settings.json";
            std::ifstream file{ settings_path.c_str() };
            file >> json_settings_;

            json::from_json(context_, json_settings_["build"], settings_);
        }
    };
}
}

#endif // SIGMA_TOOLS_PACKAGER_HPP
