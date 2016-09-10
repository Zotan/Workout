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

    Q_PROPERTY( int criteria        READ getCriteria    WRITE setCriteria     NOTIFY criteriaChanged)
    Q_PROPERTY( QDateTime startTime READ getStartTime   WRITE setStartTime    NOTIFY startTimeChanged)
    Q_PROPERTY( QDateTime endTime   READ getEndTime     WRITE setEndTime      NOTIFY endTimeChanged)

private:

    bb::cascades::ListView          *m_ListView;
    Graph                           *m_GraphController;

    int                              m_CacheId;
    int                              m_CacheCategory;
    int                              m_CacheExerciseId;
    qint64                           m_CacheTimestamp;

    int                              m_Criteria;
    QDateTime                        m_StartGraph;
    QDateTime                        m_EndGraph;

public:
    ProgressController              (QObject *parent = 0);
    virtual ~ProgressController     ()                               {};


    inline int  getCriteria() const                                    { return m_Criteria; };
    inline void setCriteria(int v)                                     { if(m_Criteria != v) { m_Criteria = v; emit criteriaChanged();} };

    inline const QDateTime&  getStartTime() const                      { return m_StartGraph; };
    inline void setStartTime(const QDateTime& v)                       { if(m_StartGraph != v) { m_StartGraph = v; emit startTimeChanged();} };

    inline const QDateTime&  getEndTime() const                        { return m_EndGraph; };
    inline void setEndTime(const QDateTime& v)                         { if(m_EndGraph != v) { m_EndGraph = v; emit endTimeChanged();} };


public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setGraph             (QObject *gr)                    { m_GraphController = dynamic_cast<Graph*>(gr); };

    QDateTime initDate               () const                          { return QDateTime::currentDateTime().addYears(-1); }
    QDateTime initDateNow            () const                          { return QDateTime::currentDateTime(); }


    void getWeightsList              ();
    void deleteRecord                (int id);
    void deleteRecordNoAsk           (int id);
    void onPromptFinishedDeleteRecord(bb::system::SystemUiResult::Type);

    void plotBodyWeights             (const QDateTime &begin, const QDateTime &end);



    void loadStrengthHistory         (int exercise_id);
    void loadCardioHistory           (int exercise_id);
    void plotCardio                  (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);
    void plotStrength                (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);


    void deleteEntry                 (int id, int category, int exercise_id);
    void onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type result);
    void deleteRecord                (int id, int category);
    void updateRepIds                ();


Q_SIGNALS:

    void completed();
    void criteriaChanged();
    void startTimeChanged();
    void endTimeChanged();



};



#endif /* PROGRESSCONTROLLER_HPP_ */
