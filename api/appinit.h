#ifndef APPINIT_H
#define APPINIT_H

#include <QObject>

class DbCleanThread;

class AppInit : public QObject
{
    Q_OBJECT
public:
    static AppInit *Instance();
    explicit AppInit(QObject *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    static QScopedPointer<AppInit> self;
    DbCleanThread *cleanAlarmLog;
    DbCleanThread *cleanNodeLog;
    DbCleanThread *cleanUserLog;

public slots:
    void start();
    void initDb();
    void initStyle();
    void changeDbMaxCount();
};

#endif // APPINIT_H
