/*
 * Database.cpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */

#include "Database.hpp"
#include <qDebug>
#include <QFile>
#include <QDir>
#include <QObject>

#include <bb/system/SystemToast>

#include "DataObject.hpp"
#include "OSDaB/zip.h"
#include "OSDaB/unzip.h"

Database *Database::m_This = NULL;


Database::Database() {

    m_Database = QSqlDatabase::addDatabase("QSQLITE");

    // 2. Set the path of where the database will be located.
    //    Note: The db extension is not required
    m_Database.setDatabaseName("./data/workoutDatabase.db");

    // 3. Open a connection to the database, if the database does not exist
    //    one will be created if permitted.
    if (m_Database.open()) {
        qDebug() << "Database created/registered.";

        initDatabase();
    } else {
        // If the database fails to open, error information can be accessed via
        // the lastError function.
        const QSqlError error = m_Database.lastError();
        qDebug() << "Error opening connection to the database: " << error.text();
    }

}

Database *Database::get() {
    if(m_This == NULL)
        m_This = new Database();

    return m_This;
}

Database::~Database() {
    m_Database.close();
}


void Database::initDatabase() {

    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS Exercises ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                Title VARCHAR, "
                                      "                Category INTEGER "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }


    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS Sets ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                exercise_id INTEGER, "
                                      "                repetition_id INTEGER, "
                                      "                time NUMERIC, "
                                      "                note VARCHAR, "
                                      "                repetition INTEGER, "
                                      "                weight REAL "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }

    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS Cardio ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                exercise_id INTEGER, "
                                      "                duration INTEGER, "
                                      "                time NUMERIC, "
                                      "                distance INTEGER, "
                                      "                heart_rate INTEGER, "
                                      "                calories INTEGER, "
                                      "                notes VARCHAR "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }


    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS Routines ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                Title VARCHAR "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }

    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS DetailRoutines ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                routine_id INTEGER, "
                                      "                exercise_id INTEGER, "
                                      "                position INTEGER "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }


    {
        const QString createSQL = "CREATE TABLE IF NOT EXISTS BodyWeights ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                time NUMERIC, "
                                      "                weight REAL, "
                                      "                notes VARCHAR "
                                      ");";

        QSqlQuery query(m_Database);
        if (query.exec(createSQL)) {
            qDebug() << "Table created.";
        } else {
            // If 'exec' fails, error information can be accessed via the lastError function
            // the last error is reset every time exec is called.
            const QSqlError error = query.lastError();
            qDebug() << "Create table error:" << error.text();
        }
    }
}


void Database::addRoutine(const QString &label) {
    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO Routines (Title) VALUES(:title)");
    query.bindValue(":title", label);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item created.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert record error:" << error.text();
    }
}



void Database::addExercise(const QString &label, int category) {
    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO Exercises (Title, Category) VALUES(:title, :category)");
    query.bindValue(":title", label);
    query.bindValue(":category", QString::number(category));

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item created.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert record error:" << error.text();
    }
}

void Database::deleteExercise(int id) {
    QSqlQuery query(m_Database);
    query.prepare("DELETE FROM Exercises WHERE id = :id");
    query.bindValue(":id", id);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item deleted.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "delete record error:" << error.text();
    }
}

void Database::deleteRoutine(int id) {
    QSqlQuery query(m_Database);
    query.prepare("DELETE FROM Routines WHERE id = :id");
    query.bindValue(":id", id);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item deleted.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "delete record error:" << error.text();
    }
}


void Database::renameExercise(int id, const QString &label) {
    QSqlQuery query(m_Database);
    query.prepare("UPDATE Exercises SET \'Title\'= :label WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":label", label);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item updated.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "update record error:" << error.text();
    }
}

void Database::renameRoutine(int id, const QString &label) {
    QSqlQuery query(m_Database);
    query.prepare("UPDATE Routines SET \'Title\'= :label WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":label", label);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item updated.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "update record error:" << error.text();
    }
}

