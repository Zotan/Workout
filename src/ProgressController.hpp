/*
 * ProgressController.hpp
 *
 *  Created on: 9 sept. 2016
 *      Author: pierre
 */

#ifndef PROGRESSCONTROLLER_HPP_
#define PROGRESSCONTROLLER_HPP_

#include <bb/cascades/ListView>
#include <QDateTime>
#include <bb/system/SystemUiResult>

#include "Graph.hpp"


class ProgressController : public QObject {
    Q_OBJECT;


private:

    bb::cascades::ListView          *m_ListView;
    Graph                           *m_GraphController;

    int                              m_tmp_id;



public:
    ProgressController              (QObject *parent = 0);
    virtual ~ProgressController     ()                               {};



public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setGraph             (QObject *gr)                    { m_GraphController = dynamic_cast<Graph*>(gr); };

    QDateTime initDate               () const                          { return QDateTime::currentDateTime().addYears(-1); }


    void getWeightsList              ();
    void deleteRecord                (int id);
    void deleteRecordNoAsk           (int id);
    void onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type);

    void plotBodyWeights             (const QDateTime &begin, const QDateTime &end);



    void loadStrengthHistory         (int exercise_id);
    void loadCardioHistory           (int exercise_id);
    void plotCardio                  (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);
    void plotStrength                (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);



Q_SIGNALS:

    void completed();



};



#endif /* PROGRESSCONTROLLER_HPP_ */
