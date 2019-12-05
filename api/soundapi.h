#ifndef SOUNDAPI_H
#define SOUNDAPI_H

#include <QObject>
#include <QTimer>

#ifdef __arm__
#include <QProcess>
#else
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtMultimedia>
#else
#include <Phonon>
#endif
#endif

class SoundAPI : public QObject
{
	Q_OBJECT
public:
    static SoundAPI *Instance();
    explicit SoundAPI(QObject *parent = 0);

private:
    static QScopedPointer<SoundAPI> self;

    int playCount;          //播放次数
    bool playAlarmSound;    //是否播放声音
    QString soundPath;      //声音文件目录
    QString soundName;      //声音文件名称
    int isPlayCount;        //已播放的次数
    QTimer *timer;          //定时器播放

#ifdef __arm__
	QProcess *process;
#else
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
	QMediaPlayer *player;
#else     
	Phonon::MediaObject *player;
#endif
#endif

private slots:
	void playSound();

public slots:
    //设置播放次数
    void setPlayCount(int playCount);

    //设置是否播放声音
    void setPlayAlarmSound(bool playAlarmSound);

    //设置声音文件目录
    void setSoundPath(const QString &soundPath);

    //播放声音文件
    void play(const QString &soundName);

    //启动定时器播放
	void start();

    //停止播放
	void stop();
};

#endif // SOUNDAPI_H
