#pragma once
#include <ProcessModel/OSSIAProcessModel.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include "SimpleProcess/SimpleProcess.hpp"


class SimpleProcessModel final : public OSSIAProcessModel
{
        ISCORE_SERIALIZE_FRIENDS(SimpleProcessModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(SimpleProcessModel, JSONObject)

    public:
        explicit SimpleProcessModel(
                const TimeValue& duration,
                const Id<Process>& id,
                QObject* parent);

        explicit SimpleProcessModel(
                const SimpleProcessModel& source,
                const Id<Process>& id,
                QObject* parent);

        template<typename Impl>
        explicit SimpleProcessModel(
                Deserializer<Impl>& vis,
                QObject* parent) :
            OSSIAProcessModel{vis, parent},
            m_ossia_process{std::make_shared<SimpleProcess>()}
        {
            vis.writeTo(*this);
        }

        // Process interface
        SimpleProcessModel* clone(
                const Id<Process>& newId,
                QObject* newParent) const override;

        QString prettyName() const override;
        QByteArray makeLayerConstructionData() const override;

        const ProcessFactoryKey& key() const override
        {
            static const ProcessFactoryKey name{"SimpleProcessModel"};
            return name;
        }

        void setDurationAndScale(const TimeValue& newDuration) override;
        void setDurationAndGrow(const TimeValue& newDuration) override;
        void setDurationAndShrink(const TimeValue& newDuration) override;

        void startExecution() override;
        void stopExecution() override;
        void reset() override;

        ProcessStateDataInterface* startState() const override;
        ProcessStateDataInterface* endState() const override;

        Selection selectableChildren() const override;
        Selection selectedChildren() const override;
        void setSelection(const Selection& s) const override;

        void serialize(const VisitorVariant& vis) const override;

        // OSSIAProcessModel interface
        std::shared_ptr<TimeProcessWithConstraint> process() const override
        {
            return m_ossia_process;
        }

    protected:
        LayerModel* makeLayer_impl(const Id<LayerModel>& viewModelId, const QByteArray& constructionData, QObject* parent) override;
        LayerModel* loadLayer_impl(const VisitorVariant&, QObject* parent) override;
        LayerModel* cloneLayer_impl(const Id<LayerModel>& newId, const LayerModel& source, QObject* parent) override;

    private:
        std::shared_ptr<TimeProcessWithConstraint> m_ossia_process;
};
