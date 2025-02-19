#pragma once
#include <QObject>
#include <iscore/plugins/settingsdelegate/SettingsDelegatePresenterInterface.hpp>

class PluginSettingsModel;
class PluginSettingsView;
class BlacklistCommand;
class PluginSettingsPresenter : public iscore::SettingsDelegatePresenterInterface
{
        Q_OBJECT
    public:
        PluginSettingsPresenter(iscore::SettingsPresenter* parent,
                                iscore::SettingsDelegateModelInterface* model,
                                iscore::SettingsDelegateViewInterface* view);

        void on_accept() override;
        void on_reject() override;

        QString settingsName() override
        {
            return tr("Plugin");
        }

        QIcon settingsIcon() override;

        void load();
        PluginSettingsModel* model();
        PluginSettingsView* view();

    public slots:
        void setBlacklistCommand(BlacklistCommand* cmd);

    private:

        // S'il y avait plusieurs contrôles chaque contrôle devrait avoir sa "commande".
        iscore::Command* m_blacklistCommand {nullptr};
};
