#ifndef RAIDFINDER_HPP
#define RAIDFINDER_HPP

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "../RaidBot.hpp"

class RaidFinder : public QThread
{
    Q_OBJECT
public:
    RaidFinder(QObject *parent = nullptr);
    ~RaidFinder();

    void startScript(QString ipRaw, QString portRaw, int denID, int location, int denType);

public slots:
    void generated(bool results);

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;

    bool abort = false;
    bool genned = false;
    bool results = false;
    bool restart = false;

    QString ipRaw;
    QString portRaw;

    int denType;
    int denID;
    int location;

signals:
    void generate(QString seed);

};

#endif // RAIDFINDER_HPP
