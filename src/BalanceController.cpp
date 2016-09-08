/*
 * BalanceController.cpp
 *
 *  Created on: 29 mai 2015
 *      Author: pierre
 */

#include "BalanceController.hpp"
#include "Database.hpp"
#include "DataObject.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/system/SystemDialog>

BalanceController::BalanceController(QObject *parent) : QObject(parent),  m_ListView(NULL), m_tmp_id(0) {

}


void BalanceController::getWeightsList() {

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

    QList< BodyWeight* > bodyweights = Database::get()->getBodyWeights();
    for(int i = 0 ; i < bodyweights.size() ; ++i) {
        datas.push_back(bodyweights[i]);
    }

    dataModel->insertList(datas);

    emit completed();

}


void BalanceController::saveWeight(float value, const QString& notes) {
    Database::get()->addBodyWeight(QDateTime::currentMSecsSinceEpoch(), value, notes);

    getWeightsList();
}


void BalanceController::plotBodyWeights(const QDateTime &begin, const QDateTime &end) {

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

void BalanceController::deleteRecord(int id) {
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

void BalanceController::deleteRecordNoAsk(int id) {
    Database::get()->deleteBodyWeight(id);
}

void BalanceController::onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type type) {
    if(type == bb::system::SystemUiResult::ConfirmButtonSelection) {

        Database::get()->deleteBodyWeight(m_tmp_id);

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

