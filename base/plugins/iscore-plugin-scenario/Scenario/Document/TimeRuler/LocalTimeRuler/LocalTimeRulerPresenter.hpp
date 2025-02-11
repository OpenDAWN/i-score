#pragma once

#include <Scenario/Document/TimeRuler/AbstractTimeRuler.hpp>
#include <Scenario/Document/TimeRuler/LocalTimeRuler/LocalTimeRulerView.hpp>

#include <Process/TimeValue.hpp>

//TODO : total refactor => unused now

class LocalTimeRulerPresenter final : public AbstractTimeRuler
{
    public:
        LocalTimeRulerPresenter(LocalTimeRulerView* view, QObject *parent);
        LocalTimeRulerPresenter(LocalTimeRulerView* view, TimeValue startDate, TimeValue duration, double pixPerMillis, QObject *parent);
        ~LocalTimeRulerPresenter();

    public slots:
        void setStartPoint(TimeValue dur) override;
};
