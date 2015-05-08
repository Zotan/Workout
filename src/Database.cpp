/*
 * Database.cpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */

#include "Database.hpp"
#include <qDebug>

#include "DataObject.hpp"

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
    query.bindValue(":repetition_id", set->getId());
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

void Database::saveCardio(int exerciseId,
                          int duration,
                          qint64 time,
                          int distance,
                          int heartRate,
                          int calories,
                          const QString &notes) {

    QDateTime date(QDate(2015,01,01), QTime(00,00,00));

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
        ex->setId(repetition_id);
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
        ex->setId(repetition_id);
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
    query.prepare("SELECT * FROM Cardio, Exercises WHERE Cardio.exercise_id = Cardio.id AND Exercises.Category = 1 AND time > :bg_time AND time < :ed_time ORDER BY time DESC ");
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




