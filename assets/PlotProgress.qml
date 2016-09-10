import bb.cascades 1.3
import bb.system 1.0
import Utility.ProgressController 1.0
import Utility.Graph 1.0

Page {
    
    property string caption
    property int category
    property int exercise_id
    
    onCaptionChanged: {
        folder.text = caption;
    }
    
        
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            
            Container {
                layout: DockLayout { }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                
                SegmentedControl {
                    id: segment
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    options: [
                        Option {
                            value: 1
                            text: qsTr("List")
                            selected: true
                        
                        },
                        Option {
                            value: 2
                            text: qsTr("Graph")
                        }
                    ]
                    
                    onSelectedValueChanged: {
                        selectRenderer(selectedValue);
                    }
                }
            }
        }
    }
    
    function selectRenderer(option) {
        if(option == 1) {
            datePickers.visible = false;
            criteriaCardio.visible =  false;
            criteriaStrength.visible = false;
            graphUtility.visible = false;
            listUtility.visible = true;
        } else {
            datePickers.visible = true;
            criteriaCardio.visible = category == 1;
            criteriaStrength.visible = category == 2;
            graphUtility.visible = true;
            listUtility.visible = false;
        }
    }
        
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        Container {
            layout: DockLayout { }
            id: graphUtility
            
            ScrollView {
                
                Container {
                    id: graphContainer
                    layout: AbsoluteLayout { }
                    preferredHeight: DisplayInfo.height/2
                    preferredWidth: DisplayInfo.width-20
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    ImageView {
                        id: image
                        image: graphController.image
                        
                        preferredHeight: DisplayInfo.height/2
                        preferredWidth: DisplayInfo.width-20
                        scalingMethod: ScalingMethod.AspectFit
                    }
                }
            }
            
            Label {
                text: qsTr("Nothing to plot yet. You should have at least worked out on two different days to plot a progresion!")
                multiline: true
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                visible: graphController.empty
            }
        }
        
        Container {
            id: listUtility
            layout: DockLayout { }
            
            ListView {
                id: recordsList
                
                
                dataModel: GroupDataModel {
                    id: histModel    
                    grouping: ItemGrouping.ByFullValue
                    sortingKeys: ["title"]  
                    sortedAscending: false   
                    
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
                                deleteActionItem.selectionList = recordsList.selectionList()
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
                                                progressController.deleteRecord(histModel.data(deleteActionItem.selectionList[i]).id, category);
                                            }
                                            if(category == 2)
                                                progressController.updateRepIds();
                                            
                                            switch(category) {
                                                case 1:
                                                    progressController.loadCardioHistory(exercise_id);
                                                    break;
                                                
                                                case 2:
                                                    progressController.loadStrengthHistory(exercise_id);
                                                    break;
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
                        type: "header"
                        Header {
                            title: ListItemData
                        }
                    },
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            id: listItemContainer
                            preferredHeight: ui.du(12)
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: DockLayout {
                            }
                            
                            leftPadding: ui.du(0.2)
                            topPadding: ui.du(0.2)
                            bottomPadding: ui.du(0.2)
                            
                            Label {
                                text: ListItemData.text
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Left
                            }
                            
                            Divider { }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Previous records")
                                                                        
                                    DeleteActionItem {
                                        title: qsTr("Delete")
                                        onTriggered: {
                                            listItemContainer.ListItem.view.deleteEntry(ListItemData.id);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                ]
                
                function deleteEntry(id) {
                    progressController.deleteEntry(id, category, exercise_id);
                }
            }
            
            
            Label {
                text: qsTr("No previous records available")
                textStyle.textAlign: TextAlign.Center
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                
                visible: histModel.empty
                
            }
        }
        
        
        Container {
            id: datePickers
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            DateTimePicker {
                id: startDate
                title: qsTr("Start")
                value: progressController.startTime
                onValueChanged: {
                    progressController.startTime = value;
                    
                    switch(category) {
                        
                        case 1:
                            progressController.plotCardio(exercise_id, value, endDate.value, criteriaCardio.selectedIndex);
                            break;
                        
                        case 2:
                            progressController.plotStrength(exercise_id, value, endDate.value, criteriaStrength.selectedIndex);
                            break;
                        
                        case 3:
                            progressController.plotBodyWeights(value, endDate.value);
                            break;
                    }
                }
            }
            
            DateTimePicker {
                id: endDate
                title: qsTr("End")
                value: progressController.endTime
                
                onValueChanged: {
                    progressController.endTime = value;
                    
                    switch(category) {
                        
                        case 1:
                            progressController.plotCardio(exercise_id, startDate.value, value, criteriaCardio.selectedIndex);
                            break;
                        
                        case 2:
                            progressController.plotStrength(exercise_id, startDate.value, value, criteriaStrength.selectedIndex);
                            break;
                        
                        case 3:
                            progressController.plotBodyWeights(startDate.value, value);
                            break;
                    }
                }
            }
        }
                
        
        DropDown {
            id: criteriaCardio
            visible: category == 1
            title: qsTr("Criteria")
            selectedIndex: progressController.criteria
            
            options: [
                Option {
                    text: qsTr("Time")
                    value: 0
                },
                Option {
                    text: qsTr("Distance")
                    value: 1
                },
                Option {
                    text: qsTr("Heart rate")
                    value: 2
                },
                Option {
                    text: qsTr("Calories")
                    value: 3
                }
            ]
            
            onSelectedIndexChanged: {
                progressController.criteria = selectedIndex;
                progressController.plotCardio(exercise_id, startDate.value, endDate.value, selectedIndex);
            }
        }
        
        
        
        DropDown {
            id: criteriaStrength
            visible: category == 2
            title: qsTr("Criteria")
            selectedIndex: progressController.criteria
            
            options: [
                Option {
                    text: qsTr("Max weight")
                    value: 0
                },
                Option {
                    text: qsTr("Min weight")
                    value: 1                                
                },
                Option {
                    text: qsTr("Avg. weight")
                    value: 2
                },
                Option {
                    text: qsTr("Total weight")
                    value: 3
                },
                Option {
                    text: qsTr("Max reps")
                    value: 4
                },
                Option {
                    text: qsTr("Min reps")
                    value: 5
                },
                Option {
                    text: qsTr("Avg. reps")
                    value: 6
                },
                Option {
                    text: qsTr("Total reps")
                    value: 7
                }
            ]
            
            onSelectedIndexChanged: {
                progressController.criteria = selectedIndex;
                progressController.plotStrength(exercise_id, startDate.value, endDate.value, selectedIndex);
            }
        }

    }
    
    actions: [
        ActionItem {
            id: refresh
            title: qsTr("Refresh")
            imageSource: "asset:///images/ic_reload.png"
            onTriggered: {
                switch(category) {
                    case 1:
                        progressController.loadCardioHistory(exercise_id);
                        break;
                    
                    case 2:
                        progressController.loadStrengthHistory(exercise_id);
                        break;
                    
                    case 3:
                        progressController.plotBodyWeights(startDate.value, endDate.value);
                        break;
                }
            }
        }
    ]
    
    onExercise_idChanged: {
        switch(category) {
            
            case 1:
                progressController.loadCardioHistory(exercise_id);
                break;
            
            case 2:
                progressController.loadStrengthHistory(exercise_id);
                break;
            
            case 3:
                progressController.plotBodyWeights(startDate.value, endDate.value);
                break;
        }
        
    }
    
    onCreationCompleted: {
        selectRenderer(1);
        graphController.setContainer(graphContainer);
        progressController.setGraph(graphController);
        progressController.setListView(recordsList);
        
        if(typeof balanceController !== 'undefined'){
            progressController.getWeightsList();
            progressController.plotBodyWeights(startDate.value, endDate.value);
        }
        
    }
    
    onCategoryChanged: {
        if(segment.selectedValue == 2) {
            criteriaCardio.visible = category == 1;
            criteriaStrength.visible = category == 2;
        }
    }
    
    attachedObjects: [
        ProgressController {
            id: progressController
        },
        Graph {
            id: graphController
            width: DisplayInfo.width-20
            height: DisplayInfo.height/2
        }
    ]
}
