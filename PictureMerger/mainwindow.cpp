#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMessageBox>
#include <QStyle>
#include <QIcon>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowIcon(QIcon("icon.png"));
    setWindowTitle("Picture Merger");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString filePath = event->mimeData()->urls()[0].toLocalFile();
    QPixmap fullPixmap(filePath);

    int inputLabelsWidth = ui->firstPicture->width();
    int inputLabelsHeight = ui->firstPicture->height();

    QPixmap labelPixmap = fullPixmap.scaled(inputLabelsWidth, inputLabelsHeight, Qt::KeepAspectRatio);

    if (ui->firstPicture->geometry().contains(event->pos()))
    {
        MainWindow::firstPixmap = fullPixmap;
        ui->firstPicture->setPixmap(labelPixmap);
    }
    else if (ui->secondPicture->geometry().contains(event->pos()))
    {
        MainWindow::secondPixmap = fullPixmap;
        ui->secondPicture->setPixmap(labelPixmap);
    }
}

void MainWindow::on_btMerge_clicked()
{
    QPixmap resultPixmap = this->getResultPixmap();
    if (!resultPixmap.isNull())
    {
        QFileDialog dialog;
        QString savePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("Images (*.jpg)"));
        resultPixmap.save(savePath, "jpg");
    }
    else
    {
        showWarning();
    }
}

void MainWindow::on_btPreview_clicked()
{
    QPixmap resultPixmap = this->getResultPixmap();
    if (!resultPixmap.isNull())
    {
        int outputLabelWidth = ui->resultPicture->width();
        int outputLabelHeight = ui->resultPicture->height();
        ui->resultPicture->setPixmap(resultPixmap.scaled(outputLabelWidth, outputLabelHeight, Qt::KeepAspectRatio));
    }
    else
    {
        showWarning();
    }
}

QPixmap MainWindow::getResultPixmap()
{
    if (!firstPixmap.isNull() && !secondPixmap.isNull())
    {
        QPixmap rescaledFirstPixmap = firstPixmap;
        QPixmap rescaledSecondPixmap = secondPixmap;
        QPixmap resultPixmap;
        if (ui->rbtHorizontal->isChecked())
        {
            if ((ui->rbtBiggest->isChecked() && firstPixmap.height() < secondPixmap.height()) ||
                (ui->rbtSmallest->isChecked() && firstPixmap.height() > secondPixmap.height()))
                    rescaledFirstPixmap = firstPixmap.scaledToHeight(secondPixmap.height());
            else if ((ui->rbtBiggest->isChecked() && firstPixmap.height() > secondPixmap.height()) ||
                     (ui->rbtSmallest->isChecked() && firstPixmap.height() < secondPixmap.height()))
                rescaledSecondPixmap = secondPixmap.scaledToHeight(firstPixmap.height());

            int collageWidth = rescaledFirstPixmap.width() + rescaledSecondPixmap.width();
            int collageHeight = rescaledFirstPixmap.height();
            QPixmap collage(collageWidth, collageHeight);
            QPainter painter(&collage);
            painter.drawPixmap(QRectF(0, 0, rescaledFirstPixmap.width(), rescaledFirstPixmap.height()),
                               rescaledFirstPixmap,
                               QRectF(0, 0, rescaledFirstPixmap.width(), rescaledFirstPixmap.height()));
            painter.drawPixmap(QRectF(rescaledFirstPixmap.width(), 0, rescaledSecondPixmap.width(), rescaledSecondPixmap.height()),
                                rescaledSecondPixmap,
                                QRectF(0, 0, rescaledSecondPixmap.width(), rescaledSecondPixmap.height()));
            painter.end();
            resultPixmap = collage;
        }
        else
        {
            if ((ui->rbtBiggest->isChecked() && firstPixmap.width() < secondPixmap.width()) ||
                (ui->rbtSmallest->isChecked() && firstPixmap.width() > secondPixmap.width()))
                    rescaledFirstPixmap = firstPixmap.scaledToWidth(secondPixmap.width());
            else if ((ui->rbtBiggest->isChecked() && firstPixmap.width() > secondPixmap.width()) ||
                     (ui->rbtSmallest->isChecked() && firstPixmap.width() < secondPixmap.width()))
                rescaledSecondPixmap = secondPixmap.scaledToWidth(firstPixmap.width());

            int collageWidth = rescaledFirstPixmap.width();
            int collageHeight = rescaledFirstPixmap.height() + rescaledSecondPixmap.height();
            QPixmap collage(collageWidth, collageHeight);
            QPainter painter(&collage);
            painter.drawPixmap(QRectF(0, 0, rescaledFirstPixmap.width(), rescaledFirstPixmap.height()),
                               rescaledFirstPixmap,
                               QRectF(0, 0, rescaledFirstPixmap.width(), rescaledFirstPixmap.height()));
            painter.drawPixmap(QRectF(0, rescaledFirstPixmap.height(), rescaledSecondPixmap.width(), rescaledSecondPixmap.height()),
                                rescaledSecondPixmap,
                                QRectF(0, 0, rescaledSecondPixmap.width(), rescaledSecondPixmap.height()));
            painter.end();
            resultPixmap = collage;
        }
        return resultPixmap;
    }
    else
    {
        return QPixmap();
    }
}

void MainWindow::showWarning()
{
    QMessageBox message;
    message.setIcon(QMessageBox::Warning);
    message.setWindowTitle("Warning");
    message.setWindowIcon(QIcon("icon.png"));
    message.setText("Add pictures at first.");
    message.exec();
}
