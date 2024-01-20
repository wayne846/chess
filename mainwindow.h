#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        Ui::MainWindow* getui();
        ~MainWindow();

    private slots:
        void on_actionrestart_triggered();

        void on_actionchangeSize50_triggered();

        void on_actionchangeSize75_triggered();

        void on_actionchangeSize100_triggered();

        void on_actionchangeSize125_triggered();

        void on_actionchangeSize150_triggered();

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
