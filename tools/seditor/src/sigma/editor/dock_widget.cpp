#include <sigma/editor/dock_widget.hpp>

#include <QMainWindow>

namespace sigma {
namespace editor {
    dock_widget::dock_widget(const QString& title, QWidget* parent,
        Qt::WindowFlags flags)
        : QDockWidget(title, parent, flags)
        , blank_title(new QWidget(this))
    {

        if (title.size())
            setObjectName(title);

        connect(this, &dock_widget::dockLocationChanged, this, &dock_widget::on_visibility_changed);
        connect(this, &dock_widget::topLevelChanged, this, &dock_widget::on_top_level_changed);
        connect(this, &dock_widget::tabified_dock_widget, this, &dock_widget::on_tab_added_or_removed);
        connect(this, &dock_widget::untabified_dock_widget, this, &dock_widget::on_tab_added_or_removed);
    }

    dock_widget::dock_widget(QWidget* parent, Qt::WindowFlags flags)
        : dock_widget("", parent, flags)
    {
    }

    dock_widget::~dock_widget()
    {
        disconnect(this, &dock_widget::dockLocationChanged, this, &dock_widget::on_visibility_changed);
        disconnect(this, &dock_widget::topLevelChanged, this, &dock_widget::on_top_level_changed);
        disconnect(this, &dock_widget::tabified_dock_widget, this, &dock_widget::on_tab_added_or_removed);
        disconnect(this, &dock_widget::untabified_dock_widget, this, &dock_widget::on_tab_added_or_removed);
    }

    void dock_widget::on_visibility_changed(Qt::DockWidgetArea visible)
    {
        on_top_level_changed(isTopLevel());
    }

    void dock_widget::on_top_level_changed(bool topLevel)
    {
        if (topLevel) {
            for (auto tab : other_tabs) {
                tab->remove_tab(this);
                emit untabified_dock_widget(tab);
            }
            other_tabs.clear();
        } else {
            auto window = dynamic_cast<QMainWindow*>(parent()); // TODO check
            if (!window)
                return;
            auto docked_with = window->tabifiedDockWidgets(this);
            for (auto docked : docked_with) {
                auto tab = dynamic_cast<dock_widget*>(docked);
                if (tab) {
                    this->add_tab(tab);
                    tab->add_tab(this);
                }
            }
        }
    }

    void dock_widget::on_tab_added_or_removed(dock_widget* widget)
    {
        if (other_tabs.size() == 0 || isTopLevel())
            setTitleBarWidget(nullptr);
        else
            setTitleBarWidget(blank_title);
    }

    void dock_widget::add_tab(dock_widget* tab)
    {
        auto it = std::find(other_tabs.cbegin(), other_tabs.cend(), tab);
        if (it == other_tabs.cend()) {
            other_tabs.push_back(tab);
            emit tabified_dock_widget(tab);
        }
    }

    void dock_widget::remove_tab(dock_widget* tab)
    {
        other_tabs.remove(tab);
        emit untabified_dock_widget(tab);
    }
}
}
