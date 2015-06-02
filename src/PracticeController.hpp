/*
 * PracticeController.hpp
 *
 *  Created on: 21 avr. 2015
 *      Author: pierre
 */

#ifndef PRACTICECONTROLLER_HPP_
#define PRACTICECONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <QDateTime>
#include <QReadWriteLock>
#include <bb/device/Led>
#include <bb/platform/Notification>
#include <bb/system/SystemUiResult>

#include "Graph.hpp"

class Set;


class PracticeController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString time       READ getTime       NOTIFY timeChanged)
    Q_PROPERTY( QString date       READ getDate       NOTIFY dateChanged)
    Q_PROPERTY( QString stopWatch  READ getStopWatch  NOTIFY stopWatchChanged)


    // -----------------------------------------------------------------------------------------------------
    Q_PROPERTY( float weight     READ getWeight       WRITE setWeight        NOTIFY weightChanged)
    Q_PROPERTY( int repetition   READ getRepetition   WRITE setRepetition    NOTIFY repetitionChanged)
    Q_PROPERTY( int sets         READ getSets         WRITE setSets          NOTIFY setsChanged)

    // -----------------------------------------------------------------------------------------------------
    Q_PROPERTY( int duration     READ getDuration     WRITE setDuration      NOTIFY durationChanged)
    Q_PROPERTY( int distance     READ getDistance     WRITE setDistance      NOTIFY distanceChanged)
    Q_PROPERTY( int heartRate    READ getHeartRate    WRITE setHeartRate     NOTIFY heartRateChanged)
    Q_PROPERTY( int calories     READ getCalories     WRITE setCalories      NOTIFY caloriesChanged)

    // -----------------------------------------------------------------------------------------------------

    Q_PROPERTY( QString notes   READ getNotes         WRITE setNotes         NOTIFY notesChanged)

private:

    bb::cascades::ListView          *m_ListView;
    bb::cascades::ListView          *m_HistoryListView;
    Graph                           *m_GraphController;
    bb::device::Led                 *m_Led;
    bb::platform::Notification      *notif;

    float                            m_Weight;
    int                              m_Repetition;
    int                              m_SetsNumber;

    int                              m_Duration;
    int                              m_Distance;
    int                              m_HeartRate;
    int                              m_Calories;

    QString                          m_Notes;

    QString                          m_Date;
    QString                          m_Time;

    QString                          m_StopWatch;
    int                              m_TimeStopWatchSec;
    qint64                           m_TargetTimeStopWatch;
    bool                             m_StopWatchRunning;
    QReadWriteLock                   m_StopWatchMutex;

    QList<Set*>                     m_Sets;


    int                              m_CacheId, m_CacheCategory, m_CacheExerciseId;


public:
    PracticeController              (QObject *parent = 0);
    virtual ~PracticeController     ()                               {};

    inline const QString &getTime() const                       { return m_Time; }
    inline const QString &getDate() const                       { return m_Date; }
    inline const QString &getStopWatch() const                  { return m_StopWatch; }

    inline float          getWeight() const                     { return m_Weight; }
    inline void           setWeight(float c)                    { if(m_Weight != c) { m_Weight = c; emit weightChanged();} }

    inline int            getRepetition() const                 { return m_Repetition; }
    inline void           setRepetition(int c)                  { if(m_Repetition != c) {m_Repetition = c; emit repetitionChanged();}}

    inline int            getSets() const                       { return m_SetsNumber; }
    inline void           setSets(int c)                        { if(m_SetsNumber != c) {m_SetsNumber = c; emit setsChanged();} }

    inline int            getDuration() const                   { return m_Duration; }
    inline void           setDuration(int c)                    { if(m_Duration != c) {m_Duration = c; emit durationChanged();} }

    inline int            getDistance() const                   { return m_Distance; }
    inline void           setDistance(int c)                    { if(m_Distance != c) {m_Distance = c; emit distanceChanged();} }

    inline int            getHeartRate() const                  { return m_HeartRate; }
    inline void           setHeartRate(int c)                   { if(m_HeartRate != c) {m_HeartRate = c; emit heartRateChanged();} }

    inline int            getCalories() const                   { return m_Calories; }
    inline void           setCalories(int c)                    { if(m_Calories != c) {m_Calories = c; emit caloriesChanged();} }

    inline const QString &getNotes() const                      { return m_Notes; }
    inline void           setNotes(const QString &c)            { if(m_Notes != c) {m_Notes = c; emit notesChanged();} }

public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setHistoryListView   (QObject *list)                  { m_HistoryListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setGraph             (QObject *gr)                    { m_GraphController = dynamic_cast<Graph*>(gr); };

    void saveSet                     (const QString &note);
    void updateDateTime              ();

    QDateTime initDate               () const                          { return QDateTime::currentDateTime().addYears(-1); }

    void pushToDB                    (int exerciseId);
    void pushCardioToDB              (int exerciseId, const QString &notes);

    void loadPractice                (int exercise_id, int category);
    void loadHistory                 (int exercise_id);
    void loadStrengthHistory         (int exercise_id);
    void restoreSession              (int exercise_id);

    void plotCardio                  (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);
    void plotStrength                (int exercise_id, const QDateTime &begin, const QDateTime &end, int criteria);

    QString getDateFromTime          (qint64 time);

    void deletePracticeEntry         (int id, int category, int exercise_id);
    void deletePracticeEntryNoAsk    (int id, int category);
    void onPromptFinishedDeletePractice(bb::system::SystemUiResult::Type);
    void updatePractice              (int category);


    void startStopWatch              ();
    void resetStopWatch              ();
    void addSecStopWatch             (int sec);
    void setSecStopWatch             (const QDateTime &time);
    QString getSecStopWatch          ();
    void stopWatchTick               ();

Q_SIGNALS:

    void completed();

    void weightChanged();
    void repetitionChanged();
    void setsChanged();
    void durationChanged();
    void distanceChanged();
    void heartRateChanged();
    void caloriesChanged();
    void notesChanged();


    void timeChanged();
    void dateChanged();
    void stopWatchChanged();



};



#endif /* PRACTICECONTROLLER_HPP_ */
