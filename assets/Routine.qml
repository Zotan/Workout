import bb.cascades 1.3
import Utility.RoutineController 1.0
import bb.system 1.2

NavigationPane {
    id: nav
    property variant pageRoutineDetail
    
    property variant cardioPage
    property variant strengthPage
    property variant practicePage
    property int navDepth

    
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
                    text: qsTr("List of routines")
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
                        routineController.createRoutine();
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
                        text: qsTr("No routines available.")
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                
                ListView {
                    id: listRoutines
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
                                    deleteActionItem.selectionList = listRoutines.selectionList()
                                    deleteActionItem.selectedItem = theModel.data(selectionList);
                                    multiSelectDeleteDialog.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: multiSelectDeleteDialog
                                        title: qsTr("Delete routines") + Retranslate.onLocaleOrLanguageChanged
                                        body: qsTr("Are you sure you want to delete these routines?") + Retranslate.onLocaleOrLanguageChanged
                                        onFinished: {
                                            if (result == 3) {
                                            } else {
                                                
                                                for (var i = 0; i < deleteActionItem.selectionList.length; ++ i) {
                                                    console.log(i)
                                                    routineController.deleteRoutinesNoAsk(theModel.data(deleteActionItem.selectionList[i]).id);
                                                }
                                                routineController.updateRoutineList();
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
                                id: listItemContainer
                                
                                preferredHeight: ui.du(12)
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                layout: DockLayout {
                                }
                                
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    verticalAlignment: VerticalAlignment.Center
                                    
                                    Container {
                                        preferredWidth: ui.du(.2)
                                    }
                                    
                                    Label {
                                        text: ListItemData.title
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Left
                                    }
                                }
                                
                                
                                Divider { }
                                
                                contextActions: [
                                    ActionSet {
                                        title: qsTr("Routines")
                                        
                                        ActionItem {
                                            title: qsTr("Rename routine")
                                            imageSource: "asset:///images/icon_write_context.png"
                                            onTriggered: {
                                                listItemContainer.ListItem.view.renameRoutine(ListItemData.id, ListItemData.title);
                                            }
                                        }
                                        
                                        DeleteActionItem {
                                            title: qsTr("Delete routine")
                                            onTriggered: {
                                                listItemContainer.ListItem.view.deleteRoutine(ListItemData.id);
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    ]
                    
                    function renameRoutine(id, label) {
                        routineController.renameRoutine(id, label);                        
                    }
                    
                    function deleteRoutine(id) {
                        routineController.deleteRoutine(id);
                    }
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        if(!pageRoutineDetail) {
                            pageRoutineDetail = routineDetail.createObject();
                        }
                        pageRoutineDetail.routine = chosenItem.id;
                        pageRoutineDetail.title = chosenItem.title;
                        nav.push(pageRoutineDetail);
                        
                    }
                
                }
                
            }
        }
        
        onCreationCompleted: {
            routineController.setRoutineList(listRoutines);
            routineController.updateRoutineList();
            routineController.need_push = true;
            navDepth = 0;
        }
        
        attachedObjects: [
            RoutineController {
                id: routineController
                property bool need_push
                
                onCompleted: {
                    listRoutines.requestFocus();
                }
                
                onLoadCardio: {
                    if(!practicePage)
                        practicePage = practice.createObject();

                    practicePage.category = 1;
                    practicePage.playlistMode = true;
                    practicePage.title = title;
                    practicePage.exercise_id = exercise_id;
                                        
                    if(need_push) {
                        need_push = false;
                        nav.push(practicePage);
                    }
                }
                
                onLoadStrength: {
                    if(!practicePage)
                        practicePage = practice.createObject();
                    
                    practicePage.category = 2;
                    practicePage.playlistMode = true;
                    practicePage.title = title;
                    practicePage.exercise_id = exercise_id;
                                        
                    if(need_push) {
                        need_push = false;
                        nav.push(practicePage);
                    }
                }
                
                onCloseCard: {
                    need_push = true;
                    nav.pop();
                }
            },
            ComponentDefinition {
                id: routineDetail
                source: "RoutineDetail.qml"
            },
            ComponentDefinition {
                id: practice
                source: "Practice.qml"
            }
        ]
    }
    
    onPopTransitionEnded: {

    }
    
    onPushTransitionEnded: {

    }
    
    
}