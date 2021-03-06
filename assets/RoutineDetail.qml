import bb.cascades 1.3
import bb.system 1.2

Page {
    property int routine
    property string title
    property variant exPickerPage
        
    property int exercisePickedId
    
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
                text: title
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
                    if(!exPickerPage) {
                        exPickerPage = exercisePicker.createObject();
                    }
                    nav.push(exPickerPage);
                }
            }
            
            Button {
                text: qsTr("OK")
                visible: routineDetailList.rearrangeHandler.active
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                preferredHeight: ui.du(7)
                preferredWidth: ui.du(7)
                
                onClicked: {
                    routineDetailList.rearrangeHandler.setActive(false);
                    routineController.saveOrder();
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
            
            id: container
            property int itemOrigin: -1
            property int lastItemDestination: -1
            
            function undoItemMove() {
                if (lastItemDestination != -1 && itemOrigin != -1) {
                    theModel.move(lastItemDestination, itemOrigin);
                    console.log("Item moved back from " + lastItemDestination + " to " + itemOrigin);
                    itemOrigin = -1;
                    lastItemDestination = -1;
                } else {
                    console.log("Nothing to undo!")
                }
            }
            
            Container {  
                id: dataEmptyLabel
                visible: theModel.empty //model.isEmpty() will not work  
                horizontalAlignment: HorizontalAlignment.Center  
                verticalAlignment: VerticalAlignment.Center
                
                layout: DockLayout {}
                
                Label {
                    text: qsTr("No exercises in this routine\nYou can add some using the \"add\" button.")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.textAlign: TextAlign.Center
                    multiline: true
                }
            }
            
            ListView {
                id: routineDetailList
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                
                dataModel: ArrayDataModel {
                    id: theModel
                    
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
                
                layout: StackListLayout {
                    id: theLayout
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
                                deleteActionItem.selectionList = routineDetailList.selectionList()
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
                                                var idReg = RegExp("([0-9]+)")
                                                var encoded_text = theModel.data(deleteActionItem.selectionList[i]);
                                                var ret = encoded_text.match(idReg);
                                                
                                                //console.log(parseInt(encoded_text.substring(0,ret[1].length)))
                                                routineController.removeExerciseFromRoutineNoAsk(routine, parseInt(encoded_text.substring(0,ret[1].length)));
                                                
                                            }
                                            routineController.loadRoutine(routine);
                                        }
                                    }
                                }
                            ]
                        }
                    ]
                    
                    status: qsTr("Delete")
                }
                
                rearrangeHandler {
                    
                    // Do not activate the handler here. It
                    // will be forcefully deactivated when 
                    // the data model is assigned.
                    onMoveStarted: {
                        console.log("onMoveStarted: " + event.startIndexPath);
                        container.itemOrigin = event.startIndexPath[0];
                    }
                    onMoveEnded: {
                        console.log("onMoveEnded: " + event.endIndexPath);
                    }
                    onMoveUpdated: {
                        // Always call denyMove() before initiating 
                        // the move. Without first denying the move
                        // the list item will only move 1 spot 
                        // regardless of how many spots it's
                        // dragged.
                        event.denyMove();
                        theModel.move(event.fromIndexPath[0], event.toIndexPath[0]);
                        console.log("onMoveUpdated: " + event.fromIndexPath[0] + " -> " + event.toIndexPath[0]);
                    }
                    onMoveAborted: {
                        console.log("onMoveAborted: " + event.endIndexPath);
                        container.undoItemMove();
                    }
                    onActiveChanged: {
                        console.log("active changed: " + active);
                    }
                }
                
                
                
                
                listItemComponents: [
                    ListItemComponent {
                        type: ""
                        
                        CustomListItem {
                            id: customListItem
                            
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
                                        text: customListItem.ListItem.view.decodeName(ListItemData)
                                        verticalAlignment: VerticalAlignment.Center
                                    }
                                }
                                
                            }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Routine")
                                    
                                    DeleteActionItem {
                                        title: qsTr("Remove exercise")
                                        onTriggered: {
                                            customListItem.ListItem.view.deleteExercise(ListItemData);
                                        }
                                    }
                                }
                            ]
                        }
                        
                    }
                    
                ]
                
                onTriggered: {
                    var encoded_text = dataModel.data(indexPath);
                    
                    var idReg = RegExp("([0-9]+)")
                    var ret = encoded_text.match(idReg);
                    
                    routineController.need_push = true;
                    routineController.playFromId(routine, parseInt(encoded_text.substring(0,ret[1].length)));                
                }
                
                function decodeName(encoded_text) {
                    var idReg = RegExp("([0-9]+:)")
                    var ret = encoded_text.match(idReg);
                    
                    return encoded_text.substring(ret[1].length);
                }
                
                function deleteExercise(encoded_text) {
                    var idReg = RegExp("([0-9]+)")
                    var ret = encoded_text.match(idReg);
                    
                    //console.log(parseInt(encoded_text.substring(0,ret[1].length)))
                    routineController.removeExerciseFromRoutine(routine, parseInt(encoded_text.substring(0,ret[1].length)));
                    
                }
            }
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Play")
            imageSource: "asset:///images/icon_play.png"
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
                routineController.play(routine);
            }
        },
        ActionItem {
            title: qsTr("Order")
            imageSource: "asset:///images/icon_filter.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                routineDetailList.rearrangeHandler.setActive(true);
            }
        }
    ]
    
    onCreationCompleted: {
        routineController.setRoutineDetailList(routineDetailList);
    }
    
    onRoutineChanged: {
        routineController.loadRoutine(routine);
        routineDetailList.requestFocus();
    }
    
    onExercisePickedIdChanged: {
        routineController.addExerciseToRoutine(routine, exercisePickedId);
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: exercisePicker
            source: "ExercisePicker.qml"
        }
    ]
}


