#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QObject>
#include <QTimer>

class Analysis : public QObject
{
    Q_OBJECT

public:
    static Analysis *Instance();
    explicit Analysis(QObject *parent = nullptr);

public slots:
    void start(int s);

signals:

private slots:
    void AnalysisHandle();

private:
    static QScopedPointer<Analysis> self;

    QTimer *m_time;

    void requestHandle();
    void resultHandle();

    bool json2xml(const QString &s);
    bool xml2json(const QString &s);
};

#endif // ANALYSIS_H
