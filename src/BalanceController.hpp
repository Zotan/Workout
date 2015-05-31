/*
 * BalanceController.hpp
 *
 *  Created on: 29 mai 2015
 *      Author: pierre
 */

#ifndef BALANCECONTROLLER_HPP_
#define BALANCECONTROLLER_HPP_

#include <bb/cascades/ListView>
#include <bb/cascades/WebView>
#include <QDateTime>
#include <bb/system/SystemUiResult>


class BalanceController : public QObject {
    Q_OBJECT;


private:

    bb::cascades::ListView          *m_ListView;
    bb::cascades::WebView           *m_HistoryWeb;

    int                              m_tmp_id;



public:
    BalanceController              (QObject *parent = 0);
    virtual ~BalanceController     ()                               {};



public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setHistoryWebView    (QObject *web)                   { m_HistoryWeb = dynamic_cast<bb::cascades::WebView*>(web); };

    void getWeightsList              ();
    void saveWeight                  (float value, const QString& notes);
    void deleteRecord                (int id);
    void deleteRecordNoAsk           (int id);
    void onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type);

    void plotBodyWeights             (const QDateTime &begin, const QDateTime &end);


Q_SIGNALS:

    void completed();



};



#endif /* BALANCECONTROLLER_HPP_ */
