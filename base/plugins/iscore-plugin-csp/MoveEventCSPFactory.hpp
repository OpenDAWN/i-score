#pragma once

#include <Scenario/Commands/Scenario/Displacement/MoveEventFactoryInterface.hpp>
#include <Scenario/Commands/Scenario/Displacement/MoveEventList.hpp>


class MoveEventCSPFactory : public MoveEventFactoryInterface
{
        // MoveEventFactory interface
    public:
        SerializableMoveEvent* make(
                Path<ScenarioModel> &&scenarioPath,
                const Id<EventModel> &eventId,
                const TimeValue &newDate,
                ExpandMode mode) override;

        SerializableMoveEvent* make() override;

        int priority(MoveEventFactoryInterface::Strategy strategy) override
        {
            switch(strategy)
            {
                case MoveEventFactoryInterface::Strategy::MOVING:
                    return 10;
                    break;
                default:
                    return -1;// not suited for other strategies
                    break;
            }
        }

        QString name() const override
        { return "CSPFlex"; }
};
