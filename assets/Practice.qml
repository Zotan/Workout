import bb.cascades 1.3
import bb.system 1.0
import Utility.PracticeController 1.0
import Utility.AppSettings 1.0
import Utility.Graph 1.0

Page {
    id: practicePage
    property string title
    property int    exercise_id
    property bool   playlistMode
    property int    category
    property string notes
    
    property variant editPracticePage
    property variant plotPage
    
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
            bottomPadding: 5
            topPadding: 2
            
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: ui.du(18)
            
            Label {
                id: folder
                text: practicePage.title
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
            
            Label {
                text: practiceController.date + "   " + practiceController.time
                textStyle.color: Color.White
                textStyle.fontSize: FontSize.XSmall
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Right
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Top
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                ImageButton {
                    defaultImageSource: "asset:///images/icon_graph_white.png" 
                    preferredHeight: ui.du(7)
                    preferredWidth: ui.du(7)
                    onClicked: {
                        if(!plotPage)
                            plotPage = plotProgress.createObject();
                        plotPage.category = category;
                        plotPage.caption = practicePage.title;
                        plotPage.exercise_id = practicePage.exercise_id;
                        nav.push(plotPage);
                    }
                }
                
                ImageButton {
                    defaultImageSource: "asset:///images/ic_notes.png" 
                    preferredHeight: ui.du(7)
                    preferredWidth: ui.du(7)
                
                    onClicked: {
                        newNote.open();
                    }
                }
            }
        }
    
    Container {
        layout: StackLayout { 
            orientation: LayoutOrientation.TopToBottom
        }
        
            
     Container {
         layout: StackLayout {
             orientation: LayoutOrientation.LeftToRight
         }
         
         // ------------------------------------------------------------------------------------------------------------------------------------------------------
         // new entry
         
         Container {
             id: interfaceContainer
             layout: StackLayout {
                 orientation: LayoutOrientation.TopToBottom
             }
             
             function isPassport() {
                 return DisplayInfo.width > 1000;
             }
             
             Container {
                 background: Color.Black
                 preferredHeight: ui.du(20)
                 leftPadding: ui.du(2)
                 rightPadding: ui.du(2)
                 bottomPadding: ui.du(5)
                 
                 id: inputStrength1
                 visible: category == 2
                 layout: GridLayout {
                     columnCount: 4
                 }
                 horizontalAlignment: HorizontalAlignment.Fill
                 verticalAlignment: VerticalAlignment.Top
                 
                 Label {
                     text: qsTr("reps")
                     textStyle.textAlign: TextAlign.Center
                     verticalAlignment: VerticalAlignment.Bottom
                     horizontalAlignment: HorizontalAlignment.Center
                     preferredWidth: interfaceContainer.isPassport() ? ui.du(30) : ui.du(20)
                     preferredHeight: ui.du(5)
                     textStyle.color: Color.White
                 }
                 
                 Container {
                     preferredHeight: interfaceContainer.isPassport() ? ui.du(3) : ui.du(2)
                     preferredWidth: ui.du(2)
                 }
                 
                 Label {
                     text: qsTr("weight")
                     textStyle.textAlign: TextAlign.Center
                     verticalAlignment: VerticalAlignment.Bottom
                     horizontalAlignment: HorizontalAlignment.Center
                     preferredWidth: interfaceContainer.isPassport() ? ui.du(30) : ui.du(20)
                     preferredHeight: ui.du(5)
                     textStyle.color: Color.White
                 }
                 
                 Container {
                     preferredWidth: ui.du(16)
                     preferredHeight: ui.du(5)
                 }
                 
                 
                 TextField {
                     id: repetitionInput
                     hintText: ""
                     text: practiceController.repetition
                     textStyle.fontSize: FontSize.Small
                     textStyle.textAlign: TextAlign.Center
                     preferredWidth: interfaceContainer.isPassport() ? ui.du(30) : ui.du(20)
                     verticalAlignment: VerticalAlignment.Top
                     horizontalAlignment: HorizontalAlignment.Center
                     clearButtonVisible: false
                     input.keyLayout: KeyLayout.Alphanumeric
                     
                     onTextChanged: {
                         practiceController.repetition = parseInt(text,10);
                     }
                     
                     input {
                         submitKey: SubmitKey.Send
                         onSubmitted: {
                             
                             if(practiceController.repetition == 0) {
                                 errorNoRepetition.show();
                             } else {
                                 practiceController.saveSet(notes);
                                 notes = "";
                                 repetitionInput.requestFocus();
                             }
                         }
                     }
                 }
                 
                 Label {
                     text: qsTr("X")
                     textStyle.textAlign: TextAlign.Center
                     verticalAlignment: VerticalAlignment.Center
                     horizontalAlignment: HorizontalAlignment.Center
                     preferredWidth: interfaceContainer.isPassport() ? ui.du(3) : ui.du(2)
                     textStyle.color: Color.White
                 }
                 
                 TextField {
                     hintText: ""
                     text: practiceController.weight
                     textStyle.fontSize: FontSize.Small
                     textStyle.textAlign: TextAlign.Center
                     preferredWidth: interfaceContainer.isPassport() ? ui.du(30) : ui.du(20)
                     verticalAlignment: VerticalAlignment.Top
                     horizontalAlignment: HorizontalAlignment.Center
                     clearButtonVisible: false
                     input.keyLayout: KeyLayout.Alphanumeric
                     
                     onTextChanged: {
                         practiceController.weight = parseFloat(text);
                     }
                     
                     input {
                         submitKey: SubmitKey.Send
                         onSubmitted: {
                             
                             if(practiceController.repetition == 0) {
                                 errorNoRepetition.show();
                             } else {
                                 practiceController.saveSet(notes);
                                 notes = "";
                                 repetitionInput.requestFocus();
                             
                             }
                         }
                     }
                 }
                 
                 ImageButton {
                     defaultImageSource: "asset:///images/icon_add_white.png"
                     preferredHeight: ui.du(8)
                     preferredWidth: ui.du(8)
                     verticalAlignment: VerticalAlignment.Top
                     horizontalAlignment: HorizontalAlignment.Center
                     
                     onClicked: {
                         if(practiceController.repetition == 0) {
                             errorNoRepetition.show();
                         } else {
                             practiceController.saveSet(notes);
                             notes = "";
                         }
                     }
                 }
                 
                 
                 attachedObjects: [
                     SystemToast {
                         id: errorNoRepetition
                         body: qsTr("You have to perform at least one repetition!")
                     }
                 ]
             }
             
             Container {
                 visible: category == 2
                 preferredHeight: ui.du(7)
                 horizontalAlignment: HorizontalAlignment.Fill
                 background: Color.LightGray
                 
                 layout: StackLayout {
                     orientation: LayoutOrientation.LeftToRight
                 }
                 
                 Container {
                     preferredWidth: ui.du(0.5)
                 }
                 
                 Label {
                     text: qsTr("Sets")
                     textStyle.color: Color.White
                     verticalAlignment: VerticalAlignment.Center
                 }
                 
                 ImageButton {
                     defaultImageSource: "asset:///images/ic_expand.png" 
                     preferredHeight: ui.du(7)
                     preferredWidth: ui.du(7)
                     verticalAlignment: VerticalAlignment.Center
                     onClicked: {
                         practiceController.loadPrevious(exercise_id);
                     }
                 }
                 
                 
             }
             
             
             ScrollView { Container {
                 
             
             Container {
                 preferredHeight: ui.du(1)
                 visible: category == 1
             }
             
             Label {
                 id: labelCardio1
                 visible: category == 1
                 text: qsTr("Duration")
                 textStyle.fontSize: FontSize.Large
                 horizontalAlignment: HorizontalAlignment.Left
                 verticalAlignment: VerticalAlignment.Bottom
             }
             Divider { 
                 id: dividerCardio1
                 visible: category == 1 
             }
             
             Container {
                 id: inputCardio1
                 visible: category == 1
                 layout: StackLayout {
                     orientation: LayoutOrientation.LeftToRight
                 }
                 horizontalAlignment: HorizontalAlignment.Center
                 verticalAlignment: VerticalAlignment.Center
                 
                 TextField {
                     id: hh
                     hintText: "hh"
                     textStyle.fontSize: FontSize.Small
                     textStyle.textAlign: TextAlign.Center
                     preferredWidth: ui.du(10)
                     verticalAlignment: VerticalAlignment.Center
                     clearButtonVisible: false
                     input.keyLayout: KeyLayout.Alphanumeric
                     
                     onTextChanged: {
                         var num = 0;
                         if(!isNaN(parseInt(hh.text, 10)))
                             num += parseInt(hh.text)*60*60;
                         if(!isNaN(parseInt(mm.text, 10)))
                             num += parseInt(mm.text, 10)*60;
                         if(!isNaN(parseInt(ss.text, 10)))
                             num += parseInt(ss.text, 10);    
                         
                         practiceController.duration = num;
                     }
                 }
                 
                 Label {
                     text: ":"
                     verticalAlignment: VerticalAlignment.Center
                 }
                 
                 TextField {
                     id: mm
                     hintText: "mm"
                     textStyle.fontSize: FontSize.Small
                     textStyle.textAlign: TextAlign.Center
                     preferredWidth: ui.du(10)
                     verticalAlignment: VerticalAlignment.Center
                     clearButtonVisible: false
                     input.keyLayout: KeyLayout.Alphanumeric
                     
                     onTextChanged: {
                         var num = 0;
                         if(!isNaN(parseInt(hh.text, 10)))
                             num += parseInt(hh.text)*60*60;
                         if(!isNaN(parseInt(mm.text, 10)))
                             num += parseInt(mm.text, 10)*60;
                         if(!isNaN(parseInt(ss.text, 10)))
                             num += parseInt(ss.text, 10);   
                         
                         practiceController.duration = num;
                     }
                 }
                 
                 Label {
                     text: ":"
                     verticalAlignment: VerticalAlignment.Center
                 }
                 
                 TextField {
                     id: ss
                     hintText: "ss"
                     textStyle.fontSize: FontSize.Small
                     textStyle.textAlign: TextAlign.Center
                     preferredWidth: ui.du(10)
                     verticalAlignment: VerticalAlignment.Center
                     clearButtonVisible: false
                     input.keyLayout: KeyLayout.Alphanumeric
                     
                     onTextChanged: {
                         var num = 0;
                         if(!isNaN(parseInt(hh.text, 10)))
                             num += parseInt(hh.text)*60*60;
                         if(!isNaN(parseInt(mm.text, 10)))
                             num += parseInt(mm.text, 10)*60;
                         if(!isNaN(parseInt(ss.text, 10)))
                             num += parseInt(ss.text, 10);   
                         
                         practiceController.duration = num;
                     }
                 }
             }
             
             Label {
                 id: labelCardio2
                 visible: category == 1
                 text: qsTr("Distance")
                 textStyle.fontSize: FontSize.Large
                 horizontalAlignment: HorizontalAlignment.Left
                 verticalAlignment: VerticalAlignment.Bottom
             }
             Divider { 
                 id: dividerCardio2
                 visible: category == 1 
             }
             
             TextField {
                 id: inputCardio2
                 visible: category == 1
                 textStyle.fontSize: FontSize.Small
                 textStyle.textAlign: TextAlign.Center
                 preferredWidth: ui.du(42)
                 verticalAlignment: VerticalAlignment.Center
                 horizontalAlignment: HorizontalAlignment.Center
                 clearButtonVisible: false
                 input.keyLayout: KeyLayout.Alphanumeric
                 
                 onTextChanged: {
                     practiceController.distance = parseInt(text, 10);
                 }
             }
             
             Label {
                 id: labelCardio3
                 visible: category == 1
                 text: qsTr("Heart rate")
                 textStyle.fontSize: FontSize.Large
                 horizontalAlignment: HorizontalAlignment.Left
                 verticalAlignment: VerticalAlignment.Bottom
             }
             Divider { 
                 id: dividerCardio3
                 visible: category == 1 
             }
             
             TextField {
                 id: inputCardio3
                 visible: category == 1
                 textStyle.fontSize: FontSize.Small
                 textStyle.textAlign: TextAlign.Center
                 preferredWidth: ui.du(42)
                 verticalAlignment: VerticalAlignment.Center
                 horizontalAlignment: HorizontalAlignment.Center
                 clearButtonVisible: false
                 input.keyLayout: KeyLayout.Alphanumeric
                 
                 onTextChanged: {
                     practiceController.heartRate = parseInt(text, 10);
                 }
             }
             
             Label {
                 id: labelCardio4
                 visible: category == 1
                 text: qsTr("Calories")
                 textStyle.fontSize: FontSize.Large
                 horizontalAlignment: HorizontalAlignment.Left
                 verticalAlignment: VerticalAlignment.Bottom
             }
             Divider { 
                 id: dividerCardio4
                 visible: category == 1 
             }
             
             TextField {
                 id: inputCardio4
                 visible: category == 1
                 textStyle.fontSize: FontSize.Small
                 textStyle.textAlign: TextAlign.Center
                 preferredWidth: ui.du(42)
                 verticalAlignment: VerticalAlignment.Center
                 horizontalAlignment: HorizontalAlignment.Center
                 clearButtonVisible: false
                 input.keyLayout: KeyLayout.Alphanumeric
                 
                 onTextChanged: {
                     practiceController.calories = parseInt(text, 10);
                 }
             }
             
             }} 
             
             ListView {
                 id: accomplishedExerciseList
                 visible: category == 2
                 
                 dataModel: GroupDataModel {
                     id: theModel    
                     grouping: ItemGrouping.None    
                     
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
                                 deleteActionItem.selectionList = accomplishedExerciseList.selectionList()
                                 deleteActionItem.selectedItem = theModel.data(selectionList);
                                 multiSelectDeleteDialog.show()
                             }
                             attachedObjects: [
                                 SystemDialog {
                                     id: multiSelectDeleteDialog
                                     title: qsTr("Delete records") + Retranslate.onLocaleOrLanguageChanged
                                     body: qsTr("Are you sure you want to delete these records?") + Retranslate.onLocaleOrLanguageChanged
                                     onFinished: {
                                         if (result == 3) {
                                         } else {
                                             
                                             for (var i = 0; i < deleteActionItem.selectionList.length; ++ i) {
                                                 practiceController.deletePractice(theModel.data(deleteActionItem.selectionList[i]).id, theModel.data(deleteActionItem.selectionList[i]).saved, category);
                                             }
                                             
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
                             
                             Label {
                                 text: qsTr("SET: ") + ListItemData.rep_id + "    " + ListItemData.repetition + qsTr(" REPS @ ") + ListItemData.weight + listItemContainer.ListItem.view.getUnitStr();
                                 verticalAlignment: VerticalAlignment.Center
                                 horizontalAlignment: HorizontalAlignment.Left
                                 textStyle.color: ListItemData.done ? Color.Black : Color.LightGray
                             }
                             
                             Label {
                                 text:  ListItemData.note
                                 verticalAlignment: VerticalAlignment.Center
                                 horizontalAlignment: HorizontalAlignment.Right
                             }
                             
                             Divider { }
                             
                             contextActions: [
                                 ActionSet {
                                     title: qsTr("Practice")
                                     
                                     ActionItem {
                                         title: qsTr("Edit")
                                         imageSource: "asset:///images/icon_write_context.png"
                                         onTriggered: {
                                             listItemContainer.ListItem.view.editEntry(ListItemData.id, ListItemData.saved, ListItemData.weight, ListItemData.repetition);
                                         }
                                     }
                                     
                                     DeleteActionItem {
                                         title: qsTr("Delete")
                                         onTriggered: {
                                             listItemContainer.ListItem.view.deleteEntry(ListItemData.id, ListItemData.saved);
                                         }
                                     }
                                 }
                             ]
                                                                 
                         }
                     }
                 ]
                                         
                 function getUnitStr() {
                     return (appSettings.unit == 2 ? qsTr(" Kg") :  qsTr(" lbs"));
                 }
                 
                 function editEntry(id, isSaved, weight, repetition) {
                     if(!editPracticePage)
                         editPracticePage = editPractice.createObject();
                     
                     editPracticePage.category = category;
                     editPracticePage.id = id;
                     editPracticePage.saved = isSaved;
                     editPracticePage.repetition = repetition;
                     editPracticePage.weight = weight;
                     nav.push(editPracticePage);
                 
                 }
                 
                 function deleteEntry(id, isSaved) {
                     practiceController.deletePracticeEntry(id, isSaved, category, exercise_id);
                 }
             
             }
             }
        }
        
    
        Container {
            id: watch
            visible: category == 2
            layout: GridLayout {
                columnCount: 7
            }
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: ui.du(10)
            
            Container {
                maxWidth: ui.du(0.1)
            }
            
            ImageButton {
                defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_rounded_refresh_white.png" : "asset:///images/icon_rounded_refresh_black.png"
                preferredWidth: ui.du(6)
                preferredHeight: ui.du(6)
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    practiceController.resetStopWatch();
                }
            }
            
            ImageButton {
                defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_rounded_minus_white.png" : "asset:///images/icon_rounded_minus_black.png"
                preferredWidth: ui.du(6)
                preferredHeight: ui.du(6)
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    practiceController.addSecStopWatch(-1);
                }
            }
        
            Label {
                text: practiceController.stopWatch
                textStyle.textAlign: TextAlign.Center
                minWidth: ui.du(5)
                horizontalAlignment: HorizontalAlignment.Center
                margin.topOffset: ui.du(0.8)
            }
            
            ImageButton {
                defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_rounded_plus_white.png" : "asset:///images/icon_rounded_plus_black.png"
                preferredWidth: ui.du(6)
                preferredHeight: ui.du(6)
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    practiceController.addSecStopWatch(1);
                }
            }
            
            ImageButton {
                defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_right_rounded_next.png" : "asset:///images/icon_right_rounded_black.png"
                preferredWidth: ui.du(6)
                preferredHeight: ui.du(6)
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    practiceController.startStopWatch();
                }
            }
            
            Container {
                maxWidth: ui.du(0.1)
            }
        }
    }
    }
    
    actions: [
        
        ActionItem {
            title: playlistMode ? qsTr("Next") : qsTr("Save")
            ActionBar.placement: ActionBarPlacement.Signature 
            imageSource: playlistMode ? "asset:///images/icon_next.png" : "asset:///images/icon_save.png"
            
            onTriggered: {
                if(!playlistMode) {
                    if(category == 1) {
                        if(practiceController.duration == 0) {
                            errorNoDuration.show();
                        } else {
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes);
                            nav.pop();
                        }
                    } else {
                        practiceController.pushToDB(practicePage.exercise_id);
                        nav.pop();
                    }
                } else {
                    if(category == 1) {
                        if(practiceController.duration != 0)
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes);
                    } else {
                        if(practiceController.sets != 0)
                            practiceController.pushToDB(practicePage.exercise_id);
                    }
                    routineController.next();
                }
            }
            
            attachedObjects: [
                SystemToast {
                    id: errorNoDuration
                    body: qsTr("You have to enter a duration!")
                }
            ]
        },
        
        
        ActionItem {
            title: playlistMode ? qsTr("Prev") : qsTr("Close")
            ActionBar.placement: ActionBarPlacement.OnBar 
            imageSource: playlistMode ? "asset:///images/icon_prev.png" : "asset:///images/icon_close.png"
            
            onTriggered: {
                if(!playlistMode) {
                    nav.pop();
                } else {
                    if(category == 1) {
                        if(practiceController.duration != 0)
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes);
                    } else {
                        if(practiceController.sets != 0)
                            practiceController.pushToDB(practicePage.exercise_id);
                    }
                    
                    routineController.prev();
                }
            }
        },
        ActionItem {
          title: qsTr("Timer")
          ActionBar.placement: ActionBarPlacement.InOverflow
          imageSource:   "asset:///images/icon_stop_watch.png"
          onTriggered: {
              timerpicker.value = timerpicker.dateFromTime(practiceController.getSecStopWatch());
              setTimer.open();
          }
          
          attachedObjects: [
              Dialog {
                  id: setTimer
                                    
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
                          text: qsTr("Choose timer duration")
                          textStyle.fontSize: FontSize.Large
                          horizontalAlignment: HorizontalAlignment.Left
                          textStyle.color: Color.White
                      }
                      
                      
                      DateTimePicker {
                          id: timerpicker
                          title: "Timer"
                          mode: DateTimePickerMode.Timer
                          horizontalAlignment: HorizontalAlignment.Fill
                          value: timerpicker.dateFromTime(practiceController.getSecStopWatch())
                      }
                      
                      Divider { }
                      
                      Container {
                          layout: StackLayout {
                              orientation: LayoutOrientation.LeftToRight
                          }
                          horizontalAlignment: HorizontalAlignment.Fill
                          
                          Button {
                              text: qsTr("Cancel")
                              horizontalAlignment: HorizontalAlignment.Fill
                              
                              onClicked: {
                                  setTimer.close();
                              }
                          }
                          
                          Button {
                              text: qsTr("Save")
                              horizontalAlignment: HorizontalAlignment.Fill
                              
                              onClicked: {
                                  setTimer.close();
                                  practiceController.setSecStopWatch(timerpicker.value)
                              }
                          }
                      }
                  }
              }
          ]
        }
    ]
    
    onCategoryChanged: {
        labelCardio1.visible = category == 1;
        dividerCardio1.visible=category == 1;
        inputCardio1.visible = category == 1;
        labelCardio2.visible = category == 1;
        dividerCardio2.visible=category == 1;
        inputCardio2.visible = category == 1;
        labelCardio3.visible = category == 1;
        dividerCardio3.visible=category == 1;
        inputCardio3.visible = category == 1;
        labelCardio4.visible = category == 1;
        dividerCardio4.visible=category == 1;
        inputCardio4.visible = category == 1;
        
        inputStrength1.visible =  category == 2;
        accomplishedExerciseList.visible = category == 2;
    }
    
    onCreationCompleted: {
        practiceController.setListView(accomplishedExerciseList);
        playlistMode = false;   
    }
    
    onExercise_idChanged: {
        if(playlistMode) {
            if(category == 1) {
                // reset cardio
                hh.text = "";
                mm.text = "";
                ss.text = "";
                inputCardio2.text = "";
                inputCardio3.text = "";
                inputCardio4.text = "";
            } else {
                practiceController.restoreSession(exercise_id);
            }
        } else {
            practiceController.resetStrengthView();
            practiceController.repetition = 0;
            practiceController.weight = 0;
            practiceController.sets = 1;
        }    
    }
    
    attachedObjects: [
        PracticeController {
            id: practiceController
        },
        AppSettings {
            id: appSettings
        },
        ComponentDefinition {
            id: editPractice
            source: "EditPractice.qml"
        },
        ComponentDefinition {
            id: plotProgress
            source: "PlotProgress.qml"                
        },
        Dialog {
            id: newNote
            
            onClosed: {
                label.text = "";
            }
            
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
                    text: qsTr("Create a new note")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    textStyle.color: Color.White
                }
                
                TextField {
                    id: label
                    hintText: qsTr("Note attached to the next set")   
                    text: notes 
                    backgroundVisible: true
                }
                
                Divider { }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Button {
                        text: qsTr("Cancel")
                        horizontalAlignment: HorizontalAlignment.Fill
                        onClicked: {
                            newNote.close();
                        }
                    }
                    
                    Button {
                        text: qsTr("Add to next set")
                        horizontalAlignment: HorizontalAlignment.Fill
                        onClicked: {
                            notes = label.text;
                            newNote.close();
                        }
                    }
                }
                
                Divider { }
            
            }
        }
    ]
}
