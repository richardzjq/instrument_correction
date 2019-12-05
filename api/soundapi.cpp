#include "soundapi.h"
#include "qmutex.h"
#include "qapplication.h"

QScopedPointer<SoundAPI> SoundAPI::self;
SoundAPI *SoundAPI::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new SoundAPI);
        }
    }

    return self.data();
}

SoundAPI::SoundAPI(QObject *parent) : QObject(parent)
{
#ifdef __arm__
    process = new QProcess(this);
#else
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    player = new QMediaPlayer;
#else
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    player = new Phonon::MediaObject(this);
    Phonon::createPath(player, audioOutput);
#endif
#endif

    playCount = 1;
    playAlarmSound = true;
    soundPath = qApp->applicationDirPath();
    soundName = "";

    isPlayCount = 0;
    timer = new QTimer(this);
    timer->setInterval(10 * 1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(playSound()));
}

void SoundAPI::playSound()
{
    if (isPlayCount >= playCount - 1) {
        isPlayCount = 0;
        timer->stop();
        return;
    }

    play(soundName);
    isPlayCount++;
}

void SoundAPI::setPlayCount(int playCount)
{
    this->playCount = playCount;
}

void SoundAPI::setPlayAlarmSound(bool playAlarmSound)
{
    this->playAlarmSound = playAlarmSound;
}

void SoundAPI::setSoundPath(const QString &soundPath)
{
    this->soundPath = soundPath;
}

void SoundAPI::play(const QString &soundName)
{
    this->soundName = soundName;
    if (playAlarmSound) {
        if (!soundName.isEmpty() && soundName != "无") {
            QString name = QString("%1/sound/%2").arg(soundPath).arg(soundName);
#ifdef __arm__
            if (process->state() == QProcess::NotRunning) {
#ifdef arma9
                process->start(QString("aplay %1").arg(name));
#else
                process->start(QString("mpv %1").arg(name));
#endif
            }
#else
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
            if (player->state() == QMediaPlayer::StoppedState) {
                player->setMedia(QUrl::fromLocalFile(name));
                player->play();
            }
#else
            if (player->state() == Phonon::StoppedState) {
                Phonon::MediaSource source(name);
                player->setCurrentSource(source);
                player->play();
            }
#endif
#endif
        }
    }
}

void SoundAPI::start()
{
    if (!playAlarmSound || soundName == "无") {
        return;
    }

    timer->start();
}

void SoundAPI::stop()
{
#ifdef __arm__
    process->close();
#else
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    player->clear();
#endif
    player->stop();
#endif

    isPlayCount = 0;
    timer->stop();
}
