#include "DocumentPresenter.hpp"

#include <iscore/plugins/documentdelegate/DocumentDelegateModelInterface.hpp>
#include <iscore/plugins/documentdelegate/DocumentDelegateFactoryInterface.hpp>

#include <core/document/DocumentView.hpp>
#include <core/document/DocumentModel.hpp>


using namespace iscore;

DocumentPresenter::DocumentPresenter(DocumentDelegateFactoryInterface* fact,
                                     DocumentModel* m,
                                     DocumentView* v,
                                     QObject* parent) :
    NamedObject {"DocumentPresenter", parent},
            m_view{v},
            m_model{m},
            m_presenter{fact->makePresenter(this,
                                            m_model->modelDelegate(),
                                            m_view->viewDelegate())}
{
}

