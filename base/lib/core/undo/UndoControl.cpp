#include "UndoControl.hpp"
#include <core/document/DocumentPresenter.hpp>

iscore::UndoControl::UndoControl(iscore::Application& app, QObject* parent):
    iscore::PluginControlInterface{app, "UndoControl", parent}
{
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setEnabled(false);
    m_undoAction->setText(tr("Nothing to undo"));
    m_undoAction->setToolTip(tr("Ctrl+Z"));
    connect(m_undoAction, &QAction::triggered,
            [&] ()
    {
        currentDocument()->commandStack().undo();
    });

    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setEnabled(false);
    m_redoAction->setText(tr("Nothing to redo"));
    m_redoAction->setToolTip(tr("Ctrl+Shift+Z"));
    connect(m_redoAction, &QAction::triggered,
            [&] ()
    {
        currentDocument()->commandStack().redo();
    });
}

iscore::UndoControl::~UndoControl()
{
    for(auto& connection : m_connections)
    {
        disconnect(connection);
    }
}

void iscore::UndoControl::populateMenus(iscore::MenubarManager* menu)
{
    ////// Edit //////
    menu->addSeparatorIntoToplevelMenu(ToplevelMenuElement::EditMenu,
                                       EditMenuElement::Separator_Undo);
    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::EditMenu,
                                       EditMenuElement::Undo,
                                       m_undoAction);
    menu->insertActionIntoToplevelMenu(ToplevelMenuElement::EditMenu,
                                       EditMenuElement::Redo,
                                       m_redoAction);
}

std::vector<iscore::OrderedToolbar> iscore::UndoControl::makeToolbars()
{
    QToolBar* bar = new QToolBar;
    bar->addAction(m_undoAction);
    bar->addAction(m_redoAction);

    return std::vector<OrderedToolbar>{OrderedToolbar(3, bar)};
}

void iscore::UndoControl::on_documentChanged(
        iscore::Document* olddoc,
        iscore::Document* newDoc)
{
    using namespace iscore;

    // Cleanup
    for(auto& connection : m_connections)
        disconnect(connection);
    m_connections.clear();

    if(!newDoc)
    {
        m_undoAction->setEnabled(false);
        m_undoAction->setText(tr("Nothing to undo"));
        m_redoAction->setEnabled(false);
        m_redoAction->setText(tr("Nothing to redo"));
        return;
    }

    // Redo the connections
    auto stack = &newDoc->commandStack();
    m_connections.push_back(
                connect(stack, &CommandStack::canUndoChanged,
                        [&] (bool b) { m_undoAction->setEnabled(b); }));
    m_connections.push_back(
                connect(stack, &CommandStack::canRedoChanged,
                        [&] (bool b) { m_redoAction->setEnabled(b); }));

    m_connections.push_back(
                connect(stack, &CommandStack::undoTextChanged,
                        [&] (const QString& s) { m_undoAction->setText(tr("Undo ") + s);}));
    m_connections.push_back(
                connect(stack, &CommandStack::redoTextChanged,
                        [&] (const QString& s) { m_redoAction->setText(tr("Redo ") + s);}));

    // Set the correct values for the current document.
    m_undoAction->setEnabled(stack->canUndo());
    m_redoAction->setEnabled(stack->canRedo());

    m_undoAction->setText(stack->undoText());
    m_redoAction->setText(stack->redoText());
}
