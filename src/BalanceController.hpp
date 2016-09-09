/*
 * BalanceController.hpp
 *
 *  Created on: 29 mai 2015
 *      Author: pierre
 */

#ifndef BALANCECONTROLLER_HPP_
#define BALANCECONTROLLER_HPP_

#include <bb/cascades/ListView>
#include <QDateTime>
#include <bb/system/SystemUiResult>

#include "Graph.hpp"


class BalanceController : public QObject {
    Q_OBJECT;


private:

    bb::cascades::ListView          *m_ListView;

    int                              m_tmp_id;



public:
    BalanceController              (QObject *parent = 0);
    virtual ~BalanceController     ()                               {};



public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };

    void getWeightsList              ();
    void saveWeight                  (float value, const QString& notes);
    void deleteRecord                (int id);
    void deleteRecordNoAsk           (int id);
    void onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type);


Q_SIGNALS:

    void completed();



};



#endif /* BALANCECONTROLLER_HPP_ */
