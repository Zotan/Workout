import bb.cascades 1.3
import Utility.ExerciseController 1.0

NavigationPane {
    id: nav

    Page {
        id: exercisePage
        property variant tpageCardio
        property variant tpageStrength
        
        titleBar: TitleBar {
            kind: TitleBarKind.FreeForm
            kindProperties: FreeFormTitleBarKindProperties {
                Container {
                    layout: DockLayout { }
                    leftPadding: 10
                    rightPadding: 10
                    
                    Label {
                        id: folder
                        text: qsTr("List of exercises")
                        textStyle {
                            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                        }
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    
                    ImageButton {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_add_white.png" : "asset:///images/icon_add.png" 
                        preferredHeight: ui.du(7)
                        preferredWidth: ui.du(7)
                        onClicked: {
                            newExercise.open();
                        }
                        
                    }
      
                }
            }
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
                    text: connectingActivity.running ?  qsTr("Loading, please wait.") : qsTr("No exercises available.")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                ActivityIndicator {
                    id: connectingActivity
                    preferredHeight: 60
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
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
                                
                                contextActions: [
                                    ActionSet {
                                        title: qsTr("Exercise")
                                        
                                        ActionItem {
                                            title: qsTr("Rename exercise")
                                            imageSource: "asset:///images/icon_write_context.png"
                                            onTriggered: {
                                                listItemContainer.ListItem.view.renameExercise(ListItemData.id, ListItemData.title);
                                            }
                                        }
                                        
                                        DeleteActionItem {
                                            title: qsTr("Delete exercise")
                                            onTriggered: {
                                                listItemContainer.ListItem.view.deleteExercise(ListItemData.id);
                                            }
                                        }
                                    }
                                ]
                                
                            }
                        }
                    ]
                    
                    function renameExercise(id, label) {
                        exerciseController.renameExercise(id, label);                        
                    }
                    
                    function deleteExercise(id) {
                        exerciseController.deleteExercise(id);
                    }
                                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        
                        if(chosenItem.category == 1) {
                            if(!exercisePage.tpageCardio) {
                                exercisePage.tpageCardio = practice.createObject(0);
                            }
                            
                            exercisePage.tpageCardio.category = 1;
                            exercisePage.tpageCardio.title = chosenItem.title;
                            exercisePage.tpageCardio.exercise_id = chosenItem.id;
                            
                            nav.push(exercisePage.tpageCardio);
                        } else {
                            if(!exercisePage.tpageStrength) {
                                exercisePage.tpageStrength = practice.createObject(0);
                            }
                            
                            exercisePage.tpageStrength.category = 2;
                            exercisePage.tpageStrength.title = chosenItem.title;
                            exercisePage.tpageStrength.exercise_id = chosenItem.id;
                            
                            nav.push(exercisePage.tpageStrength);
                        }
                    }
                }
            
            }
        }
        
        onCreationCompleted: {
            exerciseController.setListView(exercicesList);
            exerciseController.getExerciseList();
        }
        
        attachedObjects: [
            ExerciseController {
                id: exerciseController
                
                onCompleted: {
                    exercicesList.requestFocus();
                }
                
            },
            ComponentDefinition {
                id: practice
                source: "Practice.qml"
            },
            ComponentDefinition {
                id: practiceCardio
                source: "PracticeCardio.qml"
            },
            ComponentDefinition {
                id: practiceStrength
                source: "PracticeStrength.qml"
            },
            Dialog {
                id: newExercise
                
                onClosed: {
                    label.text = "";
                }
                
                content:  Container {
                    
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.White
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    preferredWidth: ui.du(60)
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    Divider { }
                    
                    Label {
                        text: qsTr("Create a new exercise")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                    }
                    
                    Divider { }
                    
                    TextField {
                        id: label
                        hintText: qsTr("Name")    
                        backgroundVisible: true
                    }
                    
                    Divider { }
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        horizontalAlignment: HorizontalAlignment.Fill
                        
                        Button {
                            text: qsTr("Cardio")
                            horizontalAlignment: HorizontalAlignment.Fill
                            onClicked: {
                                exerciseController.newExercise(label.text, 1);
                                newExercise.close();
                                exerciseController.getExerciseList();
                            }
                        }
                                                
                        Button {
                            text: qsTr("Strength")
                            horizontalAlignment: HorizontalAlignment.Fill
                            onClicked: {
                                exerciseController.newExercise(label.text, 2);
                                newExercise.close();
                                exerciseController.getExerciseList();
                            }
                        }
                    }
                    Button {
                        text: qsTr("Cancel")
                        horizontalAlignment: HorizontalAlignment.Fill
                        onClicked: {
                            newExercise.close();
                            
                        }
                    }
                    
                    Divider { }
                    
                }
                
            }
        ]
    }
}