QList<Exercise*> Database::getExerciseList(int routine_id) {
    QList<Exercise*> exercises;


    QSqlQuery query(m_Database);
    if(routine_id == -1)
        query.prepare("SELECT * FROM Exercises");
    else {
        query.prepare("SELECT * FROM Exercises, DetailRoutines WHERE DetailRoutines.routine_id = :routine_id AND Exercises.id = DetailRoutines.exercise_id ORDER BY DetailRoutines.position ASC");
        query.bindValue(":routine_id", routine_id);
    }
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        int category = query.value(2).toInt();

        Exercise *ex = new Exercise();
        ex->setId(id);
        ex->setCategory(category);
        ex->setTitle(title);

        exercises.push_back(ex);
    }

    return exercises;
}

QList<RoutineExercise*> Database::getRoutineExercises(int routine_id) {
    QList<RoutineExercise*> exercises;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Exercises, DetailRoutines WHERE DetailRoutines.routine_id = :routine_id AND Exercises.id = DetailRoutines.exercise_id ORDER BY DetailRoutines.position DESC");
    query.bindValue(":routine_id", routine_id);
    query.exec();

    while (query.next()) {
        int ex_id = query.value(0).toInt();
        QString title = query.value(1).toString();
        int category = query.value(2).toInt();
        int id = query.value(3).toInt();
        int position = query.value(4).toInt();

        RoutineExercise *ex = new RoutineExercise();
        ex->setId(id);
        ex->setExercise(ex_id);
        ex->setCategory(category);
        ex->setTitle(title);
        ex->setPosition(position);

        exercises.push_back(ex);
    }

    return exercises;
}

QList<Routine*> Database::getRoutines() {
    QList<Routine*> routines;


    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Routines");
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();

        Routine *r = new Routine();
        r->setId(id);
        r->setTitle(title);

        routines.push_back(r);
    }

    return routines;
}


void Database::saveSet(int exercise_id, const Set *set) {

    if(set == NULL) return;

    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO Sets (exercise_id, repetition_id, time, note, repetition, weight) VALUES(:exercise_id, :repetition_id, :time, :note, :repetition, :weight)");
    query.bindValue(":exercise_id", exercise_id);
    query.bindValue(":repetition_id", set->getRepId());
    query.bindValue(":time", set->getTime());
    query.bindValue(":note", set->getNote());
    query.bindValue(":repetition", set->getRepetition());
    query.bindValue(":weight", set->getWeight());


    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "set added.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert set record error:" << error.text();
    }
}

void Database::deletePracticeStrengthEntry(int id) {

    QSqlQuery query(m_Database);
    query.prepare("DELETE FROM Sets WHERE id = :id");
    query.bindValue(":id", id);


    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "set deleted.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "set deleted error:" << error.text();
    }
}

void Database::saveCardio(int exerciseId,
                          int duration,
                          qint64 time,
                          int distance,
                          int heartRate,
                          int calories,
                          const QString &notes) {

    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO Cardio (exercise_id, duration, time, distance, heart_rate, calories, notes) VALUES(:exercise_id, :duration, :time, :distance, :heart_rate, :calories, :notes)");
    query.bindValue(":exercise_id", exerciseId);
    query.bindValue(":duration", duration);
    query.bindValue(":time", time);
    query.bindValue(":distance", distance);
    query.bindValue(":heart_rate", heartRate);
    query.bindValue(":calories", calories);
    query.bindValue(":notes", notes);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "cardio record added.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert cardio record error:" << error.text();
    }
}

void Database::deletePracticeCardioEntry(int id) {

    QSqlQuery query(m_Database);
    query.prepare("DELETE FROM Cardio WHERE id = :id");
    query.bindValue(":id", id);


    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "cardio entry deleted.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "cardio entry deleted error:" << error.text();
    }
}

