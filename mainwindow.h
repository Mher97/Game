#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <algorithm>
#include "const.h"
#include "card.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        qreal m_width;
        qreal m_height;
        QGraphicsView *m_view;
        QGraphicsScene *m_scene;
        vector<QRect> m_buttons;
        vector<QTimer*> m_timers;
        QPushButton *m_pause;
        QPushButton *m_back;
        QLabel *m_timer_label;
        QLabel *m_message_label;
        vector<card*> m_cards;
        vector<char> m_symbols;
        int m_level;
        int m_game_state;
        int m_size;
        int m_current_card;
        int m_prev_card;
        int m_removed_items;
        int m_time;
        int m_score;

        void window_maker();
        void buttons_maker();
        void scene_maker();
        void symbols_maker();
        void cards_maker();
        void play();
        void transponation();
        QString toString(int)const;
    protected:
        void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent*);
    private slots:
         void start_slot();
         void back_slot();
         void pause_slot();
         void duration_slot();
         void scene_update1();
         void play_helper();
};

#endif // MAINWINDOW_H
