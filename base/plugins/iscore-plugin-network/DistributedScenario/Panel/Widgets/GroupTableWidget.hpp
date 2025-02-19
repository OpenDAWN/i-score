#pragma once
#include <QWidget>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <iscore/tools/ObjectPath.hpp>

class QTableWidget;
class GroupManager;
class Session;
class GroupTableCheckbox;
class Client;
class GroupTableWidget : public QWidget
{
    public:
        GroupTableWidget(const GroupManager* mgr, const Session* session, QWidget* parent);

        void setup();

    private:
        GroupTableCheckbox* findCheckbox(int i, Id<Client> theClient) const;

        void on_checkboxChanged(int i, int j, int state);

        QTableWidget* m_table{};
        const GroupManager* m_mgr;
        const Session* m_session;

        ObjectPath m_managerPath;
        CommandDispatcher<> m_dispatcher;
};
