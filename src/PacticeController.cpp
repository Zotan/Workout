/*
 * PacticeController.cpp
 *
 *  Created on: 21 avr. 2015
 *      Author: pierre
 */

#include "PracticeController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/system/SystemDialog>

#include <QDateTime>
#include <QTimer>

#include "DataObject.hpp"
#include "Database.hpp"

PracticeController::PracticeController(QObject *parent) : QObject(parent), m_ListView(NULL), m_HistoryWeb(NULL), m_Weight(0), m_Repetition(0), m_SetsNumber(1), m_TimeStopWatchSec(60),m_CacheId(0), m_CacheCategory(0), m_CacheExerciseId(0) {

    updateDateTime();

    QSettings settings("Amonchakai", "Workout");
    m_TimeStopWatchSec = settings.value("StopWatch", 60).toInt();
    m_StopWatch = QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec % 60)), 2, 10, QChar('0'));

    notif = NULL;
    m_Led = new bb::device::Led();
    m_Led->setColor(bb::device::LedColor::Magenta);
}

void PracticeController::saveSet(const QString &note) {
    Set *set = new Set();

    set->setRepId(m_SetsNumber);
    set->setNote(note);
    set->setRepetition(m_Repetition);
    set->setWeight(m_Weight);
    set->setTime(QDateTime::currentMSecsSinceEpoch());

    m_Sets.push_back(set);

    setSets(m_SetsNumber+1);

    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
     if (!dataModel) {
         qDebug() << "create new model";
         dataModel = new GroupDataModel(
                 QStringList() << "id"
                               << "title"
                               << "category"
                 );
         m_ListView->setDataModel(dataModel);
     }

     dataModel->insert(set);

     startStopWatch();
}


void PracticeController::updateDateTime() {

    m_Time = QDateTime::currentDateTime().time().toString("hh:mm");
    m_Date = QDateTime::currentDateTime().date().toString(Qt::SystemLocaleShortDate);

    emit dateChanged();
    emit timeChanged();

    QTimer::singleShot(1000*60, this, SLOT(updateDateTime()));
}


void PracticeController::pushToDB(int exerciseId) {

    for(int i = 0 ; i < m_Sets.size() ; ++i) {
        Database::get()->saveSet(exerciseId, m_Sets.at(i));
    }


    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
     if(dataModel != NULL)
         dataModel->clear();
     m_Sets.clear();

}


void PracticeController::pushCardioToDB(int exerciseId, const QString &notes) {
    Database::get()->saveCardio(exerciseId, m_Duration, QDateTime::currentMSecsSinceEpoch(), m_Distance, m_HeartRate, m_Calories, notes);

}


void PracticeController::loadPractice(int exercise_id, int category) {
    if(category == 1) {
        Cardio *c = Database::get()->getCardio(exercise_id);
        if(c == NULL) return;

        this->setDuration(c->getDuration());
        this->setDistance(c->getDistance());
        this->setHeartRate(c->getHeartRate());
        this->setCalories(c->getCalories());
        this->setNotes(c->getNote());

        m_CacheExerciseId = c->getId();

        c->deleteLater();

    } else {

        Set *s = Database::get()->getSet(exercise_id);
        if(s == NULL) return;

        this->setRepetition(s->getRepetition());
        this->setWeight(s->getWeight());
        this->setNotes(s->getNote());

        m_CacheExerciseId = s->getId();

        s->deleteLater();
    }
}

void PracticeController::updatePractice(int category) {
    if(category == 1) {
        Cardio c;

        c.setId(m_CacheExerciseId);
        c.setDistance(this->m_Distance);
        c.setDuration(this->m_Duration);
        c.setHeartRate(this->m_HeartRate);
        c.setCalories(this->m_Calories);
        c.setNote(this->m_Notes);

        Database::get()->updatePractice(&c);

    } else {

        Set s;

        s.setRepetition(this->m_Repetition);
        s.setWeight(this->m_Weight);
        s.setNote(this->m_Notes);
        s.setId(m_CacheExerciseId);

        Database::get()->updatePractice(&s);
    }
}

void PracticeController::loadStrengthHistory(int exercise_id) {

    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_HistoryListView == NULL) {
         qWarning() << "did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_HistoryListView->dataModel());
     if (!dataModel) {
         qDebug() << "create new model";
         dataModel = new GroupDataModel(
                 QStringList() << "id"
                               << "title"
                               << "category"
                 );
         m_HistoryListView->setDataModel(dataModel);
     }
     dataModel->clear();

     QList<Set*> sets = Database::get()->getHistoryStrength(exercise_id);

     QList<QObject *> datas;
     for(int i = sets.length()-1 ; i >= 0 ; --i) {
         sets[i]->setTime(floor(sets[i]->getTime()/(1000*60*60*4))*1000*60*60*4);
         datas.push_back(sets.at(i));
     }
     dataModel->insertList(datas);
}

