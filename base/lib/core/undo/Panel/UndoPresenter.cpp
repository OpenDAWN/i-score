#include "UndoPresenter.hpp"
#include "UndoView.hpp"
#include "UndoPanelId.hpp"
#include <iscore/plugins/panel/PanelModel.hpp>
#include <core/document/DocumentPresenter.hpp>
#include <core/document/DocumentModel.hpp>

UndoPresenter::UndoPresenter(
        iscore::Presenter* parent_presenter,
        iscore::PanelView* view) :
    iscore::PanelPresenter{parent_presenter, view}
{
}


int UndoPresenter::panelId() const
{
    return UNDO_PANEL_ID;
}


void UndoPresenter::on_modelChanged()
{
    if (model())
    {
        auto doc = safe_cast<iscore::Document *>(model()->parent()->parent());
        safe_cast<UndoView *>(view())->setStack(&doc->commandStack());
    }
    else
    {
        safe_cast<UndoView *>(view())->setStack(nullptr);
    }
}