/*
 *         const QString createSQL = "CREATE TABLE IF NOT EXISTS DetailRoutines ( "
                                      "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "                routine_id INTEGER, "
                                      "                exercise_id INTEGER, "
                                      "                position INTEGER "
                                      ");";
 */

void Database::addExerciseToRoutine(int routine_id, int exercise_id, int position) {

    int sel_position = position;
    if(position == -1) {
        QSqlQuery query(m_Database);
        query.prepare("SELECT position FROM DetailRoutines WHERE routine_id = :routine_id ORDER BY position DESC LIMIT 1 ");
        query.bindValue(":routine_id", routine_id);
        query.exec();

        if (query.next()) {
            sel_position = query.value(0).toInt()+1;
        } else {
            sel_position = 0;
        }
    }

    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO DetailRoutines (routine_id, exercise_id, position) VALUES(:routine_id, :exercise_id, :position)");
    query.bindValue(":exercise_id", exercise_id);
    query.bindValue(":routine_id", routine_id);
    query.bindValue(":position", sel_position);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "exercise added to routine.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert exercise into routine; record error:" << error.text();
    }
}

void Database::updateRoutineExercisePosition(int routine_exercise_id, int position) {
    QSqlQuery query(m_Database);
    query.prepare("UPDATE DetailRoutines SET \'position\'= :position WHERE id = :id");
    query.bindValue(":id", routine_exercise_id);
    query.bindValue(":position", position);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item updated.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "update record error:" << error.text();
    }

}

void Database::removeExerciseFromRoutine  (int routine_exercise_id) {
    QSqlQuery query(m_Database);

    query.prepare("DELETE FROM DetailRoutines WHERE id = :routine_exercise_id");
    query.bindValue(":routine_exercise_id", routine_exercise_id);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "exercise deleted from routine.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "exercise deleted from routine; record error:" << error.text();
    }
}


Cardio* Database::getCardio(int exerciseId) {
    Cardio* ex = NULL;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Cardio WHERE id = :id ");
    query.bindValue(":id", exerciseId);
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        int duration = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        int distance = query.value(4).toInt();
        int heartRate = query.value(5).toInt();
        int calories = query.value(6).toInt();
        QString notes = query.value(7).toString();

        ex = new Cardio();
        ex->setId(id);
        ex->setCalories(calories);
        ex->setDistance(distance);
        ex->setHeartRate(heartRate);
        ex->setNote(notes);
        ex->setTime(time);
        ex->setDuration(duration);

    }

    return ex;
}

Set* Database::getSet(int exerciseId) {
    Set* ex = NULL;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Sets WHERE id = :id");
    query.bindValue(":id", exerciseId);
    query.exec();

    // exercise_id, repetition_id, time, note, repetition, weight
    while (query.next()) {
        int repetition_id = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        QString notes = query.value(4).toString();
        int repetition = query.value(5).toInt();
        float weight = query.value(6).toInt();


        ex = new Set();
        ex->setId(query.value(0).toInt());
        ex->setRepId(repetition_id);
        ex->setRepetition(repetition);
        ex->setWeight(weight);
        ex->setNote(notes);
        ex->setTime(time);
    }

    return ex;
}



QList<Cardio*> Database::getHistoryCardio (int exerciseId, qint64 begin, qint64 end) {
    QList<Cardio*> exercises;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Cardio WHERE exercise_id = :exercise_id AND time > :bg_time AND time < :ed_time ORDER BY time DESC ");
    query.bindValue(":exercise_id", exerciseId);
    query.bindValue(":bg_time", begin == 0 ? QDateTime::currentDateTime().addDays(-30).toMSecsSinceEpoch() : begin);
    query.bindValue(":ed_time", end == 0 ? QDateTime::currentDateTime().toMSecsSinceEpoch() : end);
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        int duration = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        int distance = query.value(4).toInt();
        int heartRate = query.value(5).toInt();
        int calories = query.value(6).toInt();
        QString notes = query.value(7).toString();

        Cardio *ex = new Cardio();
        ex->setId(id);
        ex->setCalories(calories);
        ex->setDistance(distance);
        ex->setHeartRate(heartRate);
        ex->setNote(notes);
        ex->setTime(time);
        ex->setDuration(duration);

        exercises.push_back(ex);
    }

    return exercises;
}


