#ifndef BOTS_H
#define BOTS_H

#include <QDialog>
#include <QTcpSocket>
#include "../../Core/Bots/Workers/RaidFinder.hpp"

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
    RaidFinder thread;

    int denID;
    int location;
    int denType;

    void setupModels();

public slots:
    void setDenInfo(int denID, int location, int denType);

private slots:
    void startScript();

signals:
    void generate(QString seed);
    void generated(bool results);
    void getDenInfo();
};

#endif // BOTS_H
