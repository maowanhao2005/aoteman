#ifndef MP4_H
#define MP4_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTime>
#include <QAudioOutput>  // Qt6的音频输出

class Mp4 : public QMainWindow
{
    Q_OBJECT

public:
    Mp4(QWidget *parent = nullptr);
    ~Mp4();

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void seek(int position);
    void setVolume(int volume);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void setupUI();
    void setupConnections();
    void updateButtonStates(bool playEnabled, bool pauseEnabled, bool stopEnabled);

    // 媒体播放组件
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QAudioOutput *audioOutput;  // Qt6需要单独的音频输出
#endif

    // UI控件
    QWidget *centralWidget;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QSlider *positionSlider;
    QSlider *volumeSlider;
    QLabel *timeLabel;
    QLabel *statusLabel;
};

#endif // MP4_H