QList<Set*> Database::getHistoryStrength(int exerciseId, qint64 begin, qint64 end) {
    QList<Set*> exercises;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Sets WHERE exercise_id = :exercise_id AND time > :bg_time AND time < :ed_time ORDER BY time DESC ");
    query.bindValue(":exercise_id", exerciseId);
    query.bindValue(":bg_time", begin == 0 ? QDateTime::currentDateTime().addDays(-30).toMSecsSinceEpoch() : begin);
    query.bindValue(":ed_time", end == 0 ? QDateTime::currentDateTime().toMSecsSinceEpoch() : end);
    query.exec();

    // exercise_id, repetition_id, time, note, repetition, weight
    while (query.next()) {
        int repetition_id = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        QString notes = query.value(4).toString();
        int repetition = query.value(5).toInt();
        float weight = query.value(6).toInt();


        Set *ex = new Set();
        ex->setId(query.value(0).toInt());
        ex->setRepId(repetition_id);
        ex->setRepetition(repetition);
        ex->setWeight(weight);
        ex->setNote(notes);
        ex->setTime(time);

        exercises.push_back(ex);
    }

    return exercises;
}

QList<QPair<QString, QList<Set*> > > Database::getHistoryStrength(qint64 begin, qint64 end) {
    QList<QPair<QString, QList<Set*> > > exercises;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Sets, Exercises WHERE Sets.exercise_id = Exercises.id AND Exercises.Category = 2 AND Sets.time > :bg_time AND Sets.time < :ed_time ORDER BY Sets.time DESC ");
    query.bindValue(":bg_time", begin == 0 ? QDateTime::currentDateTime().addDays(-30).toMSecsSinceEpoch() : begin);
    query.bindValue(":ed_time", end == 0 ? QDateTime::currentDateTime().toMSecsSinceEpoch() : end);
    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (!query.exec()) {
        const QSqlError error = query.lastError();
        qDebug() << "query error:" << error.text();
    }


    // exercise_id, repetition_id, time, note, repetition, weight
    while (query.next()) {
        int repetition_id = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        QString notes = query.value(4).toString();
        int repetition = query.value(5).toInt();
        float weight = query.value(6).toInt();
        QString label = query.value(8).toString();

        if(exercises.isEmpty())
            exercises.push_back(QPair<QString, QList<Set*> >(label, QList<Set*>()));
        else {
            if(exercises.last().first != label)
                exercises.push_back(QPair<QString, QList<Set*> >(label, QList<Set*>()));
        }


        Set *ex = new Set();
        ex->setId(query.value(0).toInt());
        ex->setRepId(repetition_id);
        ex->setRepetition(repetition);
        ex->setWeight(weight);
        ex->setNote(notes);
        ex->setTime(time);

        exercises.last().second.push_back(ex);
    }

    return exercises;
}


