#pragma once

#include <QDockWidget>

#include <list>

namespace sigmafive {
namespace editor {
    class dock_widget : public QDockWidget {
        Q_OBJECT
    public:
        explicit dock_widget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

        explicit dock_widget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

        virtual ~dock_widget();

    signals:

        void tabified_dock_widget(dock_widget* widget);

        void untabified_dock_widget(dock_widget* widget);

    private:
        std::list<dock_widget*> other_tabs;
        QWidget* blank_title;

        void on_visibility_changed(Qt::DockWidgetArea visible);

        void on_top_level_changed(bool topLevel);

        void on_tab_added_or_removed(dock_widget* widget);

        void add_tab(dock_widget* tab);

        void remove_tab(dock_widget* tab);
    };
}
}
