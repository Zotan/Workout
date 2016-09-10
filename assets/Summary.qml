import bb.cascades 1.3
import Utility.SummaryController 1.0

NavigationPane {
    id: nav
    
    property variant exerciseListPage
    property variant plotPage
    property int exercisePickedCategory
    property int exercisePickedId
    property string exerciseLabel
    
    property variant selectionColor: Color.create("#1ca9be")
    
    Page {
        id: summaryPage
        
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
                     text: qsTr("Simple workout")
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
                     defaultImageSource: "asset:///images/icon_graph_white.png" 
                     preferredHeight: ui.du(7)
                     preferredWidth: ui.du(7)
                     onClicked: {
                         if(!exerciseListPage)
                             exerciseListPage = exercisePicker.createObject();
                         exerciseListPage.doNotCloseOnSelected = true;
                         nav.push(exerciseListPage);
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
             
            ListView {
                id: listDatePicker

                horizontalAlignment: HorizontalAlignment.Fill
                preferredHeight: ui.du(11)
                
                layout: StackListLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                snapMode: SnapMode.LeadingEdge
                
                listItemComponents: [
                    ListItemComponent {
                        type: ""
                        
                        
                        
                        Container {
                            id: containerItem
                            preferredHeight: ui.du(9)
                            preferredWidth: containerItem.ListItem.view.isPassport() ? ui.du(9.4) : ui.du(9)
                            layout: DockLayout {}
                            
                            
                            Label {
                                text: ListItemData.weekday
                                textStyle.color: containerItem.ListItem.selected || containerItem.ListItem.active ? Color.create("#1ca9be") :  (Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? Color.Black : Color.White)
                                textStyle.fontSize: FontSize.XSmall
                                verticalAlignment: VerticalAlignment.Top
                                horizontalAlignment: HorizontalAlignment.Center
                                
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                minHeight: ui.du(5)
                                maxHeight: ui.du(5)
                                topPadding: ui.du(1.5)
                                
                                Label {
                                    textStyle.textAlign: TextAlign.Center
                                    text: ListItemData.number
                                    textStyle.color: Color.DarkGray
                                    textStyle.fontSize: FontSize.XSmall
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Center
                                    topMargin: ui.du(1)
                                }
                            }
                            
                            
                            Container {
                                preferredHeight: ui.du(1)
                                preferredWidth: ui.du(5)
                                background: containerItem.ListItem.selected || containerItem.ListItem.active ? Color.create("#1ca9be") : (Application.themeSupport.theme.colorTheme.style != VisualStyle.Dark ? Color.Black : Color.White)
                                verticalAlignment: VerticalAlignment.Bottom
                                horizontalAlignment: HorizontalAlignment.Center
                                visible: ListItemData.marker
                            }
                        }
                    
                    }
                    
                ]
                
                function isPassport() {
                    return DisplayInfo.width > 1000;
                }
                
                onTriggered: {
                    toggleSelection(selected());   
                    toggleSelection(indexPath) 
                    
                    var newPath = indexPath;
                    newPath[0] -= isPassport() ? 6 : 4;
                    if(newPath[0] < 0) newPath[0] = 0;
                    
                    scrollToItem(newPath)
                    
                    var chosenItem = dataModel.data(indexPath);
                    
                    summaryController.getInfos(chosenItem.id);
                }
            } 
             
            
            ScrollView {
                id: scrollView
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
        
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                horizontalAlignment: HorizontalAlignment.Fill
                
                // --------------------------------------------------------------
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    minHeight: ui.du(1)
                }
        
                
                // --------------------------------------------------------------
                Label {
                    text: qsTr("STATS")
                    textStyle.fontSize: FontSize.Large
                    margin.leftOffset: ui.du(2)
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
                Divider { }
                
                Label {
                    text: summaryController.stats
                    textStyle.color: Color.Gray
                    margin.leftOffset: ui.du(2)
                    multiline: true
                }
                
                
                
                // --------------------------------------------------------------
                
                Label {
                    text: qsTr("DETAILS")
                    textStyle.fontSize: FontSize.Large
                    margin.leftOffset: ui.du(2)
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
                Divider { }
                
                Container {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout { }
                    preferredHeight: ui.du(70)
                    
                    Container {  
                        id: dataEmptyLabel
                        visible: histModel.empty //model.isEmpty() will not work  
                        horizontalAlignment: HorizontalAlignment.Center  
                        verticalAlignment: VerticalAlignment.Center
                        
                        layout: DockLayout {}
                        
                        Label {
                            text: qsTr("No previous exercises available.")
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    
                    ListView {
                        id: detailsExercises
                        preferredHeight: ui.du(70)
                        
                        dataModel: GroupDataModel {
                            id: histModel    
                            grouping: ItemGrouping.ByFullValue
                            sortingKeys: ["title"]     
                            
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
                                type: "header"
                                Header {
                                    title: ListItemData
                                }
                            },
                            ListItemComponent {
                                type: "item"
                                
                                Container {
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
                                            text: ListItemData.text
                                            verticalAlignment: VerticalAlignment.Center
                                            horizontalAlignment: HorizontalAlignment.Left
                                        }
                                    }
                                    
                                    
                                    Divider { }
                                
                                }
                            }
                        ]
                    
                    }    
                }
                
            }
        }
        }
        
        onCreationCompleted: {
            summaryController.setListView(detailsExercises);
            summaryController.setListDatePicker(listDatePicker);
            summaryController.getInfos();
            scrollView.requestFocus();
        }
        
        
        attachedObjects: [
            SummaryController {
                id: summaryController
                
            },
            ComponentDefinition {
                id: exercisePicker
                source: "ExercisePicker.qml"
            }
        ]
        
    }
    
    onExercisePickedIdChanged: {
        
        if(exercisePickedId == -1) return;
        
        if(!plotPage)
            plotPage = plotProgress.createObject();

        plotPage.caption = exerciseLabel;
        plotPage.category = exercisePickedCategory;        
        plotPage.exercise_id = exercisePickedId;
        

        nav.push(plotPage);
        
        exercisePickedId = -1;
        
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: plotProgress
            source: "PlotProgress.qml"
        }
    ]
}