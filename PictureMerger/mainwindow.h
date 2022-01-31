#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btMerge_clicked();
    void on_btPreview_clicked();

private:
    Ui::MainWindow *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    QPixmap getResultPixmap();
    void showWarning();
    QPixmap firstPixmap;
    QPixmap secondPixmap;
};
#endif // MAINWINDOW_H
