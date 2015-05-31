import bb.cascades 1.3
import Utility.AppSettings 1.0

Page {
    signal done()
    titleBar: TitleBar {
        title: qsTr("Settings")
        acceptAction: [
            ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    appSettings.save();
                    done();
                    
                }
            }
        ]
        
        dismissAction: [
            ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    done();
                }
            }
        ]
    }
    
    ScrollView {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            Container {
                preferredWidth: ui.du(5)
                preferredHeight: ui.du(5)
            }

            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                
                Label {
                    text: qsTr("General settings")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                    margin.leftOffset: ui.du(1)
                }
            
            }
            Divider { }
            
            DropDown {
                id: theme
                title: qsTr("Visual theme")
                Option { id: option1; text: "Bright"; selected: appSettings.theme == 1; value: 1 }
                Option { id: option2; text: "Dark";   selected: appSettings.theme != 1; value: 2 }
                
                onSelectedOptionChanged: {
                    appSettings.theme = theme.selectedOption.value;
                }
            }


            Container {
                preferredWidth: ui.du(1)
                preferredHeight: ui.du(1)
            }


            DropDown {
                id: unit
                title: qsTr("Unit")
                Option { id: optionU1; text: "Imperial"; selected: appSettings.unit == 1; value: 1 }
                Option { id: optionU2; text: "Metric";   selected: appSettings.unit != 1; value: 2 }
                
                onSelectedOptionChanged: {
                    appSettings.unit = unit.selectedOption.value;
                }
            }
            
            Container {
                preferredWidth: ui.du(5)
                preferredHeight: ui.du(5)
            }
            
            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                
                Label {
                    text: qsTr("Timer notification")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                    margin.leftOffset: ui.du(1)
                }
            
            }
            Divider { }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout { }
                
                Label {
                    text: qsTr("Enable LED")
                    leftMargin: ui.du(1)
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                    
                }
                
                ToggleButton {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    rightMargin: ui.du(1)
                    checked: appSettings.ledNotif == 1
                    onCheckedChanged: {
                        appSettings.ledNotif = checked;
                    }
                }
            }
            
            Container {
                preferredWidth: ui.du(2)
                preferredHeight: ui.du(2)
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout { }
                
                Label {
                    text: qsTr("Render timeout as a notification")
                    leftMargin: ui.du(1)
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                }
                
                ToggleButton {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    rightMargin: ui.du(1)
                    checked: appSettings.soundNotif == 1
                    onCheckedChanged: {
                        appSettings.soundNotif = checked;
                    }
                }
            }
            
            
            Container {
                preferredWidth: ui.du(5)
                preferredHeight: ui.du(5)
            }

            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                
                Label {
                    text: qsTr("Backup & Export")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
            
            }
            Divider { }

            Button {
                 text: qsTr("Save")
                 horizontalAlignment: HorizontalAlignment.Fill
                 onClicked: {
                     appSettings.saveDB();
                 }
            }
    
            Button {
                text: qsTr("Load")
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    appSettings.loadDB();
                }
            }

            Button {
                text: qsTr("Export to CSV")
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    appSettings.exportDB();
                }
            }

            
        }
    }

    attachedObjects: [
        AppSettings {
            id: appSettings
        }
    ]
}
