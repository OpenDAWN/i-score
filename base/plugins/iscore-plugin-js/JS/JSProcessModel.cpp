#include "JSProcessModel.hpp"
#include <DummyProcess/DummyLayerModel.hpp>
#include <core/document/Document.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <core/document/DocumentModel.hpp>


std::shared_ptr<JSProcess> JSProcessModel::makeProcess() const
{
    return std::make_shared<JSProcess>(*iscore::IDocument::documentFromObject(*this)->model().pluginModel<DeviceDocumentPlugin>());
}

JSProcessModel::JSProcessModel(
        const TimeValue& duration,
        const Id<Process>& id,
        QObject* parent):
    OSSIAProcessModel{duration, id, JSProcessMetadata::processObjectName(), parent},
    m_ossia_process{makeProcess()}
{
    pluginModelList = new iscore::ElementPluginModelList{
                      iscore::IDocument::documentFromObject(parent),
                      this};

    m_script = "(function(t) { \n"
               "     var obj = new Object; \n"
               "     obj[\"address\"] = 'OSCdevice:/millumin/layer/x/instance'; \n"
               "     obj[\"value\"] = t + iscore.value('OSCdevice:/millumin/layer/y/instance'); \n"
               "     return [ obj ]; \n"
               "});";
}

JSProcessModel::JSProcessModel(
        const JSProcessModel& source,
        const Id<Process>& id,
        QObject* parent):
    OSSIAProcessModel{source.duration(), id, JSProcessMetadata::processObjectName(), parent},
    m_ossia_process{makeProcess()},
    m_script{source.m_script}
{
    pluginModelList = new iscore::ElementPluginModelList{
                      *source.pluginModelList,
                      this};
}

void JSProcessModel::setScript(const QString& script)
{
    m_script = script;
    m_ossia_process->setTickFun(m_script);
}

JSProcessModel* JSProcessModel::clone(
        const Id<Process>& newId,
        QObject* newParent) const
{
    return new JSProcessModel{*this, newId, newParent};
}

QString JSProcessModel::prettyName() const
{
    return "Javascript Process";
}

QByteArray JSProcessModel::makeLayerConstructionData() const
{
    return {};
}

void JSProcessModel::setDurationAndScale(const TimeValue& newDuration)
{
    setDuration(newDuration);
}

void JSProcessModel::setDurationAndGrow(const TimeValue& newDuration)
{
    setDuration(newDuration);
}

void JSProcessModel::setDurationAndShrink(const TimeValue& newDuration)
{
    setDuration(newDuration);
}

void JSProcessModel::startExecution()
{
}

void JSProcessModel::stopExecution()
{
}

void JSProcessModel::reset()
{
}

ProcessStateDataInterface* JSProcessModel::startState() const
{
    return nullptr;
}

ProcessStateDataInterface* JSProcessModel::endState() const
{
    return nullptr;
}

Selection JSProcessModel::selectableChildren() const
{
    return {};
}

Selection JSProcessModel::selectedChildren() const
{
    return {};
}

void JSProcessModel::setSelection(const Selection&) const
{
}

void JSProcessModel::serialize(const VisitorVariant& s) const
{
    serialize_dyn(s, *this);
}

LayerModel* JSProcessModel::makeLayer_impl(
        const Id<LayerModel>& viewModelId,
        const QByteArray& constructionData,
        QObject* parent)
{
    return new DummyLayerModel{*this, viewModelId, parent};
}

LayerModel* JSProcessModel::loadLayer_impl(
        const VisitorVariant& vis,
        QObject* parent)
{
    return deserialize_dyn(vis, [&] (auto&& deserializer)
    {
        auto autom = new DummyLayerModel{
                        deserializer, *this, parent};

        return autom;
    });
}

LayerModel* JSProcessModel::cloneLayer_impl(
        const Id<LayerModel>& newId,
        const LayerModel& source,
        QObject* parent)
{
    return new DummyLayerModel{safe_cast<const DummyLayerModel&>(source), *this, newId, parent};
}



// MOVEME
template<>
void Visitor<Reader<DataStream>>::readFrom(const JSProcessModel& proc)
{
    readFrom(*proc.pluginModelList);

    m_stream << proc.m_script;

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(JSProcessModel& proc)
{
    proc.pluginModelList = new iscore::ElementPluginModelList{*this, &proc};

    QString str;
    m_stream >> str;
    proc.setScript(str);

    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const JSProcessModel& proc)
{
    m_obj["PluginsMetadata"] = toJsonValue(*proc.pluginModelList);
    m_obj["Script"] = proc.script();
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(JSProcessModel& proc)
{
    Deserializer<JSONValue> elementPluginDeserializer(m_obj["PluginsMetadata"]);
    proc.pluginModelList = new iscore::ElementPluginModelList{elementPluginDeserializer, &proc};

    proc.setScript(m_obj["Script"].toString());
}

