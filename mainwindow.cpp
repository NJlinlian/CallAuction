#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Call Auction Test");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buyBtn_clicked()
{
    if(ui->buyAmountLineEdit->text().toDouble() <= 0
            || ui->buyPriceLineEdit->text().toDouble() <= 0)
    {
        return;
    }

    Order order;
    order.amount = ui->buyAmountLineEdit->text().toDouble();
    order.price = ui->buyPriceLineEdit->text().toDouble();

    buyOrders.append(order);

    sortBuyOrders();
    showBuyOrders();
}

void MainWindow::on_sellBtn_clicked()
{
    if(ui->sellAmountLineEdit->text().toDouble() <= 0
            || ui->sellPriceLineEdit->text().toDouble() <= 0)
    {
        return;
    }

    Order order;
    order.amount = ui->sellAmountLineEdit->text().toDouble();
    order.price = ui->sellPriceLineEdit->text().toDouble();

    sellOrders.append(order);

    sortSellOrders();
    showSellOrders();
}

void MainWindow::sortBuyOrders()
{
    QVector<Order> tempOrders;
    foreach (Order order, buyOrders)
    {
        int size = tempOrders.size();
        int i = 0;
        for(; i < size; i++)
        {
            if(order.price == tempOrders.at(i).price)
            {
                // 合并相同价格的挂单
                tempOrders[i].amount += order.amount;
                break;
            }
            else if(order.price > tempOrders.at(i).price)
            {
                tempOrders.insert(i, order);
                break;
            }
        }
        if( i == size)
        {
            tempOrders.insert(i, order);
        }
    }

    buyOrders = tempOrders;
}

void MainWindow::sortSellOrders()
{
    QVector<Order> tempOrders;
    foreach (Order order, sellOrders)
    {
        int size = tempOrders.size();
        int i = 0;
        for(; i < size; i++)
        {
            if(order.price == tempOrders.at(i).price)
            {
                // 合并相同价格的挂单
                tempOrders[i].amount += order.amount;
                break;
            }
            else if(order.price < tempOrders.at(i).price)
            {
                tempOrders.insert(i, order);
                break;
            }
        }
        if( i == size)
        {
            tempOrders.insert(i, order);
        }
    }

    sellOrders = tempOrders;
}


void MainWindow::showBuyOrders()
{
    int size = buyOrders.size();
    ui->buyTableWidget->setRowCount(0);
    ui->buyTableWidget->setRowCount(size);

    for(int i = 0; i < size; i++)
    {
        Order order = buyOrders.at(i);
        ui->buyTableWidget->setItem(i,0, new QTableWidgetItem(QString::number(order.price)));
        ui->buyTableWidget->setItem(i,1, new QTableWidgetItem(QString::number(order.amount)));
    }
}

void MainWindow::showSellOrders()
{
    int size = sellOrders.size();
    ui->sellTableWidget->setRowCount(0);
    ui->sellTableWidget->setRowCount(size);

    for(int i = 0; i < size; i++)
    {
        Order order = sellOrders.at(i);
        ui->sellTableWidget->setItem(i,0, new QTableWidgetItem(QString::number(order.price)));
        ui->sellTableWidget->setItem(i,1, new QTableWidgetItem(QString::number(order.amount)));
    }
}

void MainWindow::on_dealBtn_clicked()
{
    double dealAmount = 0;
    double dealPrice = 0;

    while (buyOrders.size() > 0 && sellOrders.size() > 0
           && buyOrders.at(0).price >= sellOrders.at(0).price)
    {
        if(buyOrders.at(0).amount > sellOrders.at(0).amount)
        {
            dealAmount += sellOrders.at(0).amount;
            dealPrice = sellOrders.at(0).price;
            buyOrders[0].amount -= sellOrders.at(0).amount;
            sellOrders.removeFirst();
        }
        else if(buyOrders.at(0).amount < sellOrders.at(0).amount)
        {
            dealAmount += buyOrders.at(0).amount;
            dealPrice = sellOrders.at(0).price;
            sellOrders[0].amount -= buyOrders.at(0).amount;
            buyOrders.removeFirst();
        }
        else
        {
            // 如果买卖单第一档位数量相同
            dealAmount += buyOrders.at(0).amount;
            dealPrice = sellOrders.at(0).price;
            buyOrders.removeFirst();
            sellOrders.removeFirst();
        }
    }

    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(QString("Deal Amount: %1\nDeal Price: %2")
                                       .arg(dealAmount).arg(dealPrice));

    showBuyOrders();
    showSellOrders();
}

