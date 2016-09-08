import bb.cascades 1.3
import Utility.ExerciseController 1.0

Page {
    id: exercisePage
    property variant tpageCardio
    property variant tpageStrength
    property bool doNotCloseOnSelected
    
    signal exercisePicked(int id, string name);
    
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
                text: qsTr("Choose an exercise")
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
        
        Container {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout { }
            
            Container {  
                id: dataEmptyLabel
                visible: theModel.empty //model.isEmpty() will not work  
                horizontalAlignment: HorizontalAlignment.Center  
                verticalAlignment: VerticalAlignment.Center
                
                layout: DockLayout {}
                
                Label {
                    text: qsTr("No exercises available.")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                TextField {
                    id: search
                    hintText: qsTr("Search")
                    
                    onTextChanging: {
                        if(search.text.length > 2 || search.text.length == 0)
                            exerciseController.filter(search.text);
                    }
                }
                
                ListView {
                    id: exercicesList
                    focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                    
                    dataModel: GroupDataModel {
                        id: theModel
                        sortingKeys: ["id"]
                        grouping: ItemGrouping.None
                        
                        property bool empty: true
                        
                        
                        onItemAdded: {
                            empty = isEmpty();
                        }
                        onItemRemoved: {
                            empty = isEmpty();
                        }  
                        onItemUpdated: empty = isEmpty()  
                        
                        // You might see an 'unknown signal' error  
                        // in the QML-editor, guess it's a SDK bug.  
                        onItemsChanged: empty = isEmpty()
                    
                    }
                    
                                    
                    listItemComponents: [
                        ListItemComponent {
                            type: "item"
                            
                            
                            Container {
                                preferredHeight: ui.du(12)
                                id: listItemContainer
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                layout: DockLayout {
                                }
                                
                                Container {
                                    verticalAlignment: VerticalAlignment.Center
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Container {
                                        preferredWidth: ui.du(0.1)
                                    }
                                    Label {
                                        text: ListItemData.title
                                        verticalAlignment: VerticalAlignment.Center
                                    }
                                }
                                
                                
                                Divider { }
                            
                            }
                        }
                    ]
                    
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        
                        if(!doNotCloseOnSelected) {
                            exercisePickedId = chosenItem.id;
                            nav.pop();
                        } else {
                            exerciseLabel = chosenItem.title;
                            exercisePickedCategory = chosenItem.category;
                            exercisePickedId = chosenItem.id;
                        }
                    }
                }
            
            }
        }
    }
    
    onCreationCompleted: {
        exerciseController.setListView(exercicesList);
        exerciseController.getExerciseList();
        doNotCloseOnSelected = false;
    }
    
    attachedObjects: [
        ExerciseController {
            id: exerciseController
            
            onCompleted: {
                exercicesList.requestFocus();
            }
        
        }
    ]
}
