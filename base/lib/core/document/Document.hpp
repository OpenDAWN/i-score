#pragma once
#include <iscore/tools/NamedObject.hpp>
#include <iscore/selection/SelectionStack.hpp>
#include <iscore/locking/ObjectLocker.hpp>
#include <core/command/CommandStack.hpp>
#include <core/document/DocumentContext.hpp>

#include <core/document/DocumentBackupManager.hpp>

namespace iscore
{
class DocumentModel;
class DocumentPresenter;
class DocumentView;
class DocumentDelegateFactoryInterface;
class PanelFactory;
class PanelPresenter;
/**
     * @brief The Document class is the central part of the software.
     *
     * It is similar to the opened file in Word for instance, this is the
     * data on which i-score operates, further defined by the plugins.
     */
class Document final : public NamedObject
{
        Q_OBJECT
        friend class DocumentBuilder;
    public:
        ~Document();

        const Id<DocumentModel>& id() const;

        CommandStack& commandStack()
        { return m_commandStack; }

        SelectionStack& selectionStack()
        { return m_selectionStack; }

        ObjectLocker& locker()
        { return m_objectLocker; }

        DocumentContext& context()
        { return m_context; }

        DocumentModel& model() const
        { return *m_model; }

        DocumentPresenter& presenter() const
        { return *m_presenter; }

        DocumentView& view() const
        { return *m_view; }

        void setupNewPanel(PanelFactory* factory);
        void bindPanelPresenter(PanelPresenter*);


        QJsonObject saveDocumentModelAsJson();
        QByteArray saveDocumentModelAsByteArray();

        QJsonObject saveAsJson();
        QByteArray saveAsByteArray();

        DocumentBackupManager* backupManager() const
        { return m_backupMgr; }

        void setBackupMgr(DocumentBackupManager* backupMgr);

        QString docFileName() const;
        void setDocFileName(const QString &docFileName);

    signals:
        void fileNameChanged(const QString& newName);

    private:
        // These are to be constructed by DocumentBuilder.
        Document(
                const Id<DocumentModel>& id,
                DocumentDelegateFactoryInterface* type,
                QWidget* parentview,
                QObject* parent);

        Document(
                const QVariant& data,
                DocumentDelegateFactoryInterface* type,
                QWidget* parentview,
                QObject* parent);

        void init();

        CommandStack m_commandStack;
        SelectionStack m_selectionStack;
        ObjectLocker m_objectLocker;

        DocumentModel* m_model{};
        DocumentView* m_view{};
        DocumentPresenter* m_presenter{};

        DocumentBackupManager* m_backupMgr{};

        DocumentContext m_context;
};



}