QList<QPair<QString, QList<Cardio*> > > Database::getHistoryCardio  (qint64 begin, qint64 end) {
    QList<QPair<QString, QList<Cardio*> > > exercises;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM Cardio, Exercises WHERE Cardio.exercise_id = Exercises.id AND Exercises.Category = 1 AND time > :bg_time AND time < :ed_time ORDER BY time DESC ");
    query.bindValue(":bg_time", begin == 0 ? QDateTime::currentDateTime().addDays(-30).toMSecsSinceEpoch() : begin);
    query.bindValue(":ed_time", end == 0 ? QDateTime::currentDateTime().toMSecsSinceEpoch() : end);
    if (!query.exec()) {
        const QSqlError error = query.lastError();
        qDebug() << "query error:" << error.text();
    }

    while (query.next()) {

        int id = query.value(0).toInt();
        int duration = query.value(2).toInt();
        qint64 time = static_cast<qint64>(query.value(3).toLongLong());
        int distance = query.value(4).toInt();
        int heartRate = query.value(5).toInt();
        int calories = query.value(6).toInt();
        QString notes = query.value(7).toString();
        QString label = query.value(9).toString();

        if(exercises.isEmpty())
            exercises.push_back(QPair<QString, QList<Cardio*> >(label, QList<Cardio*>()));
        else {
            if(exercises.last().first != label)
                exercises.push_back(QPair<QString, QList<Cardio*> >(label, QList<Cardio*>()));
        }


        Cardio *ex = new Cardio();
        ex->setId(id);
        ex->setCalories(calories);
        ex->setDistance(distance);
        ex->setHeartRate(heartRate);
        ex->setNote(notes);
        ex->setTime(time);
        ex->setDuration(duration);

        exercises.last().second.push_back(ex);
    }


    return exercises;
}

void Database::updatePractice(Cardio *c) {
    if(c == NULL) return;

    QSqlQuery query(m_Database);
    query.prepare("UPDATE Cardio SET duration = :duration, distance = :distance, heart_rate = :heart_rate, calories = :calories, notes = :notes WHERE id = :id");
    query.bindValue(":id", c->getId());
    query.bindValue(":duration", c->getDuration());
    query.bindValue(":distance", c->getDistance());
    query.bindValue(":heart_rate", c->getHeartRate());
    query.bindValue(":calories", c->getCalories());
    query.bindValue(":notes", c->getNote());

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "cardio record updated.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "update cardio record error:" << error.text();
    }
}

void Database::updatePractice(Set *set) {
    if(set == NULL) return;

    QSqlQuery query(m_Database);
    query.prepare("UPDATE Sets SET note = :note, repetition = :repetition, weight = :weight WHERE id = :id");
    query.bindValue(":note", set->getNote());
    query.bindValue(":repetition", set->getRepetition());
    query.bindValue(":weight", set->getWeight());
    query.bindValue(":id", set->getId());


    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "set updated.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "update set record error:" << error.text();
    }
}


QDateTime Database::getLastExerciseDate() {
    QDateTime date;

    qint64 date_64 = 0;

    {
        QSqlQuery query(m_Database);
        query.prepare("SELECT time FROM Sets ORDER BY time DESC LIMIT 1 ");
        query.exec();


        if(query.next()) {
            date_64 = query.value(0).toLongLong();
        }
    }


    {
        QSqlQuery query(m_Database);
        query.prepare("SELECT time FROM Cardio ORDER BY time DESC LIMIT 1 ");
        query.exec();


        if(query.next()) {
            date_64 = date_64 > query.value(0).toLongLong() ? date_64 : query.value(0).toLongLong();
        }
    }

    if(date_64 != 0) {
        date = QDateTime::fromMSecsSinceEpoch(date_64);
    }

    return date;
}



QList< BodyWeight* > Database::getBodyWeights(qint64 begin, qint64 end) {
    QList< BodyWeight* > result;

    QSqlQuery query(m_Database);
    query.prepare("SELECT * FROM BodyWeights WHERE time > :bg_time AND time < :ed_time ORDER BY time DESC ");
    query.bindValue(":bg_time", begin == 0 ? QDateTime::currentDateTime().addDays(-30).toMSecsSinceEpoch() : begin);
    query.bindValue(":ed_time", end == 0 ? QDateTime::currentDateTime().toMSecsSinceEpoch() : end);
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        qint64 time = static_cast<qint64>(query.value(1).toLongLong());
        float weight = query.value(2).toFloat();
        QString notes = query.value(3).toString();

        BodyWeight *ex = new BodyWeight();
        ex->setId(id);
        ex->setNote(notes);
        ex->setTime(QDateTime::fromMSecsSinceEpoch(time).date().toString(Qt::DefaultLocaleShortDate));
        ex->setWeight(weight);

        result.push_back(ex);
    }


    return result;
}