void PracticeController::restoreSession(int exercise_id) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (!dataModel) {
        return;
    }
    dataModel->clear();


    QList<Set*> sets = Database::get()->getHistoryStrength(exercise_id, QDateTime::currentDateTime().toMSecsSinceEpoch()-(1000*60*60*2));
    QList<QObject*> ex;
    for(int i = sets.length()-1 ; i >= 0 ; --i) {
        ex.push_back(sets.at(i));
    }

    dataModel->insertList(ex);
    if(!sets.isEmpty())
        setWeight(sets.first()->getWeight());
    else
        setWeight(0);

    setSets(sets.size()+1);

}

void PracticeController::loadHistory(int exercise_id) {


    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_HistoryListView == NULL) {
         qWarning() << "did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_HistoryListView->dataModel());
     if (!dataModel) {
         qDebug() << "create new model";
         dataModel = new GroupDataModel(
                 QStringList() << "id"
                               << "title"
                               << "category"
                 );
         m_HistoryListView->setDataModel(dataModel);
     }
     dataModel->clear();

     QList<Cardio*> cardios = Database::get()->getHistoryCardio(exercise_id);

     QList<QObject *> datas;
     for(int i = 0 ; i < cardios.size() ; ++i) {
         cardios[i]->setDuration(cardios.at(i)->getDuration()/60);
         datas.push_back(cardios.at(i));
     }
     dataModel->insertList(datas);
}


QString PracticeController::getDateFromTime(qint64 time) {
    QString str;

    str = QDateTime::fromMSecsSinceEpoch(time).date().toString(Qt::SystemLocaleShortDate);

    return str;
}

void PracticeController::plotCardio(int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria) {

    if(m_HistoryWeb == NULL) return;

    QList<Cardio*> cardios = Database::get()->getHistoryCardio(exercise_id, begin.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());

    if(cardios.empty()) return;

    QString datas = "var data = { "
                        "labels: [";

    for(int i = cardios.length()-1 ; i >= 0 ; --i) {
        datas += "\"" + QDateTime::fromMSecsSinceEpoch(cardios.at(i)->getTime()).toString(Qt::SystemLocaleShortDate) + "\"";

        if(i > 0)
            datas += ", ";
    }

    datas += "], "
            "datasets: ["
                "{"
                    "label: \"My First dataset\","
                    "fillColor: \"rgba(220,220,220,0.2)\","
                    "strokeColor: \"rgba(220,220,220,1)\","
                    "pointColor: \"rgba(220,220,220,1)\","
                    "pointStrokeColor: \"#fff\","
                    "pointHighlightFill: \"#fff\","
                    "pointHighlightStroke: \"rgba(220,220,220,1)\","
                    "data: [" ;


    for(int i = cardios.length()-1 ; i >= 0 ; --i) {

        switch(criteria) {
            case 0:
                datas += QString::number(static_cast<float>(cardios.at(i)->getDuration())/60);
                break;

            case 1:
                datas += QString::number(static_cast<int>(cardios.at(i)->getDistance()));
                break;

            case 2:
                datas += QString::number(static_cast<int>(cardios.at(i)->getHeartRate()));
                break;

            case 3:
                datas += QString::number(static_cast<int>(cardios.at(i)->getCalories()));
                break;
        }

        if(i > 0)
            datas += ", ";
    }

    datas += "]}]";

    datas += "};";
    datas += "new Chart(ctx).Line(data, {"
                "bezierCurve: false"
            "});";


    m_HistoryWeb->evaluateJavaScript(datas);

}


