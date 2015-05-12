/*
 * ExerciseController.cpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */


#include "ExerciseController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/system/SystemDialog>
#include <bb/system/SystemPrompt>

#include "Database.hpp"
#include "DataObject.hpp"
#include <QSettings>
#include <QDir>

ExerciseController::ExerciseController(QObject *parent) : QObject(parent), m_ListView(NULL), m_tmp_id(0) {

}

void ExerciseController::getExerciseList() {

    QSettings settings("Amonchakait", "Workout");
    if(settings.value("FIRST_START", "") == "") {
        settings.setValue("FIRST_START", "DONE");

        using namespace bb::cascades;
        using namespace bb::system;

        SystemDialog *dialog = new SystemDialog("Yes", "No");

        dialog->setTitle(tr("Initialize exercises"));
        dialog->setBody(tr("You are running the application for the first time, do you want to load the pre-defined list of exercises?"));

        bool success = connect(dialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onPromptFinishedInitExercise(bb::system::SystemUiResult::Type)));

        if (success) {
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


    QList<Exercise*> exercises = Database::get()->getExerciseList();

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
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "id"
                              << "title"
                              << "category"
                );
        m_ListView->setDataModel(dataModel);
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<QObject*> datas;
    for(int i = exercises.length()-1 ; i >= 0 ; --i) {
        datas.push_back(exercises.at(i));
    }

    dataModel->clear();
    dataModel->insertList(datas);

    emit completed();

}

void ExerciseController::onPromptFinishedInitExercise(bb::system::SystemUiResult::Type result) {
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        Database::get()->executeScript(QDir::currentPath() + "/app/native/assets/populate_exercises.sql");

        getExerciseList();
    }
}

void ExerciseController::filter(const QString &key) {
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
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "id"
                              << "title"
                              << "category"
                );
        m_ListView->setDataModel(dataModel);
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<Exercise*> exercises = Database::get()->getExerciseList();

    QList<QObject*> datas;
    for(int i = exercises.length()-1 ; i >= 0 ; --i) {
        if(exercises.at(i)->getTitle().lastIndexOf(key) != -1)
            datas.push_back(exercises.at(i));
    }

    dataModel->clear();
    dataModel->insertList(datas);
}


void ExerciseController::newExercise(const QString &label, int type) {
    if(label.isEmpty()) return;

    Database::get()->addExercise(label, type);
}

void ExerciseController::deleteExercise(int id) {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Delete Exercise"));
    dialog->setBody(tr("Are you sure you want to delete this exercise? All related practices will be deleted as well..."));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedDeleteExercise(bb::system::SystemUiResult::Type)));

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

void ExerciseController::onPromptFinishedDeleteExercise(bb::system::SystemUiResult::Type type) {

    if(type == bb::system::SystemUiResult::ConfirmButtonSelection) {

        Database::get()->deleteExercise(m_tmp_id);

        if(m_ListView == NULL) {
            qWarning() << "did not received the listview. quit.";
            return;
        }

        using namespace bb::cascades;
        GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
        QVariantList indexPath = dataModel->find(QVariantList() << m_tmp_id);
        dataModel->removeAt(indexPath);

    }
}

void ExerciseController::renameExercise(int id, const QString &label) {
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Rename exercise"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setDefaultText(label);

    m_tmp_id = id;

    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedRenameExercise(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }

}

void ExerciseController::onPromptFinishedRenameExercise(bb::system::SystemUiResult::Type result) {
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {


        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            Database::get()->renameExercise(m_tmp_id, prompt->inputFieldTextEntry());

            if(m_ListView == NULL) {
                qWarning() << "did not received the listview. quit.";
                return;
            }

            getExerciseList();

            prompt->deleteLater();
        }
    }
}

