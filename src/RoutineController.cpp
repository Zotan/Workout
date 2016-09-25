/*
 * RoutineController.cpp
 *
 *  Created on: 3 mai 2015
 *      Author: pierre
 */

#include "RoutineController.hpp"
#include <bb/system/SystemPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <QRegExp>

#include "Database.hpp"
#include "DataObject.hpp"



RoutineController::RoutineController(QObject *parent) : QObject(parent), m_RoutineList(NULL), m_RoutineDetailList(NULL), m_InvokeManager(NULL), m_tmp_id(0), m_PlayListIndex(0) {

}


void RoutineController::updateRoutineList() {

    if(m_RoutineList == NULL)
        return;

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_RoutineList == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_RoutineList->dataModel());
    if (dataModel) {
        dataModel->clear();
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<Routine*> routines = Database::get()->getRoutines();

    QList<QObject*> datas;
    for(int i = 0 ; i < routines.length() ; ++i) {
        datas.push_back(routines.at(i));
    }

    dataModel->insertList(datas);
    emit completed();

}

void RoutineController::createRoutine() {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Add a new routine"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("name of the template..."));


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedAddRoutine(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }
}

void RoutineController::onPromptFinishedAddRoutine (bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {

        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            //qDebug() << "Prompt Accepted:" << prompt->inputFieldTextEntry();
            if(!prompt->inputFieldTextEntry().isEmpty())
                Database::get()->addRoutine(prompt->inputFieldTextEntry());

            prompt->deleteLater();
            updateRoutineList();
        }
    }
}



void RoutineController::deleteRoutine(int id) {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Delete routine"));
    dialog->setBody(tr("Are you sure you want to delete this routine?"));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedDeleteRoutine(bb::system::SystemUiResult::Type)));

    if (success) {
        m_tmp_id = id;
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

void RoutineController::deleteRoutinesNoAsk(int id) {
    Database::get()->deleteRoutine(id);
}

void RoutineController::onPromptFinishedDeleteRoutine(bb::system::SystemUiResult::Type type) {

    if(type == bb::system::SystemUiResult::ConfirmButtonSelection) {

        Database::get()->deleteRoutine(m_tmp_id);

        if(m_RoutineList == NULL) {
            qWarning() << "did not received the listview. quit.";
            return;
        }

        using namespace bb::cascades;
        GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_RoutineList->dataModel());
        QVariantList indexPath = dataModel->find(QVariantList() << m_tmp_id);
        dataModel->removeAt(indexPath);

    }
}

void RoutineController::renameRoutine(int id, const QString &label) {
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Rename routine"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setDefaultText(label);

    m_tmp_id = id;

    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedRenameRoutine(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }

}

void RoutineController::onPromptFinishedRenameRoutine(bb::system::SystemUiResult::Type result) {
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {


        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            Database::get()->renameRoutine(m_tmp_id, prompt->inputFieldTextEntry());

            updateRoutineList();

            prompt->deleteLater();
        }
    }
}