void Database::addBodyWeight(qint64 time, float weight, const QString& notes) {
    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO BodyWeights (time, weight, notes) VALUES(:time, :weight, :notes)");
    query.bindValue(":time", time);
    query.bindValue(":weight", weight);
    query.bindValue(":notes", notes);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item created.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "insert record error:" << error.text();
    }
}

void Database::deleteBodyWeight(int id) {
    QSqlQuery query(m_Database);
    query.prepare("DELETE FROM BodyWeights WHERE id = :id");
    query.bindValue(":id", id);

    // Note that no SQL Statement is passed to 'exec' as it is a prepared statement.
    if (query.exec()) {
        qDebug() << "item deleted.";
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        qDebug() << "delete record error:" << error.text();
    }
}

// ------------------------------------------------------------------------------------------------------
// IO Database

void Database::saveDB(const QString &path) {
    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        if (!QFile::exists(directory)) {
            QDir dir;
            dir.mkpath(directory);
        }

        QFile file(directory + "/Exercises.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS Exercises ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                Title VARCHAR, "
                                          "                Category INTEGER "
                                          ");";


            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM Exercises");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {

                QString title = query.value(1).toString();
                title.replace("\'", "\'\'");

                stream << "INSERT INTO Exercises (id, Title, Category) VALUES ( " + query.value(0).toString() + ", \'" + title + " \', " + query.value(2).toString() + ");\n";
            }


            file.close();
        } else {
            qDebug()<< "file NOT opened.";
        }

    }


    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        QFile file(directory + "/Sets.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS Sets ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                exercise_id INTEGER, "
                                          "                repetition_id INTEGER, "
                                          "                time NUMERIC, "
                                          "                note VARCHAR, "
                                          "                repetition INTEGER, "
                                          "                weight REAL "
                                          ");";


            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM Sets");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {

                QString notes = query.value(4).toString();
                notes.replace("\'", "\'\'");

                stream << "INSERT INTO Sets (id, exercise_id, repetition_id, time, note, repetition, weight) VALUES ( "
                        + query.value(0).toString()
                        + ", " + query.value(1).toString()
                        + ", " + query.value(2).toString()
                        + ", " + query.value(3).toString()
                        + ", \'" + notes + " \'"
                        + ", " + query.value(5).toString()
                        + ", " + query.value(6).toString() + "); \n";
            }


            file.close();
        }

    }

    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        QFile file(directory + "/Cardio.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS Cardio ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                exercise_id INTEGER, "
                                          "                duration INTEGER, "
                                          "                time NUMERIC, "
                                          "                distance INTEGER, "
                                          "                heart_rate INTEGER, "
                                          "                calories INTEGER, "
                                          "                notes VARCHAR "
                                          ");";


            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM Cardio");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {

                QString notes = query.value(7).toString();
                notes.replace("\'", "\'\'");

                stream << "INSERT INTO Cardio (id, exercise_id, duration, time, distance, heart_rate, calories, notes) VALUES ( "
                        + query.value(0).toString()
                        + ", " + query.value(1).toString()
                        + ", " + query.value(2).toString()
                        + ", " + query.value(3).toString()
                        + ", " + query.value(4).toString()
                        + ", " + query.value(5).toString()
                        + ", " + query.value(6).toString()
                        + ", \'" + notes + " \'); \n";
            }


            file.close();
        }

    }


    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        QFile file(directory + "/Routines.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS Routines ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                Title VARCHAR "
                                          ");";

            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM Routines");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {

                QString title = query.value(1).toString();
                title.replace("\'", "\'\'");

                stream << "INSERT INTO Routines (id, Title) VALUES ( "
                        + query.value(0).toString()
                        + ", \'" + title + "\'); \n";
            }


            file.close();
        }

    }

    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        QFile file(directory + "/DetailRoutines.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS DetailRoutines ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                routine_id INTEGER, "
                                          "                exercise_id INTEGER, "
                                          "                position INTEGER "
                                          ");";

            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM DetailRoutines");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {
                stream << "INSERT INTO DetailRoutines (id, routine_id, exercise_id, position) VALUES ( "
                        + query.value(0).toString()
                        + ", " + query.value(1).toString()
                        + ", " + query.value(2).toString()
                        + ", " + query.value(3).toString() + "); \n";
            }


            file.close();
        }
    }

    {
        QString directory = QDir::homePath() + QLatin1String("/Backup");
        QFile file(directory + "/BodyWeights.sql");

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            const QString createSQL = "CREATE TABLE IF NOT EXISTS BodyWeights ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                time NUMERIC, "
                                          "                weight REAL, "
                                          "                notes VARCHAR "
                                          ");";

            stream << createSQL << "\n";

            QSqlQuery query(m_Database);
            query.prepare("SELECT * FROM BodyWeights");
            if (!query.exec()) {
                const QSqlError error = query.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query.next()) {

                QString notes = query.value(3).toString() ;
                notes.replace("\'", "\'\'");

                stream << "INSERT INTO BodyWeights (id, time, weight, notes) VALUES ( "
                        + query.value(0).toString()
                        + ", " + query.value(1).toString()
                        + ", " + query.value(2).toString()
                        + ", \'" + notes + "\'); \n";
            }


            file.close();
        }
    }
    Zip uz;
    Zip::ErrorCode ec = uz.createArchive(path + ".zip");
    if (ec != Zip::Ok)
        return;

    QString directory = QDir::homePath() + QLatin1String("/Backup");
    uz.addFile(directory + "/Exercises.sql");
    uz.addFile(directory + "/Sets.sql");
    uz.addFile(directory + "/Cardio.sql");
    uz.addFile(directory + "/Routines.sql");
    uz.addFile(directory + "/DetailRoutines.sql");
    uz.addFile(directory + "/BodyWeights.sql");

    ec = uz.closeArchive();
}


