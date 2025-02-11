#pragma once

#include <iscore/plugins/plugincontrol/PluginControlInterface.hpp>

namespace iscore
{
/**
 * @brief The UndoControl class
 *
 * Base class for the "fake" undo plugin,
 * which provides a undo panel.
 */
class UndoControl final : public iscore::PluginControlInterface
{
    public:
        UndoControl(iscore::Application& app, QObject* parent);
        ~UndoControl();

        void populateMenus(MenubarManager*) override;
        std::vector<OrderedToolbar> makeToolbars() override;

    private slots:
        void on_documentChanged(
                iscore::Document* olddoc,
                iscore::Document* newdoc) override;

    private:
        // Connections to keep for the running document.
        QList<QMetaObject::Connection> m_connections;

        QAction* m_undoAction{new QAction{this}};
        QAction* m_redoAction{new QAction{this}};
};
}
