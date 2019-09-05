#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QTimerEvent>
#include <QObject>

class Element : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name READ name CONSTANT)
    Q_PROPERTY(QString message NOTIFY messageChanged READ message WRITE setStr)
    Q_PROPERTY(int lifeSpan NOTIFY lifeSpanChanged READ lifeSpan)
    Q_PROPERTY(QString detail NOTIFY lifeSpanChanged READ toString)

private:
    const QString m_name;
    QString m_message;
    int m_LifeSpan;
    int m_oneSecTimerId;

public:
    Q_SIGNAL void messageChanged();
    Q_SIGNAL void lifeSpanChanged();
    Q_SIGNAL void killMe();

    explicit Element(QObject *parent = nullptr) : QObject(parent){}
    explicit Element(QString name, QObject* parent = nullptr) : QObject(parent), m_name(name)
    {
        m_message = "I was born!";
        m_LifeSpan = qrand() % 30;
        m_oneSecTimerId = startTimer(1000);;
    }

    QString name() const { return m_name; }
    QString message() const { return m_message; }
    int lifeSpan() const { return m_LifeSpan; }

    Q_INVOKABLE void setStr(QString message) { m_message = message; }

    Q_INVOKABLE QString toString() {
        return QString("I'm %1. lifspan: %2.").arg(m_name).arg(m_LifeSpan);
    }

protected:
    void timerEvent(QTimerEvent * e)
    {
        if(e->timerId() == m_oneSecTimerId)
        {
            m_LifeSpan--;
            lifeSpanChanged();
            if(m_LifeSpan==3){
                m_message = "last 3 sec";
                messageChanged();
            }
            if(m_LifeSpan==0){
                m_message = "I'm die.";
                messageChanged();
                killMe();
            }
        }
    }

};

#endif // ELEMENT_H
