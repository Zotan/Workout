/*
 * ProgressController.cpp
 *
 *  Created on: 9 sept. 2016
 *      Author: pierre
 */


#include "ProgressController.hpp"
#include "Database.hpp"
#include "DataObject.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/system/SystemDialog>
#include <limits>

ProgressController::ProgressController(QObject *parent) : QObject(parent),  m_ListView(NULL), m_CacheId(0), m_CacheCategory(0), m_CacheExerciseId(0) {
    m_Criteria = 0;
    m_StartGraph = initDate();
    m_EndGraph = initDateNow();

}


void ProgressController::getWeightsList() {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    } else {
        qWarning() << "no group data model!";
        return;
    }


    QList<QObject*> datas;
    QSettings settings("Amonchakai", "Workout");
    bool metric = settings.value("unit", 2).toInt() == 2;

    QList< BodyWeight* > bodyweights = Database::get()->getBodyWeights();
    for(int i = 0 ; i < bodyweights.size() ; ++i) {
        Detail *d = new Detail();

        BodyWeight* bodyweight = bodyweights.at(i);

        d->setTitle(bodyweight->getTime());
        d->setId(bodyweight->getId());
        if(metric)
            d->setText( QString::number(bodyweight->getWeight()) + " Kg" );
        else
            d->setText( QString::number(bodyweight->getWeight()) + " lbs" );

        datas.push_back(d);
    }



    dataModel->insertList(datas);

    emit completed();

}


void ProgressController::plotBodyWeights(const QDateTime &begin, const QDateTime &end) {

    if(m_GraphController == NULL) return;

    QList< BodyWeight* > bodyweights = Database::get()->getBodyWeights(begin.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());

    QList<QString> labels;
    QList<float>   datas;

    for(int i = bodyweights.length()-1 ; i >= 0 ; --i) {
        labels.push_back(bodyweights.at(i)->getTime());
    }


    for(int i = bodyweights.length()-1 ; i >= 0 ; --i) {
        datas.push_back(bodyweights.at(i)->getWeight());
    }

    m_GraphController->plot(labels, datas);
}

void ProgressController::deleteRecord(int id) {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Delete record"));
    dialog->setBody(tr("Are you sure you want to delete this record? "));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type)));

    if (success) {
        m_CacheId = id;
        // Signal was successfully connected.
        // Now show the dialog box in your UI.
        dialog->show();
    } else {
        // Failed to connect to signal.
        // This is not normal in most cases and can be a critical
        // situation for your app! Make sure you know exactly why
        // this has happened. Add some code to recover from the
        // lost connection below this line.
        dialog->deleteLater();
    }
}

void ProgressController::deleteRecordNoAsk(int id) {
    Database::get()->deleteBodyWeight(id);
}

void ProgressController::onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type type) {
    if(type == bb::system::SystemUiResult::ConfirmButtonSelection) {

        Database::get()->deleteBodyWeight(m_CacheId);

        if(m_ListView == NULL) {
            qWarning() << "did not received the listview. quit.";
            return;
        }

        using namespace bb::cascades;
        GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
        QVariantList indexPath = dataModel->find(QVariantList() << m_CacheId);
        dataModel->removeAt(indexPath);

    }
}




void ProgressController::loadStrengthHistory(int exercise_id) {
    qDebug() << "loadStrengthHistory";
    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "(loadStrengthHistory) did not received the listview. quit.";
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
     dataModel->clear();

     QList<Set*> sets = Database::get()->getHistoryStrength(exercise_id);

     QSettings settings("Amonchakai", "Workout");
     bool metric = settings.value("unit", 2).toInt() == 2;

     QList<QObject*> datas;

     for(int k = 0 ; k < sets.length() ; ++k) {
         Detail *d = new Detail();
         const Set *set = sets.at(k);

         d->setTitle(QDateTime::fromMSecsSinceEpoch(set->getTime()).date().toString(Qt::SystemLocaleShortDate));
         d->setId(set->getId());
         if(metric)
             d->setText(tr("SET ") + QString::number(set->getRepId()) + ": " + QString::number(set->getRepetition()) + tr(" REPS @ ") + QString::number(set->getWeight()) + tr("Kg") );
         else
             d->setText(tr("SET ") + QString::number(set->getRepId()) + ": " + QString::number(set->getRepetition()) + tr(" REPS @ ") + QString::number(set->getWeight()) + tr("lbs") );

         datas.push_back(d);
     }

     dataModel->insertList(datas);


     plotStrength(exercise_id, m_StartGraph, m_EndGraph, m_Criteria);
}


