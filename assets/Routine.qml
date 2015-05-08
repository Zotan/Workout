import bb.cascades 1.3
import Utility.RoutineController 1.0

NavigationPane {
    id: nav
    property variant pageRoutineDetail
    
    property variant cardioPage
    property variant strengthPage
    property variant practicePage
    property int navDepth

    
    Page {
        titleBar: TitleBar {
            kind: TitleBarKind.FreeForm
            kindProperties: FreeFormTitleBarKindProperties {
                Container {
                    layout: DockLayout { }
                    leftPadding: 10
                    rightPadding: 10
                    
                    Label {
                        id: folder
                        text: qsTr("List of routines")
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
                            routineController.createRoutine();
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
                        pageRoutineDetail = routineDetail.createObject(0);
                    }
                    pageRoutineDetail.routine = chosenItem.id;
                    pageRoutineDetail.title = chosenItem.title;
                    nav.push(pageRoutineDetail);
                    
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