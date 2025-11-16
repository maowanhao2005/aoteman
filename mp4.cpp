#include "mp4.h"
#include <QStatusBar>

Mp4::Mp4(QWidget *parent)
    : QMainWindow(parent)
    , mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget(this))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , audioOutput(new QAudioOutput(this))
#endif
{
    setupUI();
    setupConnections();

    // 初始状态
    updateButtonStates(false, false, false);
    positionSlider->setEnabled(false);

    setWindowTitle("idokeno MP4播放器");
    resize(800, 600);
}

Mp4::~Mp4()
{
}

void Mp4::setupUI()
{
    // 创建中央部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建按钮
    openButton = new QPushButton("打开文件", this);
    playButton = new QPushButton("播放", this);
    pauseButton = new QPushButton("暂停", this);
    stopButton = new QPushButton("停止", this);

    // 创建滑块
    positionSlider = new QSlider(Qt::Horizontal, this);
    positionSlider->setRange(0, 0);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setFixedWidth(100);

    // 创建标签
    timeLabel = new QLabel("00:00 / 00:00", this);
    timeLabel->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("就绪", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: blue;");

    // 创建布局
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addStretch();
    controlLayout->addWidget(new QLabel("音量:", this));
    controlLayout->addWidget(volumeSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(videoWidget, 1);
    mainLayout->addWidget(positionSlider);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(controlLayout);
}

void Mp4::setupConnections()
{
    // 连接按钮信号
    connect(openButton, &QPushButton::clicked, this, &Mp4::openFile);
    connect(playButton, &QPushButton::clicked, this, &Mp4::play);
    connect(pauseButton, &QPushButton::clicked, this, &Mp4::pause);
    connect(stopButton, &QPushButton::clicked, this, &Mp4::stop);

    // 连接滑块信号
    connect(positionSlider, &QSlider::sliderMoved, this, &Mp4::seek);
    connect(volumeSlider, &QSlider::valueChanged, this, &Mp4::setVolume);

    // 连接媒体播放器信号
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &Mp4::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &Mp4::updateDuration);

    // Qt版本兼容性处理
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6设置音频输出
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setVideoOutput(videoWidget);
    audioOutput->setVolume(0.5);  // 设置初始音量

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &Mp4::handleMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        switch (state) {
        case QMediaPlayer::PlayingState:
            updateButtonStates(false, true, true);
            statusLabel->setText("播放中");
            break;
        case QMediaPlayer::PausedState:
            updateButtonStates(true, false, true);
            statusLabel->setText("已暂停");
            break;
        case QMediaPlayer::StoppedState:
            updateButtonStates(true, false, false);
            statusLabel->setText("已停止");
            positionSlider->setValue(0);
            timeLabel->setText("00:00 / 00:00");
            break;
        }
    });
#else
    // Qt5设置视频输出
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setVolume(50);  // Qt5使用setVolume

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &Mp4::handleMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        switch (state) {
        case QMediaPlayer::PlayingState:
            updateButtonStates(false, true, true);
            statusLabel->setText("播放中");
            break;
        case QMediaPlayer::PausedState:
            updateButtonStates(true, false, true);
            statusLabel->setText("已暂停");
            break;
        case QMediaPlayer::StoppedState:
            updateButtonStates(true, false, false);
            statusLabel->setText("已停止");
            positionSlider->setValue(0);
            timeLabel->setText("00:00 / 00:00");
            break;
        }
    });
#endif
}

void Mp4::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开媒体文件",
                                                    "",
                                                    "视频文件 (*.mp4 *.avi *.mkv *.mov *.wmv);;"
                                                    "所有文件 (*.*)");
    if (!fileName.isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
#else
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
#endif
        playButton->setEnabled(true);
        positionSlider->setEnabled(true);

        // 更新窗口标题显示文件名
        QFileInfo fileInfo(fileName);
        setWindowTitle("idokeno MP4播放器 - " + fileInfo.fileName());
        statusLabel->setText("文件已加载: " + fileInfo.fileName());
    }
}

void Mp4::play()
{
    mediaPlayer->play();
}

void Mp4::pause()
{
    mediaPlayer->pause();
}

void Mp4::stop()
{
    mediaPlayer->stop();
}

void Mp4::seek(int position)
{
    mediaPlayer->setPosition(position);
}

void Mp4::setVolume(int volume)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6: 音量范围是0.0到1.0
    audioOutput->setVolume(volume / 100.0);
#else
    // Qt5: 音量范围是0到100
    mediaPlayer->setVolume(volume);
#endif
}

void Mp4::updatePosition(qint64 position)
{
    if (!positionSlider->isSliderDown()) {
        positionSlider->setValue(position);
    }

    QTime currentTime(0, 0, 0);
    currentTime = currentTime.addMSecs(position);
    QTime totalTime(0, 0, 0);
    totalTime = totalTime.addMSecs(mediaPlayer->duration());

    QString timeStr = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    timeLabel->setText(timeStr);
}

void Mp4::updateDuration(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void Mp4::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::LoadingMedia:
        statusLabel->setText("加载中...");
        break;
    case QMediaPlayer::LoadedMedia:
        statusLabel->setText("已加载");
        break;
    case QMediaPlayer::BufferingMedia:
        statusLabel->setText("缓冲中...");
        break;
    case QMediaPlayer::BufferedMedia:
        statusLabel->setText("就绪");
        break;
    case QMediaPlayer::EndOfMedia:
        statusLabel->setText("播放结束");
        updateButtonStates(true, false, false);
        break;
    case QMediaPlayer::InvalidMedia:
        statusLabel->setText("无效媒体文件");
        break;
    default:
        break;
    }
}

void Mp4::updateButtonStates(bool playEnabled, bool pauseEnabled, bool stopEnabled)
{
    playButton->setEnabled(playEnabled);
    pauseButton->setEnabled(pauseEnabled);
    stopButton->setEnabled(stopEnabled);
}