void ProgressController::loadCardioHistory(int exercise_id) {


    // ----------------------------------------------------------------------------------------------
     // get the dataModel of the listview if not already available
     using namespace bb::cascades;


     if(m_ListView == NULL) {
         qWarning() << "(loadCardioHistory) did not received the listview. quit.";
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
     dataModel->clear();

     QSettings settings("Amonchakai", "Workout");
      bool metric = settings.value("unit", 2).toInt() == 2;

     QList<Cardio*> cardios = Database::get()->getHistoryCardio(exercise_id);

     QList<QObject *> datas;

     for(int k = 0 ; k < cardios.length() ; ++k) {
         Detail *d = new Detail();
         const Cardio *cardio = cardios.at(k);

         d->setTitle(QDateTime::fromMSecsSinceEpoch(cardio->getTime()).date().toString(Qt::SystemLocaleShortDate));
         if(metric)
             d->setText(QString::number(cardio->getDistance()) + tr("m in ") + QString::number(cardio->getDuration()/60) + tr("min"));
         else
             d->setText(QString::number(cardio->getDistance()) + tr("ft in ") + QString::number(cardio->getDuration()/60) + tr("min"));

         datas.push_back(d);
     }

     dataModel->insertList(datas);

     plotCardio(exercise_id, m_StartGraph, m_EndGraph, m_Criteria);
}



void ProgressController::plotCardio(int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria) {

    if(m_GraphController == NULL) return;

    QList<Cardio*> cardios = Database::get()->getHistoryCardio(exercise_id, begin.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());

    QList<QString> labels;
    QList<float>   datas;

    for(int i = cardios.length()-1 ; i >= 0 ; --i) {
        labels.push_back(QDateTime::fromMSecsSinceEpoch(cardios.at(i)->getTime()).toString(Qt::SystemLocaleShortDate));
    }

    for(int i = cardios.length()-1 ; i >= 0 ; --i) {

        switch(criteria) {
            case 0:
                datas.push_back((static_cast<float>(cardios.at(i)->getDuration())/60));
                break;

            case 1:
                datas.push_back((static_cast<int>(cardios.at(i)->getDistance())));
                break;

            case 2:
                datas.push_back((static_cast<int>(cardios.at(i)->getHeartRate())));
                break;

            case 3:
                datas.push_back((static_cast<int>(cardios.at(i)->getCalories())));
                break;
        }
    }

    m_GraphController->plot(labels, datas);
}


void ProgressController::plotStrength(int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria) {

    if(m_GraphController == NULL) return;

    QList<Set*> sets = Database::get()->getHistoryStrength(exercise_id, begin.toMSecsSinceEpoch(), end.toMSecsSinceEpoch());


    QList<QString> labels;
    QList<float>   datas;

    if(sets.length() > 0) {
        labels.push_back(QDateTime::fromMSecsSinceEpoch(sets.last()->getTime()).toString(Qt::SystemLocaleShortDate));
    }

    for(int i = sets.length()-2 ; i >= 0 ; --i) {
        if((sets.at(i)->getTime() - sets.at(i+1)->getTime()) > 1000*60*60*4) {
            labels.push_back(QDateTime::fromMSecsSinceEpoch(sets.at(i)->getTime()).toString(Qt::SystemLocaleShortDate));
        }
    }


    QList<double> feature;
    if(sets.length() > 0) {
        if(criteria < 4) {
            if(criteria < 2)
                feature.push_back(sets.last()->getWeight());
            else {
                feature.push_back(sets.last()->getWeight() * sets.last()->getRepetition());
            }
        } else
            feature.push_back(sets.last()->getRepetition());
    }

    qDebug() << "Plot generate features: " << criteria;

    for(int i = sets.length()-2 ; i >= 0 ; --i) {

        // if the time difference is larger than 4h, then compute the index on the features
        if((sets.at(i)->getTime() - sets.at(i+1)->getTime()) > 1000*60*60*4) {

            switch(criteria % 4) {
                case 0: {
                    double mx = 0;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        mx = std::max(mx, feature.at(n));
                    }
                    datas.push_back(mx);
                    break;
                }

                case 1: {
                    double mn = std::numeric_limits<double>::max();
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        mn = std::min(mn, feature.at(n));
                    }
                    datas.push_back(mn);
                    break;
                }

                case 2: {
                    double avg = 0.;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        avg += feature.at(n);
                    }
                    datas.push_back(avg / feature.size());
                    break;
                }

                case 3: {
                    double avg = 0.;
                    for(int n = 0 ; n < feature.size() ; ++n) {
                        avg += feature.at(n);
                    }
                    datas.push_back(avg);
                    break;
                }
            }

            feature.clear();
            if(criteria < 4) {
                if(criteria < 2)
                    feature.push_back(sets.at(i)->getWeight());
                else {
                    feature.push_back(sets.at(i)->getWeight() * sets.at(i)->getRepetition());
                }
            } else
                feature.push_back(sets.at(i)->getRepetition());

        // if the time difference between two sets is less than 4h, then build the array features
        } else {
            if(criteria < 4) {
                if(criteria < 2)
                    feature.push_back(sets.at(i)->getWeight());
                else {
                    feature.push_back(sets.at(i)->getWeight() * sets.at(i)->getRepetition());
                }
            } else
                feature.push_back(sets.at(i)->getRepetition());
        }
    }

    switch(criteria % 4) {
        case 0: {
            double mx = 0;
            for(int n = 0 ; n < feature.size() ; ++n) {
                mx = std::max(mx, feature.at(n));
            }
            datas.push_back(mx);
            break;
        }

        case 1: {
            double mn = std::numeric_limits<double>::max();
            for(int n = 0 ; n < feature.size() ; ++n) {
                mn = std::min(mn, feature.at(n));
            }
            datas.push_back(mn);
            break;
        }

        case 2: {
            double avg = 0.;
            for(int n = 0 ; n < feature.size() ; ++n) {
                avg += feature.at(n);
            }
            datas.push_back(avg / feature.size());
            break;
        }

        case 3: {
            double avg = 0.;
            for(int n = 0 ; n < feature.size() ; ++n) {
                avg += feature.at(n);
            }
            datas.push_back(avg);
            break;
        }
    }

    m_GraphController->plot(labels, datas);
}


