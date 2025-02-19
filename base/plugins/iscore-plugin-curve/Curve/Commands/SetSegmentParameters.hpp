#pragma once
#include <Curve/Commands/CurveCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

class CurveModel;
class CurveSegmentModel;
using SegmentParameterMap = QMap<Id<CurveSegmentModel>, QPair<double, double>>;
class SetSegmentParameters final : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(CurveCommandFactoryName(), SetSegmentParameters, "SetSegmentParameters")
    public:
        SetSegmentParameters(Path<CurveModel>&& model, SegmentParameterMap&& parameters);

        void undo() const override;
        void redo() const override;

        void update(Path<CurveModel>&& model, SegmentParameterMap&&  segments);

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<CurveModel> m_model;
        SegmentParameterMap m_new;
        QMap<
            Id<CurveSegmentModel>,
            QPair<
                boost::optional<double>,
                boost::optional<double>
            >
        > m_old;
};