void Database::loadDB(const QString &path) {
    QString directory = QDir::homePath() + QLatin1String("/Backup");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QDir dir(directory);
    QStringList fileList = dir.entryList();
    for(int i = 2 ; i < fileList.length() ; ++i) {
        QFile(directory + fileList[i]).remove();
    }

    UnZip uz;
    UnZip::ErrorCode ec = uz.openArchive(path);

    if (ec != UnZip::Ok) {
        bb::system::SystemToast *toast = new bb::system::SystemToast();

        toast->setBody("This is not a valid archive!");
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();
        return;
    }

    ec = uz.extractAll(directory);
    if (ec != UnZip::Ok) {
        bb::system::SystemToast *toast = new bb::system::SystemToast();

        toast->setBody("Oups.. cannot read the content of the file!");
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();
        return;
    }


    bool exec_scripts = true;
    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM Exercises WHERE id > 0;");
        exec_scripts = query.exec();

        exec_scripts = exec_scripts && executeScript(directory + "/Exercises.sql");
    }

    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM Sets WHERE id > 0;");
        exec_scripts = query.exec();

        exec_scripts = exec_scripts && executeScript(directory + "/Sets.sql");
    }


    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM Cardio WHERE id > 0;");
        exec_scripts = query.exec();

        exec_scripts = exec_scripts && executeScript(directory + "/Cardio.sql");
    }

    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM Routines WHERE id > 0;");
        exec_scripts = query.exec();

        executeScript(directory + "/Routines.sql");
    }

    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM DetailRoutines WHERE id > 0;");
        exec_scripts = query.exec();

        exec_scripts = exec_scripts && executeScript(directory + "/DetailRoutines.sql");
    }

    if(exec_scripts) {
        QSqlQuery query(m_Database);
        query.prepare("DELETE FROM BodyWeights WHERE id > 0;");
        exec_scripts = query.exec();

        exec_scripts = exec_scripts && executeScript(directory + "/BodyWeights.sql");
    }

    if(!exec_scripts) {
        bb::system::SystemToast *toast = new bb::system::SystemToast();

        toast->setBody("Something went wrong during the loading process... All data will not be necessarily available... :(");
        toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
        toast->show();
        return;
    }
}


