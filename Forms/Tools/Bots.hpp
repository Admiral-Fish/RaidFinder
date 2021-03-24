#ifndef BOTS_H
#define BOTS_H

#include <QDialog>
#include <QTcpSocket>
#include "../../Core/Bots/Workers/RaidFinder.hpp"
#include "../../Core/Bots/Workers/StarFinder.hpp"

namespace Ui {
class Bots;
}

class Bots : public QDialog
{
    Q_OBJECT

public:
    explicit Bots(QWidget *parent = nullptr);
    ~Bots() override;

private:
    Ui::Bots *ui;

    QTcpSocket *socket;
    RaidFinder raidFinder;
    StarFinder starFinder;

    int denID;
    int denType;
    int species;
    int starsMin;
    int starsMax;
    bool gmax;
    int shinyLock;

    bool threadRunning = false;

    void setupModels();

public slots:
    void setDenInfo(int denID, int denType, int species, int starsMin, int starsMax, bool gmax, int shinyLock);

private slots:
    void startScript();
    void addProfile();
    void botFinished();
    void log(QString log);

signals:
    void generate(QString seed);
    void generated(bool results);
    void getDenInfo();
    void updateProfiles();
    void lockBoxes(bool location, bool den, bool rarity, bool species, bool seed);
    void unlockBoxes();
};

#endif // BOTS_H
