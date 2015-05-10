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
                    text: qsTr("Theme")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                    margin.leftOffset: ui.du(1)
                }
            
            }
            Divider { }
            
            RadioGroup {
                id: theme
                Option { id: option1; text: "Bright"; selected: appSettings.theme == 1; value: 1 }
                Option { id: option2; text: "Dark";   selected: appSettings.theme != 1; value: 2 }
                
                onSelectedOptionChanged: {
                    appSettings.theme = theme.selectedOption.value;
                }
            }


            Container {
                preferredWidth: ui.du(5)
                preferredHeight: ui.du(5)
            }

            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                
                Label {
                    text: qsTr("Unit")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                    margin.leftOffset: ui.du(1)
                }
            
            }
            Divider { }


            RadioGroup {
                id: unit
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
                    text: qsTr("Backup & Export")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
            
            }
            Divider { }

            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                horizontalAlignment: HorizontalAlignment.Center
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
            }

            Button {
                text: qsTr("Export to CSV")
                horizontalAlignment: HorizontalAlignment.Fill
            }

            
        }
    }

    attachedObjects: [
        AppSettings {
            id: appSettings
        }
    ]
}