bool Database::executeScript(const QString &path) {
    QFile file(path);

    bool result = true;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {

            QString line = in.readLine();

            QSqlQuery query(m_Database);
            query.prepare(line);

            if (!query.exec()) {
                result = false;
                qWarning() << "Error: " << query.lastError().text() << " " << line;
            }
        }
        file.close();
    } else {
        return false;
    }

    return result;
}


void Database::exportCSV(const QString &path) {
    QFile file(path + ".csv");


    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        QSqlQuery query(m_Database);
        query.prepare("SELECT * FROM Exercises, Sets WHERE Sets.exercise_id = Exercises.id;");
        if (!query.exec()) {
            const QSqlError error = query.lastError();
            qDebug() << "query error:" << error.text();
        }

        /*
         *             const QString createSQL = "CREATE TABLE IF NOT EXISTS Sets ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                exercise_id INTEGER, "
                                          "                repetition_id INTEGER, "
                                          "                time NUMERIC, "
                                          "                note VARCHAR, "
                                          "                repetition INTEGER, "
                                          "                weight REAL "
                                          ");";

                        const QString createSQL = "CREATE TABLE IF NOT EXISTS Exercises ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                Title VARCHAR, "
                                          "                Category INTEGER "
                                          ");";


         *             const QString createSQL = "CREATE TABLE IF NOT EXISTS Cardio ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                exercise_id INTEGER, "
                                          "                duration INTEGER, "
                                          "                time NUMERIC, "
                                          "                distance INTEGER, "
                                          "                heart_rate INTEGER, "
                                          "                calories INTEGER, "
                                          "                notes VARCHAR "
                                          ");";

                      const QString createSQL = "CREATE TABLE IF NOT EXISTS BodyWeights ( "
                                          "                id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "                time NUMERIC, "
                                          "                weight REAL, "
                                          "                notes VARCHAR "
                                          ");";
         */

        while (query.next()) {
            stream << query.value(1).toString() << ", "
                   << (query.value(2).toInt() == 1 ? "Cardio" : "Strength") << ", "
                   << query.value(5).toString() << ", "
                   << QDateTime::fromMSecsSinceEpoch(query.value(6).toLongLong()).toString() << ", "
                   << query.value(7).toString() << ", "
                   << query.value(8).toString() << ", "
                   << query.value(9).toString() << "\n";
        }




        {
            QSqlQuery query2(m_Database);
            query2.prepare("SELECT * FROM Exercises, Cardio WHERE Cardio.exercise_id = Exercises.id;");
            if (!query2.exec()) {
                const QSqlError error = query2.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query2.next()) {
                stream << query2.value(1).toString() << ", "
                       << (query2.value(2).toInt() == 1 ? "Cardio" : "Strength") << ", "
                       << query2.value(5).toString() << ", "
                       << QDateTime::fromMSecsSinceEpoch(query2.value(6).toLongLong()).toString() << ", "
                       << query2.value(7).toString() << ", "
                       << query2.value(8).toString() << ", "
                       << query2.value(9).toString() << ", "
                       << query2.value(10).toString() << "\n";
            }
        }


        {
            QSqlQuery query2(m_Database);
            query2.prepare("SELECT * FROM BodyWeights;");
            if (!query2.exec()) {
                const QSqlError error = query2.lastError();
                qDebug() << "query error:" << error.text();
            }

            while (query2.next()) {
                stream << QDateTime::fromMSecsSinceEpoch(query2.value(1).toLongLong()).toString() << ", "
                       << query2.value(2).toString() << ", "
                       << query2.value(3).toString() << "\n";
            }
        }


        file.close();
    }

}




