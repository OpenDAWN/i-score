#pragma once
#include <iscore/plugins/plugincontrol/PluginControlInterface.hpp>

class ClientSessionBuilder;
class ClientSession;
namespace iscore
{
class DocumentDelegatePluginModel;
}

#ifdef USE_ZEROCONF
class ZeroconfBrowser;
#endif

class NetworkControl : public iscore::PluginControlInterface
{
        Q_OBJECT

    public:
        NetworkControl(iscore::Application& app);
        void populateMenus(iscore::MenubarManager*) override;

    public slots:
        void setupClientConnection(QString ip, int port);
        void on_sessionBuilt(
                ClientSessionBuilder* sessionBuilder,
                ClientSession* builtSession);

    private:
        virtual iscore::DocumentDelegatePluginModel* loadDocumentPlugin(
                const QString& name,
                const VisitorVariant& var,
                iscore::Document* parent) override;

        iscore::Presenter* m_presenter {};
        ClientSessionBuilder* m_sessionBuilder{};

#ifdef USE_ZEROCONF
        ZeroconfBrowser* m_zeroconfBrowser{};
#endif
};
