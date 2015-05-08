import bb.cascades 1.3
import Utility.SummaryController 1.0

Page {
    
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
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            
            minHeight: ui.du(30)
            
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.fontSize: FontSize.Large
                    textStyle.textAlign: TextAlign.Center
                    text: qsTr("YOUR LAST WORKOUT")
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: summaryController.date
                    textStyle.color: Color.Gray
                    textStyle.textAlign: TextAlign.Center
                    multiline: true
                }
            }
            
            
            
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
    
    onCreationCompleted: {
        summaryController.setListView(detailsExercises);
        summaryController.getInfos();
        scrollView.requestFocus();
    }
    
    attachedObjects: [
        SummaryController {
            id: summaryController
            
        }
    ]
    
}
