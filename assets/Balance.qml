import bb.cascades 1.3
import bb.system 1.2
import Utility.BalanceController 1.0
import Utility.AppSettings 1.0

NavigationPane {
    id: nav
    property variant plotPage
    
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
                    text: qsTr("Body Weight")
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
                        if(!plotPage)
                            plotPage = plotProgress.createObject();
                        plotPage.category = 3;
                        plotPage.caption = qsTr("Body weight")
                        nav.push(plotPage);
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
            
            
            ScrollView {
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                id: scrollView
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    Container {
                        preferredHeight: ui.du(1)
                        horizontalAlignment: HorizontalAlignment.Fill
                    }
                    
                    Label {
                        text: qsTr("New")
                        textStyle.fontSize: FontSize.Large
                        margin.leftOffset: ui.du(2)
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    Divider { }
                    
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        horizontalAlignment: HorizontalAlignment.Fill
                        
                        
                        TextField {
                            id: newEntry
                            hintText: qsTr("Body weight")
                            horizontalAlignment: HorizontalAlignment.Fill
                            input.keyLayout: KeyLayout.Alphanumeric
                        }
                        
                        ImageButton {
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_add_white.png" : "asset:///images/icon_add.png"
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            
                            onClicked: {
                                var weight = parseFloat(newEntry.text);
                                if(weight > 0) {
                                    balanceController.saveWeight(weight, notes.text);
                                    notes.text = "";
                                    newEntry.text= "";
                                }
                            }
                        }
                        
                        Container {
                            preferredWidth: ui.du(1)
                        }
                    }
                    
                    TextField {
                        id: notes
                        hintText: qsTr("Notes")
                    }
                                        
                    
                    Label {
                        text: qsTr("Previous records")
                        textStyle.fontSize: FontSize.Large
                        margin.leftOffset: ui.du(2)
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    Divider {
                        visible: histModel.empty
                    }
                    
                    
                    Container {
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        layout: DockLayout { }
                        
                        Container {  
                            id: dataEmptyLabel
                            visible: histModel.empty //model.isEmpty() will not work  
                            horizontalAlignment: HorizontalAlignment.Center  
                            verticalAlignment: VerticalAlignment.Center
                            
                            layout: DockLayout {}
                            
                            Label {
                                text: qsTr("No previous body weight records.")
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                        
                        ListView {
                            
                            id: history
                            preferredHeight: ui.du(70)
                            
                            dataModel: GroupDataModel {
                                id: histModel    
                                grouping: ItemGrouping.None
                                sortingKeys: ["id"]     
                                
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
                                            deleteActionItem.selectionList = history.selectionList()
                                            deleteActionItem.selectedItem = histModel.data(selectionList);
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
                                                            balanceController.deleteRecordNoAsk(histModel.data(deleteActionItem.selectionList[i]).id);
                                                        }
                                                        balanceController.getWeightsList();
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
                                    type: "header"
                                    Header {
                                        title: ListItemData
                                    }
                                },
                                ListItemComponent {
                                    type: "item"
                                    
                                    Container {
                                        id: overallContainer
                                        preferredHeight: ui.du(12)
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Center
                                        layout: DockLayout {
                                        }
                                        
                                        Container {
                                            layout: DockLayout {
                                            }
                                            verticalAlignment: VerticalAlignment.Center
                                            horizontalAlignment: HorizontalAlignment.Fill
                                                         
                                            
                                            Container {
                                                layout: StackLayout {
                                                    orientation: LayoutOrientation.LeftToRight
                                                }
                                                verticalAlignment: VerticalAlignment.Center
                                                horizontalAlignment: HorizontalAlignment.Right
                                                
                                                Container {
                                                    preferredWidth: ui.du(1)
                                                }
                                                
                                                Label {
                                                    text: ListItemData.time
                                                    verticalAlignment: VerticalAlignment.Center
                                                    horizontalAlignment: HorizontalAlignment.Right
                                                }
                                            }              
                                            
                                            Container {
                                                layout: StackLayout {
                                                    orientation: LayoutOrientation.LeftToRight
                                                }
                                                verticalAlignment: VerticalAlignment.Center
                                                horizontalAlignment: HorizontalAlignment.Left
                                                
                                                
                                                Label {
                                                    text: ListItemData.weight + overallContainer.ListItem.view.getUnit() + (ListItemData.note == "" ? "" :  " -- " + ListItemData.note)
                                                    verticalAlignment: VerticalAlignment.Center
                                                    horizontalAlignment: HorizontalAlignment.Right
                                                }
                                                
                                                Container {
                                                    preferredWidth: ui.du(1)
                                                }
                                            }        
                                            
                                        }
                                        
                                        
                                        Divider { }
                                        
                                        contextActions: [
                                            ActionSet {
                                                title: qsTr("Body weight")
                                                
                                                DeleteActionItem {
                                                    title: qsTr("Delete entry")
                                                    onTriggered: {
                                                        overallContainer.ListItem.view.deleteEntry(ListItemData.id);
                                                    }
                                                }
                                            }
                                        ]
                                    
                                    }
                                }
                            ]
                            
                            function getUnit() {
                                return appSettings.unit == 2 ? qsTr("Kg") : qsTr("lbs");
                            }
                            
                            function deleteEntry(id) {
                                balanceController.deleteRecord(id);
                            }
                        
                        }    
                    }
                    
                }
            }
        }
        
        onCreationCompleted: {
            balanceController.setListView(history);
            balanceController.getWeightsList();
            scrollView.requestFocus();
        }
        
        
        attachedObjects: [
            BalanceController {
                id: balanceController
                
                onCompleted: {
                    scrollView.requestFocus();
                }
            },
            AppSettings {
                id: appSettings
            },
            ComponentDefinition {
                id: plotProgress
                source: "PlotProgress.qml"                
            }
        ]
    }
}
