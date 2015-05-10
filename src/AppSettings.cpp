/*
 * AppSettings.cpp
 *
 *  Created on: 8 mai 2015
 *      Author: pierre
 */


#include "AppSettings.hpp"
#include <QSettings>
#include <bb/cascades/pickers/FilePicker>
#include <bb/system/SystemDialog>

#include "Database.hpp"

AppSettings::AppSettings(QObject *parent) : QObject(parent), m_Theme(0), m_Unit(0) {
    QSettings settings("Amonchakai", "Workout");

    m_Theme = settings.value("theme", 1).toInt();
    m_Unit  = settings.value("unit", 2).toInt();

}


void AppSettings::save() {
    QSettings settings("Amonchakai", "Workout");
    settings.setValue("theme", m_Theme);
    settings.setValue("unit", m_Unit);

}


void AppSettings::saveDB() {
    using namespace bb::cascades::pickers;

    FilePicker* filePicker = new FilePicker();
    filePicker->setType(FileType::Other);
    filePicker->setTitle("Select location");
    filePicker->setMode(FilePickerMode::Saver);
    filePicker->open();

    // Connect the fileSelected() signal with the slot.
    QObject::connect(filePicker,
        SIGNAL(fileSelected(const QStringList&)),
        this,
        SLOT(saveFileSelected(const QStringList&)));

    // Connect the canceled() signal with the slot.
    QObject::connect(filePicker,
        SIGNAL(canceled()),
        this,
        SLOT(canceled()));


}


void AppSettings::canceled() {

}


void AppSettings::saveFileSelected(const QStringList &files) {
    Database::get()->saveDB(files.first());
}


void AppSettings::loadDB() {

    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Load database"));
    dialog->setBody(tr("Loading a new database will replace all data by the one from the selected file. Are you really sure you want to continue? This cannot be undone!"));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedLoadDB(bb::system::SystemUiResult::Type)));

    if (success) {
        dialog->show();
    } else {
        dialog->deleteLater();
    }

}

void AppSettings::onPromptFinishedLoadDB(bb::system::SystemUiResult::Type result) {
    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {

        using namespace bb::cascades::pickers;

        FilePicker* filePicker = new FilePicker();
        filePicker->setType(FileType::Other);
        filePicker->setTitle("Select backup");
        filePicker->setMode(FilePickerMode::Picker);
        filePicker->open();

        // Connect the fileSelected() signal with the slot.
        QObject::connect(filePicker,
            SIGNAL(fileSelected(const QStringList&)),
            this,
            SLOT(loadFileSelected(const QStringList&)));

        // Connect the canceled() signal with the slot.
        QObject::connect(filePicker,
            SIGNAL(canceled()),
            this,
            SLOT(canceled()));

    }
}

void AppSettings::loadFileSelected (const QStringList& files) {
    Database::get()->loadDB(files.first());
}


