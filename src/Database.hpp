/*
 * Database.hpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <QtSql/QtSql>

class Exercise;
class Set;
class Cardio;
class Routine;
class RoutineExercise;
class BodyWeight;

class Database {



public:
    // ----------------------------------------------------------------------------------------------
    // member functions
    static Database* get            ();
    virtual ~Database               ();


    QList<Exercise*> getExerciseList(int routine_id = -1);
    QList<Routine*>  getRoutines();
    QList<RoutineExercise*> getRoutineExercises(int routine_id);

    void addRoutine                 (const QString &label);
    void deleteRoutine              (int id);
    void renameRoutine              (int id, const QString &label);

    void addExercise                (const QString &label, int category);
    void deleteExercise             (int id);
    void renameExercise             (int id, const QString &label);

    void addBodyWeight              (qint64 time, float weight, const QString& notes);
    void deleteBodyWeight           (int id);

    void saveSet                    (int exercise_id, const Set *set);
    void deletePracticeStrengthEntry(int id);

    void saveCardio                 (int exerciseId,
                                    int duration,
                                    qint64 time,
                                    int distance,
                                    int heartRate,
                                    int calories,
                                    const QString &notes) ;

    void deletePracticeCardioEntry  (int id);

    void addExerciseToRoutine       (int routine_id, int exercise_id, int position = -1);
    void removeExerciseFromRoutine  (int routine_exercise_id);
    void updateRoutineExercisePosition(int routine_exercise_id, int position);

    Cardio*        getCardio        (int exerciseId);
    Set*           getSet           (int exerciseId);
    QList<Cardio*> getHistoryCardio (int exerciseId, qint64 begin = 0, qint64 end = 0);
    QList<Set*>    getHistoryStrength(int exerciseId, qint64 begin = 0, qint64 end = 0);
    QDateTime      getLastExerciseDate();

    void           updatePractice   (Cardio *c);
    void           updatePractice   (Set *c);

    QList<QPair<QString, QList<Set*> > >    getHistoryStrength(qint64 begin = 0, qint64 end = 0);
    QList<QPair<QString, QList<Cardio*> > > getHistoryCardio  (qint64 begin = 0, qint64 end = 0);
    QList< BodyWeight* >                    getBodyWeights    (qint64 begin = 0, qint64 end = 0);




    void            saveDB(const QString &path);
    void            loadDB(const QString &path);
    void            exportCSV(const QString &path);

    bool            executeScript(const QString &path);


private:

    // ----------------------------------------------------------------------------------------------
    // member variables
    static Database *m_This;

    QSqlDatabase     m_Database;

    // singleton; hide constructor
    Database                        ();


    void initDatabase               ();


};



#endif /* DATABASE_HPP_ */