void RoutineController::loadRoutine(int routine_id) {
    QList<RoutineExercise *> exercises = Database::get()->getRoutineExercises(routine_id);

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_RoutineDetailList == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    ArrayDataModel* dataModel = dynamic_cast<ArrayDataModel*>(m_RoutineDetailList->dataModel());
    if (dataModel) {
        dataModel->clear();
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view
    for(int i = exercises.length()-1 ; i >= 0 ; --i) {
        dataModel->append(QString::number(exercises.at(i)->getId()) + ":" + exercises.at(i)->getTitle());
    }

}

void RoutineController::saveOrder() {
    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_RoutineDetailList == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    ArrayDataModel* dataModel = dynamic_cast<ArrayDataModel*>(m_RoutineDetailList->dataModel());
    if(!dataModel) return;

    for(int i = 0 ; i < dataModel->size() ; ++i) {

        QString encoded = dataModel->value(i).toString();
        QRegExp idReg("([0-9]+)");

        if(idReg.indexIn(encoded) != -1) {
            Database::get()->updateRoutineExercisePosition(idReg.cap(1).toInt(), i);
        }
    }
}


void RoutineController::addPlan(const QString& title, const QString& plan) {
    QRegExp exercise("([^\n]+)");


    int routine_id = Database::get()->getRoutineId(title);
    if(routine_id != -1) {
        bb::system::SystemToast *toast = new bb::system::SystemToast(this);

        toast->setBody(tr("You already have a routine called: \"") + title + tr("\". Please rename the existing one."));
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();
        return;
    }


    Database::get()->addRoutine(title);
    routine_id = Database::get()->getRoutineId(title);


    if(routine_id == -1) {
        qWarning() << "addPlan(): Routine ID is null. Should not happen, quit.";
        return;
    }

    int pos = exercise.indexIn(plan);
    int ex_pos = 0;
    while(pos != -1) {
        qDebug() << exercise.cap(1);

        int exercise_id = Database::get()->getExerciseId(exercise.cap(1));

        if(exercise_id == -1) {
            Database::get()->addExercise(exercise.cap(1), 2);
            exercise_id = Database::get()->getExerciseId(exercise.cap(1));
        }

        Database::get()->addExerciseToRoutine(routine_id, exercise_id, ex_pos);

        pos += exercise.matchedLength();
        pos = exercise.indexIn(plan, pos);
        ++ex_pos;
    }

    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

    toast->setBody(tr("The routine was added!"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();

    updateRoutineList();

}


void RoutineController::addExerciseToRoutine(int routine_id, int exercise_id) {
    Database::get()->addExerciseToRoutine(routine_id, exercise_id);

    loadRoutine(routine_id);
}

void RoutineController::removeExerciseFromRoutine(int routine_id, int routine_exercise_id) {
    Database::get()->removeExerciseFromRoutine(routine_exercise_id);
    loadRoutine(routine_id);
}

void RoutineController::removeExerciseFromRoutineNoAsk(int routine_id, int routine_exercise_id) {
    Database::get()->removeExerciseFromRoutine(routine_exercise_id);
}



// ------------------------------------------------------------------------
// control playback

void RoutineController::play(int routine_id) {
    m_Routine = Database::get()->getRoutineExercises(routine_id);

    if(m_Routine.isEmpty()) return;

    m_PlayListIndex = m_Routine.length()-1;

    open();
}

void RoutineController::playFromId(int routine_id, int idx) {
    m_Routine = Database::get()->getRoutineExercises(routine_id);

    if(m_Routine.isEmpty()) return;

    bool found = false;
    for(int i = 0 ; i < m_Routine.size() && !found ; ++i) {
        if(m_Routine.at(i)->getId() == idx) {
            found = true;
            m_PlayListIndex = i;
        }
    }

    if(found)
        open();

}

void RoutineController::open() {
    if(m_Routine.at(m_PlayListIndex)->getCategory() == 1)
        emit loadCardio(m_Routine.at(m_PlayListIndex)->getExercise(), m_Routine.at(m_PlayListIndex)->getTitle());
    else
        emit loadStrength(m_Routine.at(m_PlayListIndex)->getExercise(), m_Routine.at(m_PlayListIndex)->getTitle());
}

void RoutineController::stop() {

}

void RoutineController::next() {

    if(m_PlayListIndex > 0) {
        --m_PlayListIndex;
        open();
    } else {
        // close!
        using namespace bb::cascades;
        using namespace bb::system;

        SystemDialog *dialog = new SystemDialog("Yes", "No");

        dialog->setTitle(tr("End of routine"));
        dialog->setBody(tr("You have completed all the exercises, quit?"));

        bool success = connect(dialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onPromptFinishedEndRoutine(bb::system::SystemUiResult::Type)));

        if (success) {
            dialog->show();
        } else {
            dialog->deleteLater();
        }

    }

}



void RoutineController::prev() {
    if(m_PlayListIndex+1 < m_Routine.length()) {
        ++m_PlayListIndex;
        open();
    } else {
        // already first one!
        bb::system::SystemToast *toast = new bb::system::SystemToast(this);

        toast->setBody(tr("You are already at the first exercise of the list!"));
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();
    }
}



void RoutineController::onPromptFinishedEndRoutine(bb::system::SystemUiResult::Type result) {
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        emit closeCard();
    }
}


void RoutineController::invokeBrowser(const QString& url) {
    if(m_InvokeManager == NULL) {
        m_InvokeManager = new bb::system::InvokeManager(this);
    }

    bb::system::InvokeRequest request;
    request.setAction("bb.action.OPEN");
    request.setUri(url);

    m_InvokeManager->invoke(request);
}




