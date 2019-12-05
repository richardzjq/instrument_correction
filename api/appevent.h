#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>

class ButtonDefence;

class AppEvent : public QObject
{
    Q_OBJECT
public:
    static AppEvent *Instance();
    explicit AppEvent(QObject *parent = 0);

private:
    static QScopedPointer<AppEvent> self;

signals:
    void changeStyle();
    void changeText();
    void doubleClicked(ButtonDefence *btn);

public slots:
    void slot_changeStyle();
    void slot_changeText();
    void slot_doubleClicked();
};

#endif // APPEVENT_H