void PracticeController::plotStrength(int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria) {

    if(m_HistoryWeb == NULL) return;

    QList<Set*> sets = Database::get()->getHistoryStrength(exercise_id, begin.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());

    if(sets.empty()) return;

    QString datas = "var data = { "
                        "labels: [";

    bool added = false;
    if(sets.length() > 0) {
        datas += "\"" + QDateTime::fromMSecsSinceEpoch(sets.last()->getTime()).toString(Qt::SystemLocaleShortDate) + "\", ";
        added = true;
    }

    for(int i = sets.length()-2 ; i >= 0 ; --i) {
        if((sets.at(i)->getTime() - sets.at(i+1)->getTime()) > 1000*60*60*4) {
            datas += "\"" + QDateTime::fromMSecsSinceEpoch(sets.at(i)->getTime()).toString(Qt::SystemLocaleShortDate) + "\"";
            added = true;
        }

        if(added) {
            datas += ", ";
            added = false;
        }
    }
    datas[datas.length()-2] = ' '; // remove the last comma...


    datas += "], "
            "datasets: ["
                "{"
                    "label: \"My First dataset\","
                    "fillColor: \"rgba(220,220,220,0.2)\","
                    "strokeColor: \"rgba(220,220,220,1)\","
                    "pointColor: \"rgba(220,220,220,1)\","
                    "pointStrokeColor: \"#fff\","
                    "pointHighlightFill: \"#fff\","
                    "pointHighlightStroke: \"rgba(220,220,220,1)\","
                    "data: [" ;



    QList<double> feature;
    if(sets.length() > 0) {
        if(criteria < 4)
            feature.push_back(sets.last()->getWeight());
        else
            feature.push_back(sets.last()->getRepetition());
    }

    for(int i = sets.length()-2 ; i >= 0 ; --i) {
        if((sets.at(i)->getTime() - sets.at(i+1)->getTime()) > 1000*60*60*4) {
            switch(criteria % 4) {
                case 0: {
                    double mx = 0;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        mx = mx >  feature.at(n) ? mx : feature.at(n);
                    }
                    datas += "\"" + QString::number(mx) + "\", ";
                    break;
                }

                case 1: {
                    double mn = 10000000000.;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        mn = mn <  feature.at(n) ? mn : feature.at(n);
                    }
                    datas += "\"" + QString::number(mn) + "\", ";
                    break;
                }

                case 2: {
                    double avg = 0.;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        avg += feature.at(n);
                    }
                    datas += "\"" + QString::number(avg / feature.size()) + "\", ";
                    break;
                }

                case 3: {
                    double avg = 0.;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        avg += feature.at(n);
                    }
                    datas += "\"" + QString::number(avg) + "\", ";
                    break;
                }
            }

            feature.clear();
            if(criteria < 4)
                feature.push_back(sets.at(i)->getWeight());
            else
                feature.push_back(sets.at(i)->getRepetition());

        } else {
            if(criteria < 4)
                feature.push_back(sets.at(i)->getWeight());
            else
                feature.push_back(sets.at(i)->getRepetition());
        }
    }
    switch(criteria % 4) {
        case 0: {
            double mx = 0;
            for(int n = 0 ; n < feature.size() ; ++n) {
                mx = mx >  feature.at(n) ? mx : feature.at(n);
            }
            datas += "\"" + QString::number(mx) + "\" ";
            break;
        }

        case 1: {
            double mn = 10000000000.;
            for(int n = 0 ; n < feature.size() ; ++n) {
                mn = mn <  feature.at(n) ? mn : feature.at(n);
            }
            datas += "\"" + QString::number(mn) + "\" ";
            break;
        }

        case 2: {
            double avg = 0.;
            for(int n = 0 ; n < feature.size() ; ++n) {
                avg += feature.at(n);
            }
            datas += "\"" + QString::number(avg / feature.size()) + "\" ";
            break;
        }

        case 3: {
            double avg = 0.;
            for(int n = 0 ; n < feature.size() ; ++n) {
                avg += feature.at(n);
            }
            datas += "\"" + QString::number(avg) + "\" ";
            break;
        }
    }

    datas += "]}]";

    datas += "};";
    datas += "new Chart(ctx).Line(data, {"
                "bezierCurve: false"
            "});";

    qDebug() << datas;

    m_HistoryWeb->evaluateJavaScript(datas);

}




void PracticeController::deletePracticeEntry(int id, int category, int exercise_id) {
    m_CacheId = id;
    m_CacheCategory = category;
    m_CacheExerciseId = exercise_id;

    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Delete practice record"));
    dialog->setBody(tr("Are you sure you want to delete this record?"));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type)));

    if (success) {
        dialog->show();
    } else {
        dialog->deleteLater();
    }
}

void PracticeController::deletePracticeEntryNoAsk(int id, int category) {
    if(category == 1) {
        Database::get()->deletePracticeCardioEntry(id);
    } else {
        Database::get()->deletePracticeStrengthEntry(id);
    }
}

void PracticeController::onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type result) {

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {

        if(m_CacheCategory == 1) {
            Database::get()->deletePracticeCardioEntry(m_CacheId);
            loadHistory(m_CacheExerciseId);
        } else {
            Database::get()->deletePracticeStrengthEntry(m_CacheId);
            loadStrengthHistory(m_CacheExerciseId);
        }

    }
}

