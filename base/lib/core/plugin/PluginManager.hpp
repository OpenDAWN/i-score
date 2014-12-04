#pragma once
#include <QPluginLoader>
#include <QMap>

#include <core/processes/ProcessList.hpp>
#include <interface/autoconnect/Autoconnect.hpp>

#include <interface/plugins/PluginControlInterface_QtInterface.hpp>
#include <interface/plugins/Autoconnect_QtInterface.hpp>
#include <interface/plugins/PanelFactoryInterface_QtInterface.hpp>
#include <interface/plugins/DocumentDelegateFactoryInterface_QtInterface.hpp>
#include <interface/plugins/ProcessFactoryInterface_QtInterface.hpp>
#include <interface/plugins/SettingsDelegateFactoryInterface_QtInterface.hpp>
#include <interface/plugins/InspectorWidgetFactoryInterface_QtInterface.hpp>

#include <interface/autoconnect/Autoconnect.hpp>

namespace iscore
{

	using CommandList = std::vector<PluginControlInterface*>;
	using PanelList = std::vector<PanelFactoryInterface*>;
	using DocumentPanelList = std::vector<DocumentDelegateFactoryInterface*>;
	using SettingsList = std::vector<SettingsDelegateFactoryInterface*>;
	using InspectorList = std::vector<InspectorWidgetFactoryInterface*>;
	using AutoconnectList = std::vector<Autoconnect>;

	/**
	 * @brief The PluginManager class loads and keeps track of the plug-ins.
	 */
	class PluginManager : public QNamedObject
	{
			Q_OBJECT
			friend class Application;
		public:
			PluginManager(QObject* parent): 
				QNamedObject{parent, "PluginManager"},
				m_processList{this}
			{ 
			}

			~PluginManager()
			{
				clearPlugins();
			}

			QMap<QString, QObject*> availablePlugins() const
			{
				return m_availablePlugins;
			}

			void reloadPlugins();
			
			QStringList pluginsOnSystem() const
			{
				return m_pluginsOnSystem;
			}
			
			const InspectorList& inspectorFactoriesList() const
			{
				return m_inspectorList;
			}

		private:
			// We need a list for all the plug-ins present on the system even if we do not load them.
			// Else we can't blacklist / unblacklist plug-ins.
			QStringList m_pluginsOnSystem;

			void dispatch(QObject* plugin);
			void clearPlugins();

			QStringList pluginsBlacklist();

			// Here, the plug-ins that are effectively loaded.
			QMap<QString, QObject*> m_availablePlugins;

			AutoconnectList m_autoconnections; // TODO try unordered_set
			ProcessList  m_processList;
			CommandList  m_commandList;
			PanelList    m_panelList;
			DocumentPanelList m_documentPanelList;
			SettingsList m_settingsList;
			InspectorList m_inspectorList;
	};
}
