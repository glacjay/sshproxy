#ifndef HEADER__INDICATOR_HPP
#define HEADER__INDICATOR_HPP

#include <QWidget>
#include <QPainter>

class Indicator : public QWidget
{
public:
    enum class State
    {
        Off,
        On,
        Busy
    };

private:
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
    explicit Indicator(QWidget* parent=nullptr);

    inline State state() const noexcept;

public slots:
    void setState(State state) noexcept;

signals:
    void stateChanged();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Indicator(const Indicator&)=delete;
    Indicator& operator=(const Indicator&)=delete;

private:
    State mState;
};

inline Indicator::State Indicator::state() const noexcept
{
    return mState;
}


#endif // HEADER__INDICATOR_HPP
