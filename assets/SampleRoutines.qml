import bb.cascades 1.4

Page {
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }            
        
        Container {
            background: back.imagePaint
            layout: DockLayout { }
            leftPadding: 10
            rightPadding: 10
            
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: ui.du(12)
            
            Label {
                id: folder
                text: qsTr("Find a program")
                textStyle {
                    color: Color.White
                    fontSize: FontSize.Large 
                }
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }            
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: back
                    imageSource: "asset:///images/color/gradient.png"
                    repeatPattern: RepeatPattern.X
                }
            ]
        }
        
        ListView {
            dataModel: XmlDataModel {
                source: "asset:///SampleRoutine.xml"
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    CustomListItem {
                        id: customListItem
                        
                        Container {
                            id: listItemContainer
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container {
                                    preferredWidth: ui.du(0.1)
                                }
                                Label {
                                    text: ListItemData.title
                                    verticalAlignment: VerticalAlignment.Center
                                    textStyle.fontSize: FontSize.Large
                                    textStyle.color: Color.create("#1ca9be")
                                }
                            }
                            
                            Container {
                                preferredHeight: ui.du(1)
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container {
                                    preferredWidth: ui.du(0.1)
                                }
                                Label {
                                    text: ListItemData.description
                                    verticalAlignment: VerticalAlignment.Center
                                }
                            }
                            
                            Container {
                                preferredHeight: ui.du(1)
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container {
                                    preferredWidth: ui.du(0.1)
                                }
                                Label {
                                    text: ListItemData.text
                                    verticalAlignment: VerticalAlignment.Center
                                    multiline: true
                                    textStyle.color: Color.LightGray
                                }
                            }
                            
                            Button {
                                horizontalAlignment: HorizontalAlignment.Fill
                                text: qsTr("Explanation video")
                                onClicked: {
                                    customListItem.ListItem.view.invokeBrowser(ListItemData.url);
                                }
                            }
                            
                            Container {
                                preferredHeight: ui.du(2)
                            }
                        
                        }
                                                
                    }
                
                }
                
            ]
            
            function invokeBrowser(url) {
                routineController.invokeBrowser(url);
            }
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                
                routineController.addPlan(chosenItem.title, chosenItem.text);
              
            }
            
        }
        
        
    }
}
