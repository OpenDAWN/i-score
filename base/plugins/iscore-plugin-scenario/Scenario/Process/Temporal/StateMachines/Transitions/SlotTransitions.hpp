#pragma once
#include <Scenario/Process/Temporal/StateMachines/ScenarioStateMachineBaseTransitions.hpp>

namespace Scenario
{
class ClickOnSlotOverlay_Transition final : public iscore::MatchedTransition<ClickOnSlotOverlay_Event>
{
    public:
        ClickOnSlotOverlay_Transition(Scenario::SlotState& state);

        Scenario::SlotState& state() const;

    protected:
        void onTransition(QEvent * ev) override;

    private:
        Scenario::SlotState& m_state;
};

class ClickOnSlotHandle_Transition final : public iscore::MatchedTransition<ClickOnSlotHandle_Event>
{
    public:
        ClickOnSlotHandle_Transition(Scenario::SlotState& state);

        Scenario::SlotState& state() const;

    protected:
        void onTransition(QEvent * ev) override;

    private:
        Scenario::SlotState& m_state;
};
}
