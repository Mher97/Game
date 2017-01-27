#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window_maker();
    m_game_state=MENU;
    m_size=6;
    m_current_card=-1;
    m_prev_card=-1;
    m_level=0;
    m_removed_items=0;
    m_time=70;
    m_score=0;
    m_timers=vector<QTimer*>(4);
    for (int i=0;i<4;++i)
        m_timers[i]=new QTimer(this);
    scene_maker();
    connect(m_pause,SIGNAL(clicked()),this,SLOT(pause_slot()));
    connect(m_back,SIGNAL(clicked()),this,SLOT(back_slot()));

    connect(m_timers[0],SIGNAL(timeout()),this,SLOT(scene_update1()));
    connect(m_timers[1],SIGNAL(timeout()),this,SLOT(play_helper()));
    connect(m_timers[2],SIGNAL(timeout()),this,SLOT(start_slot()));
    connect(m_timers[3],SIGNAL(timeout()),this,SLOT(duration_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::window_maker()
{
    m_width=QApplication::desktop()->geometry().width();
    m_height=QApplication::desktop()->geometry().height();
    this->setGeometry(0,0,m_width,m_height);
    buttons_maker();
}

void MainWindow::buttons_maker()
{
    m_buttons=vector<QRect>(2);
    for (size_t i=0;i<2;++i)
        m_buttons[i]=QRect(for_buttons[2]*m_width/100,for_buttons[i]*m_height/100,
                for_buttons[3]*m_width/100,for_buttons[4]*m_height/100);
    m_pause=new QPushButton("PAUSE");
    m_pause->setGeometry(m_width*for_buttons[7]/100,m_height*for_buttons[9]/100,
                        m_width*for_buttons[8]/100,m_height*for_buttons[8]/100);
    m_pause->setStyleSheet("QPushButton { background-color : transparent; color : black; }");
    m_back=new QPushButton("BACK");
    m_back->setGeometry(m_width*for_buttons[7]/100,m_height*for_buttons[10]/100,
                        m_width*for_buttons[8]/100,m_height*for_buttons[8]/100);
    m_back->setStyleSheet("QPushButton { background-color : transparent; color : black; }");
    m_timer_label=new QLabel();
    m_timer_label->setGeometry(m_width*for_buttons[7]/100,m_height*for_buttons[11]/100,
                        m_width*for_buttons[8]/100,m_height*for_buttons[8]/100);
    m_timer_label->setStyleSheet("QLabel { background-color : transparent; color : black; }");
    m_message_label=new QLabel();
    m_message_label->setGeometry(0,0,m_width*for_buttons[12]/100,m_height*for_buttons[12]/100);
    m_message_label->setStyleSheet("QLabel { background-color : transparent; color : black; }");
}

void MainWindow::scene_maker()
{
    m_view=new QGraphicsView(this);
    m_view->hide();
    m_view->setGeometry(0,0,m_width,m_height);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scene=new QGraphicsScene(this);
    m_scene->setSceneRect(0,0,m_width,m_height);
    m_scene->setBackgroundBrush(QBrush("#61CEB6",Qt::DiagCrossPattern));
    m_scene->addWidget(m_pause);
    m_scene->addWidget(m_back);
    m_scene->addWidget(m_timer_label);
    m_scene->addWidget(m_message_label);
    m_message_label->hide();
    m_view->setScene(m_scene);
}

void MainWindow::symbols_maker()
{
    int i, j, k=0;
    vector<char> helper;
    random_device rd;
    mt19937 g(rd());
    for (char i='A';i<='Z';++i)
    {
        helper.push_back(i);
        helper.push_back(i-'A'+'a');
    }
    shuffle(helper.begin(), helper.end(), g);
    if (m_size==6)
    {
        for (i=1;i<=5;++i)
        {
            for (j=1;j<=4;++j)
                m_symbols.push_back(helper[k]);
            ++k;
        }
        for (i=1;i<=7;++i)
        {
            for (j=1;j<=2;++j)
                m_symbols.push_back(helper[k]);
            ++k;
        }
        for (i=1;i<=2;++i)
            m_symbols.push_back('?');
    }
    else if (m_size == 8)
    {
        for (i=1;i<=7;++i)
        {
            for (j=1;j<=4;++j)
                m_symbols.push_back(helper[k]);
            ++k;
        }
        for (i=1;i<=16;++i)
        {
            for (j=1;j<=2;++j)
                m_symbols.push_back(helper[k]);
            ++k;
        }
        for (i=1;i<=2;++i)
        {
            m_symbols.push_back('?');
            m_symbols.push_back('+');
        }
    }
    shuffle(m_symbols.begin(),m_symbols.end(),g);
}

void MainWindow::cards_maker()
{
    m_cards=vector<card*>(m_size*m_size);
    qreal temp, temp2;
    symbols_maker();
    if (m_size==6)
        temp2 = m_height*for_cards_y[6]/100.0;
    else temp2=m_height*for_cards_y2[8]/100.0;
    for (int i=0;i<m_size*m_size;++i)
    {
        if (m_size == 6)
        {
            if (i % 6 == 0)
                temp=(m_width-m_height*0.84)/2.0;
            m_cards[i]=new card(PRESENT, CLOSED,m_symbols[i],QPointF(temp,m_height*for_cards_y[i/6]/100.0),temp2);
            temp+=(temp2);
        }
        else
        {
            if (i % 8 == 0)
                temp=(m_width-m_height*0.88)/2.0;
            m_cards[i]=new card(PRESENT, CLOSED, m_symbols[i],QPointF(temp,m_height*for_cards_y2[i/8]/100.0),temp2);
            temp+=(temp2);
        }
        m_scene->addItem(m_cards[i]);
    }
    m_symbols.clear();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (m_game_state==MENU)
    {
        vector<QString> v={"PLAY","EXIT"};
        QPainter painter(this);
        QRectF R(0,0,m_width,m_height);
        QFont f("Sans",for_buttons[6]*m_height/100,for_buttons[7]*m_height/100);
        painter.fillRect(R,QBrush("#61CEB6",Qt::DiagCrossPattern));
        painter.setBrush(QBrush(Qt::green));
        painter.setFont(f);
        for (size_t i=0;i<2;++i)
        {
            painter.drawRect(m_buttons[i]);
            painter.drawText(m_buttons[i],v[i]);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        int x=e->x(),y=e->y();
        if (m_game_state==MENU)
        {
            if (x>m_buttons[0].topLeft().x() && x<m_buttons[0].topRight().x())
            {
                if (y>m_buttons[0].topLeft().y() && y<m_buttons[0].bottomLeft().y())
                    m_timers[2]->start(1000);
            else if (y>m_buttons[1].topLeft().y() && y<m_buttons[1].bottomLeft().y())
                    close();
            }
            update();
        }
        else if (m_game_state==PLAY && !m_timers[0]->isActive() && !m_timers[1]->isActive())
        {
            int t=-1;
            if (m_size==6)
            {
                if (x>=(m_width-m_height*0.84)/2.0 && x<m_width-((m_width-m_height*0.84)/2.0))
                {
                    if (y>=(m_height*for_cards_y[0]/100.0) && y<m_height-((m_height*for_cards_y[0]/100.0)))
                    {
                        x-=(m_width-m_height*0.84)/2.0;
                        y-=(m_height*for_cards_y[0])/100.0;
                        x/=(for_cards_y[6]*m_height)/100.0;
                        y/=(for_cards_y[6]*m_height)/100.0;
                        if (x>=0 && x<m_size && y>=0 && y<m_size)
                            t=y*m_size+x;
                        if (m_cards[t]->get_presence()==PRESENT)
                        {
                            m_prev_card=m_current_card;
                            m_current_card=t;
                            play();
                        }
                    }
                }
            }
            if (m_size==8)
            {
                if (x>=(m_width-m_height*0.88)/2.0 && x<m_width-((m_width-m_height*0.88)/2.0))
                {
                    if (y>=(m_height*for_cards_y2[0]/100.0) && y<m_height-((m_height*for_cards_y2[0]/100.0)))
                    {
                        x-=(m_width-m_height*0.88)/2.0;
                        y-=(m_height*for_cards_y2[0])/100.0;
                        x/=(for_cards_y2[8]*m_height)/100.0;
                        y/=(for_cards_y2[8]*m_height)/100.0;
                        if (x>=0 && x<m_size && y>=0 && y<m_size)
                            t=y*m_size+x;
                        if (m_cards[t]->get_presence()==PRESENT)
                        {
                            m_prev_card=m_current_card;
                            m_current_card=t;
                            play();
                        }
                     }
                }
            }
        }
    }
}

void MainWindow::start_slot()
{
    ++m_level;
    if (m_level==1)
    {
        m_size=6;
        m_time=70;
    }
    if (m_level==2)
    {
        m_size=8;
        m_time=135;
    }
    m_timer_label->setText(toString(m_time));
    m_removed_items=0;
    m_game_state=PLAY;
    cards_maker();
    m_view->show();
    m_timers[3]->start(1000);
    m_timers[2]->stop();
}

void MainWindow::back_slot()
{
    int i;
    for (i=0;i<m_size*m_size;++i)
    {
        if (m_cards[i]->isActive())
        m_scene->removeItem(m_cards[i]);
    }
    m_message_label->hide();
    m_pause->setText("PAUSE");
    m_pause->setEnabled(true);
    for (i=0;i<4;++i)
    {
        if (m_timers[i]->isActive())
            m_timers[i]->stop();
    }
    m_current_card=m_prev_card=-1;
    m_level=0;
    m_score=0;
    m_game_state=MENU;
    m_scene->update();
    m_view->hide();
}

void MainWindow::pause_slot()
{
    if (m_game_state==PLAY)
    {
        m_game_state=PAUSE;
        m_pause->setText("PLAY");
        m_timers[3]->stop();
    }
    else
    {
        m_game_state=PLAY;
        m_pause->setText("PAUSE");
        m_timers[3]->start(1000);
    }
}

void MainWindow::duration_slot()
{
    --m_time;
    m_timer_label->setText(toString(m_time));
    if (m_time==0)
    {
        m_game_state=GAME_OVER;
        m_message_label->setText("   YOU LOST!!!");
        m_message_label->show();
        for (int i=0;i<4;++i)
            m_timers[i]->stop();
        m_pause->setDisabled(true);

    }
}

void MainWindow::play()
{
    if (m_current_card!=-1 && m_prev_card!=-1)
    {
        if (m_current_card!=m_prev_card)
        {
            m_cards[m_current_card]->set_state(1-m_cards[m_current_card]->get_state());
            m_timers[0]->start(10);
            m_timers[1]->start(300);
        }
        else
        {
            m_cards[m_current_card]->set_state(1-m_cards[m_current_card]->get_state());
            m_timers[0]->start(10);
            m_prev_card=m_current_card=-1;
            m_scene->update();
        }
    }
    else if (m_prev_card==-1)
    {
        m_cards[m_current_card]->set_state(1-m_cards[m_current_card]->get_state());
        m_prev_card=m_current_card;
        m_timers[0]->start(10);
    }
}

void MainWindow::scene_update1()
{
    m_scene->update();
    m_timers[0]->stop();
}

void MainWindow::play_helper()
{
    if (m_cards[m_current_card]->get_ID()==m_cards[m_prev_card]->get_ID())
    {
        m_cards[m_current_card]->set_presence(ABSENT);
        m_cards[m_prev_card]->set_presence(ABSENT);
        m_removed_items+=2;
        if (m_cards[m_current_card]->get_ID()=='?')
            transponation();
        if (m_cards[m_current_card]->get_ID()=='+')
            m_time+=30;
        m_prev_card=m_current_card=-1;
        if (m_removed_items==m_size*m_size)
        {
            for (int i=0;i<m_size*m_size;++i)
                if (m_cards[i]->isActive())
                    m_scene->removeItem(m_cards[i]);
            m_score+=m_time;
            if (m_size==6)
            {
                m_timers[2]->start(1000);
                m_timers[3]->stop();
            }
            else
            {
                m_message_label->setText("   YOU WIN \n   YOUR SCORE " + QString::number(m_score));
                m_message_label->show();
                m_pause->setDisabled(true);
                for (int i=0;i<4;++i)
                {
                    if (m_timers[i]->isActive())
                        m_timers[i]->stop();
                }
            }
        }
    }
    else
    {
        m_cards[m_prev_card]->set_state(1-m_cards[m_prev_card]->get_state());
        m_cards[m_current_card]->set_state(1-m_cards[m_current_card]->get_state());
        m_prev_card=m_current_card=-1;
    }
    m_scene->update();
    m_timers[1]->stop();
}

void MainWindow::transponation()
{
    int i,j;
    vector<vector<int>> helper(m_size,vector<int>(m_size));
    for (i=0;i<m_size;++i)
    {
        for (j=0;j<m_size;++j)
            helper[i][j]=m_size*i+j;
    }
    for (int i=0;i<m_size-1;++i)
    {
        for (j=i+1;j<m_size;++j)
            m_cards[helper[i][j]]->card_swap(m_cards[helper[j][i]]);
    }
}

QString MainWindow::toString(int m)const
{
    QString answer(8,' ');
    int a=m/60,b=m%60;
    answer.push_back('0');
    answer.push_back(a+'0');
    answer+=" : ";
    if (b>=10)
    {
        QString helper;
        while(b)
        {
            helper.push_back(b%10+'0');
            b/=10;
        }
        reverse(helper.begin(),helper.end());
        answer+=helper;
    }
    else
    {
        answer.push_back("0");
        answer.push_back(b+'0');
    }
    return answer;
}

