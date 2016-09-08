import bb.cascades 1.3
import Utility.ExerciseController 1.0
import bb.system 1.2

NavigationPane {
    id: nav

    Page {
        id: exercisePage
        property variant tpageCardio
        property variant tpageStrength
        
        property variant selectionColor: Color.create("#1ca9be")
        property variant darkColor: Color.create("#282828")
        
        
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
                    text: qsTr("List of exercises")
                    textStyle {
                        color: Color.White
                        fontSize: FontSize.Large 
                    }
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    defaultImageSource: "asset:///images/icon_add_white.png" 
                    preferredHeight: ui.du(7)
                    preferredWidth: ui.du(7)
                    onClicked: {
                        newExercise.open();
                    }
                
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
                        
                        multiSelectAction: MultiSelectActionItem {}
                        
                        multiSelectHandler {
                            // These actions will be shown during multiple selection, while this 
                            // multiSelectHandler is active
                            actions: [
                                DeleteActionItem {
                                    property variant selectionList
                                    property variant selectedItem
                                    id: deleteActionItem
                                    onTriggered: {
                                        deleteActionItem.selectionList = exercicesList.selectionList()
                                        deleteActionItem.selectedItem = theModel.data(selectionList);
                                        multiSelectDeleteDialog.show()
                                    }
                                    attachedObjects: [
                                        SystemDialog {
                                            id: multiSelectDeleteDialog
                                            title: qsTr("Delete exercises") + Retranslate.onLocaleOrLanguageChanged
                                            body: qsTr("Are you sure you want to delete these exercises?") + Retranslate.onLocaleOrLanguageChanged
                                            onFinished: {
                                                if (result == 3) {
                                                } else {
                                                    
                                                    for (var i = 0; i < deleteActionItem.selectionList.length; ++ i) {
                                                        console.log(i)
                                                        exerciseController.deleteExerciseNoAsk(theModel.data(deleteActionItem.selectionList[i]).id);
                                                    }
                                                    exerciseController.getExerciseList();
                                                }
                                            }
                                        }
                                    ]
                                }
                            ]
                            
                            status: qsTr("Delete")
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
                                    exercisePage.tpageCardio = practice.createObject();
                                }
                                
                                exercisePage.tpageCardio.category = 1;
                                exercisePage.tpageCardio.title = chosenItem.title;
                                exercisePage.tpageCardio.exercise_id = chosenItem.id;
                                
                                nav.push(exercisePage.tpageCardio);
                            } else {
                                if(!exercisePage.tpageStrength) {
                                    exercisePage.tpageStrength = practice.createObject();
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
                
                attachedObjects: [
                    ImagePaintDefinition {
                        id: back2
                        imageSource: "asset:///images/color/flat.amd"
                        repeatPattern: RepeatPattern.X
                    }
                ]
                
                content:  Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(2)
                    bottomPadding: ui.du(2)
                    
                    background: back2.imagePaint
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    preferredWidth: ui.du(60)
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    Label {
                        text: qsTr("Create a new exercise")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle.color: Color.White
                    }
                    
                    
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
