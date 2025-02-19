#pragma once
#include <iscore/tools/NamedObject.hpp>

namespace iscore
{
    class DocumentPresenter;
    class DocumentDelegateModelInterface;
    class DocumentDelegateViewInterface;

    class DocumentDelegatePresenterInterface : public NamedObject
    {
        public:
            DocumentDelegatePresenterInterface(DocumentPresenter* parent_presenter,
                                               const QString& object_name,
                                               DocumentDelegateModelInterface* model,
                                               DocumentDelegateViewInterface* view);

            virtual ~DocumentDelegatePresenterInterface();

        protected:
            DocumentDelegateModelInterface* m_model;
            DocumentDelegateViewInterface* m_view;
            DocumentPresenter* m_parentPresenter;
    };
}