// -------------------------------------------------------------------
// Stop watch!

#define max(a, b) (a) > (b) ? (a) : (b)

void PracticeController::startStopWatch() {
    m_StopWatchMutex.lockForWrite();
    m_TargetTimeStopWatch = QDateTime::currentMSecsSinceEpoch() + m_TimeStopWatchSec * 1000;

    if(!m_StopWatchRunning) {
        m_StopWatchRunning = true;
        QTimer::singleShot(1000, this, SLOT(stopWatchTick()));
    }
    m_StopWatchMutex.unlock();
}

void PracticeController::resetStopWatch() {
    m_StopWatchMutex.lockForWrite();
    m_StopWatchRunning = false;
    QSettings settings("Amonchakai", "Workout");
    m_TimeStopWatchSec = settings.value("StopWatch", 60).toInt();

    m_StopWatch = QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec % 60)), 2, 10, QChar('0'));
    emit stopWatchChanged();
    m_StopWatchMutex.unlock();

}

void PracticeController::addSecStopWatch(int sec) {
    m_StopWatchMutex.lockForWrite();
    m_TimeStopWatchSec = max(m_TimeStopWatchSec+sec, 0);
    m_TargetTimeStopWatch += sec*1000;

    if(!m_StopWatchRunning) {
        QSettings settings("Amonchakai", "Workout");
        settings.setValue("StopWatch", m_TimeStopWatchSec);
    }

    if(m_StopWatchRunning) {
        qint64 delta = max((m_TargetTimeStopWatch - QDateTime::currentMSecsSinceEpoch()) / 1000, 0);
        m_StopWatch = QString("%1").arg(static_cast<int>(floor(delta/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(delta % 60)), 2, 10, QChar('0'));
    } else {
        m_StopWatch = QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec % 60)), 2, 10, QChar('0'));
    }
    emit stopWatchChanged();

    m_StopWatchMutex.unlock();
}

void PracticeController::stopWatchTick() {
    m_StopWatchMutex.lockForWrite();
    if(!m_StopWatchRunning) {
        m_StopWatchMutex.unlock();
        return;
    }

    qint64 delta = max((m_TargetTimeStopWatch - QDateTime::currentMSecsSinceEpoch()) / 1000, 0);
    if(delta == 0) {
        QSettings settings("Amonchakai", "Workout");
        delta = settings.value("StopWatch", 60).toInt();

        if(settings.value("ledNotif", 1) == 1)
            m_Led->flash(2);

        if(settings.value("soundNotif", 0) == 1) {
            if(notif == NULL) {
                notif = new bb::platform::Notification();
            }
            notif->notify();
            notif->clearEffectsForAll();
        }

        m_StopWatchRunning = false;
    }

    m_StopWatch = QString("%1").arg(static_cast<int>(floor(delta/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(delta % 60)), 2, 10, QChar('0'));
    emit stopWatchChanged();

    if(m_StopWatchRunning)
        QTimer::singleShot(1000, this, SLOT(stopWatchTick()));

    m_StopWatchMutex.unlock();
}


void PracticeController::setSecStopWatch(const QDateTime &time) {
    m_StopWatchMutex.lockForWrite();
    m_TimeStopWatchSec = time.time().hour()*3600+time.time().minute()*60+time.time().second();

    if(!m_StopWatchRunning) {
        QSettings settings("Amonchakai", "Workout");
        settings.setValue("StopWatch", m_TimeStopWatchSec);
    }

    if(m_StopWatchRunning) {
        qint64 delta = max((m_TargetTimeStopWatch - QDateTime::currentMSecsSinceEpoch()) / 1000, 0);
        m_StopWatch = QString("%1").arg(static_cast<int>(floor(delta/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(delta % 60)), 2, 10, QChar('0'));
    } else {
        m_StopWatch = QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec % 60)), 2, 10, QChar('0'));
    }
    emit stopWatchChanged();

    m_StopWatchMutex.unlock();
}

QString PracticeController::getSecStopWatch() {
    return QString("%1").arg(static_cast<int>(floor(m_TimeStopWatchSec/3600)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(floor((m_TimeStopWatchSec - floor(m_TimeStopWatchSec/3600)*3600)/60)), 2, 10, QChar('0')) + ":" + QString("%1").arg(static_cast<int>(m_TimeStopWatchSec%60), 2, 10, QChar('0'));
}



