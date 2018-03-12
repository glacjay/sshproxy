#include "Indicator.hpp"

Indicator::Indicator(QWidget* parent)
    :QWidget(parent)
    , mState(State::Off)
{
    setFixedSize(28, 28);
    setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
}

void Indicator::setState(State state) noexcept
{
    if(state != mState)
    {
        mState = state;
        emit stateChanged();
        update();
    }
}

void Indicator::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)
        QPainter painter(this);
        QRect r = rect();
        r.setWidth(r.height());
        painter.setPen(Qt::darkGray);
        painter.setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
        painter.drawRect(r);
        painter.setPen(Qt::darkGray);
        if(mState == State::On)
        {
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        }
        else if (mState == State::Busy)
        {
            painter.setBrush(QBrush(Qt::green, Qt::Dense4Pattern));
        }
        else
        {
            painter.setBrush(QBrush(Qt::lightGray, Qt::Dense4Pattern));
        }
        r.setHeight((r.height() - 2));
        r.setWidth(r.height());
        r.setX(1);
        r.setY(1);
        painter.drawRect(r);
    }
