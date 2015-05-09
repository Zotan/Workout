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

import bb.cascades 1.3

TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("Policy")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                    about.open();
                }
            },
            ActionItem {
                title: qsTr("Settings")
                onTriggered: {
                    settings.open();
                }
            },
            ActionItem {
                title: qsTr("Report")
                imageSource: "asset:///bugReportIcons/BugReport_white.png"
                onTriggered: {
                    bugReportSheet.open();
                }
            },
            ActionItem {
                title: qsTr("Donate")
                imageSource: "asset:///images/icon_credit.png"
                onTriggered: {
                    payment.open();
                }
            }
        ]
    }

    
    
    Tab { 
        // Localized text with the dynamic translation and locale updates support
        id: tabSummary
        title: qsTr("Summary") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_home.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Summary.qml"
        }
    } 
    
    
    
    Tab { 
        // Localized text with the dynamic translation and locale updates support
        id: tabExercise
        title: qsTr("Exercise") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_exercise.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Exercise.qml"
        }
    } 
    
    
    Tab { 
        // Localized text with the dynamic translation and locale updates support
        id: tabRoutine
        title: qsTr("Routine") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_daily.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Routine.qml"
        }
    } 
    
    
    attachedObjects: [
        Sheet {
            id: settings
            Settings {
                onDone: {
                    settings.close();
                }
            }
        },
        Delegate {
            id: policyDelegate
            source: "Policy.qml"
        },
        Sheet {
            id: about
            content: policyDelegate.object
            onOpenedChanged: {
                if (opened)
                    policyDelegate.active = true;
            }
            onClosed: {
                policyDelegate.active = false;
            }
        },
        Delegate {
            id: bugReportDelegate
            source: "BugReport.qml"
        },
        Sheet {
            id: bugReportSheet
            content: bugReportDelegate.object
            onOpenedChanged: {
                if (opened)
                    bugReportDelegate.active = true;
            }
            onClosed: {
                bugReportDelegate.active = false;
            }
        },
        Delegate {
            id: paymentDelegate
            source: "Payment.qml"
        
        },
        Sheet {
            id: payment
            content: paymentDelegate.object
            onOpenedChanged: {
                if (opened)
                    paymentDelegate.active = true;
            }
            onClosed: {
                paymentDelegate.active = false;
            }
        }
    ]
}
