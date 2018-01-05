#ifndef SIGMA_TOOLS_PACKAGER_HPP
#define SIGMA_TOOLS_PACKAGER_HPP

#include <sigma/context.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace sigma {
namespace tools {

    struct package_settings {
        static constexpr const char* GROUP = "package";

        BOOST_HANA_DEFINE_STRUCT(
            package_settings,
            (boost::filesystem::path, build_directory),
            (std::vector<boost::filesystem::path>, source_directories));
    };

    template <class PackageSettings, class ContextType>
    class packager;

    template <class PackageSettings, class ContextType>
    class resource_loader {
    public:
        virtual ~resource_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const = 0;

        virtual void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) = 0;
    };

    template <class PackageSettings, class... Resources, class... Settings>
    class packager<PackageSettings, context<type_set<Resources...>, type_set<Settings...>>> {
    public:
        using package_settings_type = PackageSettings;
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
            this->loaders_.push_back(std::move(std::make_unique<Loader>(context_)));
        }

        void scan()
        {
            load_settings();

            for (const auto& loader : loaders_) {
                for (const auto& source_directory : settings_.source_directories) {
                    boost::filesystem::recursive_directory_iterator it{ source_directory };
                    boost::filesystem::recursive_directory_iterator end;
                    for (; it != end; ++it) {
                        auto path = it->path();
                        if (sigma::filesystem::is_hidden(path)) {
                            if (boost::filesystem::is_directory(path))
                                it.no_push();
                            continue;
                        }

                        auto ext = boost::algorithm::to_lower_copy(path.extension().string());
                        if (boost::filesystem::is_regular_file(path) && loader->supports_filetype(ext)) {
                            try {
                                loader->load(settings_, source_directory, ext, path);
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
        package_settings_type settings_;
        std::vector<std::unique_ptr<resource_loader<package_settings_type, context_type>>> loaders_;

        void load_settings()
        {
            auto settings_path = context_.get_cache_path() / "settings.json";
            std::ifstream file{ settings_path.c_str() };
            file >> json_settings_;

            json::from_json(context_, json_settings_["package"], settings_);
        }
    };
}
}

#endif // SIGMA_TOOLS_PACKAGER_HPP
