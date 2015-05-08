/*
 * AppSettings.cpp
 *
 *  Created on: 8 mai 2015
 *      Author: pierre
 */


#include "AppSettings.hpp"
#include <QSettings>

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
