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
#include "Graph.hpp"


PracticeController::PracticeController(QObject *parent) : QObject(parent), m_ListView(NULL), m_Weight(0), m_Repetition(0), m_SetsNumber(1), m_TimeStopWatchSec(60),m_CacheId(0), m_CacheCategory(0), m_CacheExerciseId(0), m_CacheIsSaved(0) {

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

    set->setRepId((m_SetsNumber+1));
    set->setNote(note);
    set->setRepetition(m_Repetition);
    set->setWeight(m_Weight);
    set->setTime(QDateTime::currentMSecsSinceEpoch());
    set->setDone(true);
    set->setSaved(false);
    set->setId(m_Sets.size());

    m_Sets.push_back(set);

    setSets(m_SetsNumber+1);

    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "(saveSet) did not received the listview. quit.";
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


     if((m_SetsNumber-1) < m_LastSets.size()) {
         Set *eSet = m_LastSets[m_SetsNumber-1];

         eSet->setDone(true);
         eSet->setSaved(false);
         eSet->setNote(note);
         eSet->setRepId(m_SetsNumber);
         eSet->setRepetition(m_Repetition);
         eSet->setWeight(m_Weight);
         eSet->setTime(set->getTime());
         eSet->setId(m_Sets.length()-1);

     } else {
         dataModel->insert(set);
     }

     if(m_SetsNumber < m_LastSets.size()) {
         setRepetition(m_LastSets[m_SetsNumber]->getRepetition());
         setWeight(m_LastSets[m_SetsNumber]->getWeight());
     }


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
        if(!m_Sets.at(i)->isSaved())
            Database::get()->saveSet(exerciseId, m_Sets.at(i));
    }


    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "(pushToDB) did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
     if(dataModel != NULL)
         dataModel->clear();
     m_Sets.clear();
     m_LastSets.clear();

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

void PracticeController::resetStrengthView() {
    setWeight(0);
    setRepetition(0);
    setSets(0);

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "(resetStrengthView) did not received the listview. quit.";
         return;
     }

     GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
     if(dataModel != NULL)
         dataModel->clear();
     m_Sets.clear();

}

void PracticeController::updatePractice(int id, int isSaved, int category) {
    if(category == 1) {
        Cardio c;

        c.setId(id);
        c.setDistance(this->m_Distance);
        c.setDuration(this->m_Duration);
        c.setHeartRate(this->m_HeartRate);
        c.setCalories(this->m_Calories);
        c.setNote(this->m_Notes);

        Database::get()->updatePractice(&c);

    } else {

        if(isSaved == 1) {

            for(int i = 0 ; i < m_Sets.size() ; ++i) {

                if(m_Sets.at(i)->isSaved() && m_Sets.at(i)->getId() == id) {

                    m_Sets.at(i)->setRepetition(this->m_Repetition);
                    m_Sets.at(i)->setWeight(this->m_Weight);
                    m_Sets.at(i)->setNote(this->m_Notes);

                    Database::get()->updatePractice(m_Sets.at(i));

                    return;
                }
            }

        } else {

            if(id < m_Sets.size()) {
                m_Sets.at(id)->setRepetition(this->m_Repetition);
                m_Sets.at(id)->setWeight(this->m_Weight);
                m_Sets.at(id)->setNote(this->m_Notes);
            }
        }
    }
}


void PracticeController::restoreSession(int exercise_id) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "(restoreSession) did not received the listview. quit.";
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
        m_Sets.push_back(sets.at(i));
    }


    dataModel->insertList(ex);
    if(!sets.isEmpty()) {
        setWeight(sets.first()->getWeight());
        setRepetition(sets.first()->getRepetition());
    } else {
        setWeight(0);
        setRepetition(0);
    }

    setSets(sets.size());

}

void PracticeController::loadPrevious(int exercise_id) {
    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "(loadPrevious) did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (!dataModel) {
        return;
    }

    int numberSetDone = m_Sets.size();

    QList<Set*> sets = Database::get()->getLastHistoryStrength(exercise_id);
    QList<QObject*> ex;


    for(int i = m_LastSets.size() ; i < sets.size() ; ++i) {
        m_LastSets.push_back(sets.at(i));
    }

    for(int i = 0 ; i < sets.size() - dataModel->size() ; ++i) {
        ex.push_back(sets.at(dataModel->size() + i));
    }

    dataModel->insertList(ex);
    if(!sets.isEmpty()) {
        int idx = std::min(numberSetDone, sets.size()-1);
        setWeight(sets[idx]->getWeight());
        setRepetition(sets[idx]->getRepetition());
    }

    setSets(numberSetDone);
}


QString PracticeController::getDateFromTime(qint64 time) {
    QString str;

    str = QDateTime::fromMSecsSinceEpoch(time).date().toString(Qt::SystemLocaleShortDate);

    return str;
}



void PracticeController::deletePracticeEntry(int id, int isSaved, int category, int exercise_id) {
    m_CacheId = id;
    m_CacheCategory = category;
    m_CacheExerciseId = exercise_id;
    m_CacheIsSaved = isSaved;

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


void PracticeController::onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type result) {

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        deletePractice(m_CacheId, m_CacheIsSaved, m_CacheCategory);
    }
}

void PracticeController::deletePractice(int id, int isSaved, int category) {
    if(isSaved) {
        if(category == 1) {
            Database::get()->deletePracticeCardioEntry(id);
        } else {
            Database::get()->deletePracticeStrengthEntry(id);

            for(int i = 0 ; i < m_Sets.size() ; ++i) {
                if(m_Sets.at(i)->getId() == id) {
                    removeItemFromListView(i);
                    updateItemFromListViewAfterDelete(i);
                    return;
                }
            }
        }
    } else {
        removeItemFromListView(id);
    }
}

void PracticeController::removeItemFromListView(int position) {

    // remove from view
    if(m_ListView == NULL) {
         qWarning() << "(removeItemFromListView) did not received the listview. quit.";
         return;
     }

    using namespace bb::cascades;
    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(dataModel == NULL) {
        qWarning() << "(removeItemFromListView) No pre-defined datamodel. quit.";
        return;
    }

    if(position >=  m_Sets.size()) {
        qWarning() << "Deleting item which does not exists. quit.";
        return;
    }

    // remove it from the array of indication set
    if(position < m_LastSets.size()) {
        m_LastSets.removeAt(position);

        for(int i = position ; i < m_LastSets.size() ; ++i) {
            m_LastSets[i]->setRepId(i+1);
            m_LastSets[i]->setId(i);
        }
    }

    // remove the item from the array & update the ids
    m_Sets.removeAt(position);
    for(int i = position ; i < m_Sets.size() ; ++i) {
        m_Sets[i]->setRepId(i+1);
        if(!m_Sets[i]->isSaved())
            m_Sets[i]->setId(i);
    }



     QVariantList path;
     path << dataModel->size() - 1 - position;
     dataModel->removeAt(path);

     setSets(m_Sets.size());

}

void PracticeController::updateItemFromListViewAfterDelete(int position) {
    for(int i = position ; i < m_Sets.size() ; ++i) {
        if(m_Sets.at(i)->isSaved()) {
            Database::get()->updatePractice(m_Sets.at(i));
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
            QTimer::singleShot(1000, notif, SLOT(clearEffectsForAll()));
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



