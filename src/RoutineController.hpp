/*
 * RoutineController.hpp
 *
 *  Created on: 3 mai 2015
 *      Author: pierre
 */

#ifndef ROUTINECONTROLLER_HPP_
#define ROUTINECONTROLLER_HPP_


#include <bb/cascades/ListView>
#include <bb/system/SystemUiResult>

class RoutineExercise;

class RoutineController : public QObject {
    Q_OBJECT;


private:


    bb::cascades::ListView          *m_RoutineList;
    bb::cascades::ListView          *m_RoutineDetailList;
    int                              m_tmp_id;

    QList<RoutineExercise*>          m_Routine;
    int                              m_PlayListIndex;

public:
    RoutineController               (QObject *parent = 0);
    virtual ~RoutineController      ()                               {};



public Q_SLOTS:


    inline void setRoutineList      (QObject *list)                 { m_RoutineList = dynamic_cast<bb::cascades::ListView*>(list);};
    inline void setRoutineDetailList(QObject *list)                 { m_RoutineDetailList = dynamic_cast<bb::cascades::ListView*>(list);};
    void updateRoutineList          ();
    void loadRoutine                (int routine_id);

    void createRoutine              ();
    void onPromptFinishedAddRoutine (bb::system::SystemUiResult::Type);

    void deleteRoutine              (int id);
    void deleteRoutinesNoAsk        (int id);
    void onPromptFinishedDeleteRoutine(bb::system::SystemUiResult::Type);
    void renameRoutine             (int id, const QString &label);
    void onPromptFinishedRenameRoutine(bb::system::SystemUiResult::Type);

    void addExerciseToRoutine       (int routine_id, int exercise_id);
    void removeExerciseFromRoutine  (int routine_id, int routine_exercise_id);
    void removeExerciseFromRoutineNoAsk(int routine_id, int routine_exercise_id);

    void saveOrder                  ();


    // ------------------------------------------------------------------------
    // control playback

    void play(int routine_id);
    void playFromId(int routine_id, int idx);
    void open();
    void stop();
    void next();
    void prev();


    void onPromptFinishedEndRoutine(bb::system::SystemUiResult::Type);


Q_SIGNALS:

    void loadCardio(int exercise_id, const QString &title);
    void loadStrength(int exercise_id, const QString &title);
    void closeCard();
    void completed();

};



#endif /* ROUTINECONTROLLER_HPP_ */
