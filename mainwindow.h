#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

struct Order
{
    double price = 0;
    double amount = 0;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buyBtn_clicked();

    void on_sellBtn_clicked();

    void on_dealBtn_clicked();

private:
    Ui::MainWindow *ui;

    QVector<Order> buyOrders;
    QVector<Order> sellOrders;
    void sortBuyOrders();
    void sortSellOrders();

    void showBuyOrders();
    void showSellOrders();
};

#endif // MAINWINDOW_H
