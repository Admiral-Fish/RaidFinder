#ifndef BOTS_H
#define BOTS_H

#include <QDialog>
#include <QTcpSocket>
#include "../MainWindow.hpp"

namespace Ui {
class Bots;
}

class Bots : public QDialog
{
    Q_OBJECT

public:
    explicit Bots(QWidget *parent = nullptr, Ui::MainWindow *mainui = nullptr);
    ~Bots() override;

private:
    Ui::Bots *ui;
    Ui::MainWindow *uimain;

    QTcpSocket *socket;

    void setupModels();

private slots:
    void startScript();

signals:
    void generate();
};

#endif // BOTS_H