void ProgressController::deleteEntry(int id, int category, int exercise_id) {
    m_CacheId = id;
    m_CacheCategory = category;
    m_CacheExerciseId = exercise_id;

    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Delete record"));
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


void ProgressController::onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type result) {

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        deleteRecord(m_CacheId, m_CacheCategory);

        switch(m_CacheCategory) {
            case 1:
                loadCardioHistory(m_CacheExerciseId);
                break;

            case 2:
                updateRepIds();
                loadStrengthHistory(m_CacheExerciseId);
                break;
        }
    }
}


void ProgressController::deleteRecord(int id, int category) {
    if(category == 1) {
        Database::get()->deletePracticeCardioEntry(id);
    } else {

        m_CacheTimestamp = Database::get()->getSet(id)->getTime();

        Database::get()->deletePracticeStrengthEntry(id);
    }
}


void ProgressController::updateRepIds() {
    qDebug() << "updateRepIds() ";
    QList<Set*> sets = Database::get()->getHistoryStrength(m_CacheExerciseId, m_CacheTimestamp-4*1000*60*60, m_CacheTimestamp+4*1000*60*60);

    for(int i = sets.length()-1 ; i >= 0 ; --i) {
        qDebug() << sets.at(i)->getRepId();

        sets.at(i)->setRepId(sets.length()-i);
        Database::get()->updatePractice(sets.at(i));
    }
}




