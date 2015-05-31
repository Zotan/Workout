/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/device/DisplayInfo>

#include "ExerciseController.hpp"
#include "PracticeController.hpp"
#include "SummaryController.hpp"
#include "RoutineController.hpp"
#include "AppSettings.hpp"
#include "BalanceController.hpp"

#include "BugReportController.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();


    qmlRegisterType<ExerciseController>("Utility.ExerciseController", 1, 0, "ExerciseController");
    qmlRegisterType<PracticeController>("Utility.PracticeController", 1, 0, "PracticeController");
    qmlRegisterType<SummaryController>("Utility.SummaryController", 1, 0, "SummaryController");
    qmlRegisterType<RoutineController>("Utility.RoutineController", 1, 0, "RoutineController");
    qmlRegisterType<BalanceController>("Utility.BalanceController", 1, 0, "BalanceController");
    qmlRegisterType<AppSettings>("Utility.AppSettings", 1, 0, "AppSettings");

    qmlRegisterType<BugReportController>("Lib.BugReport", 1, 0, "BugReport");


    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);

    bb::device::DisplayInfo display;
    QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
    displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
    displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
    qml->setContextProperty( "DisplayInfo", displayDimensions );
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("Workout_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
