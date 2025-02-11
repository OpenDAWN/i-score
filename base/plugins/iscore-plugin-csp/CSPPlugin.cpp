#include "CSPPlugin.hpp"
#include "CSPControl.hpp"
#include "MoveEventCSPFactory.hpp"
#include "MoveEventCSPFlexFactory.hpp"

iscore_plugin_csp::iscore_plugin_csp() :
    QObject {}
{
}

iscore::PluginControlInterface* iscore_plugin_csp::make_control(iscore::Presenter* pres)
{
    return new CSPControl{pres};
}

QVector<iscore::FactoryInterface*> iscore_plugin_csp::factories(const QString& factoryName)
{
    if(factoryName == MoveEventFactoryInterface::factoryName())
    {
        return {new MoveEventCSPFactory, new MoveEventCSPFlexFactory};
    }

    return {};
}
