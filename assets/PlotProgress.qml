import bb.cascades 1.3
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
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
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
                            
                            leftPadding: ui.du(0.2)
                            topPadding: ui.du(0.2)
                            bottomPadding: ui.du(0.2)
                            
                            Label {
                                text: ListItemData.text
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Left
                            }
                            
                            Divider { }
                        
                        }
                    }
                ]
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
                value: progressController.initDate();
                onValueChanged: {
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
                
                onValueChanged: {
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
            
            options: [
                Option {
                    text: qsTr("Time")
                    value: 0
                },
                Option {
                    text: qsTr("Distance")
                    value: 1
                    selected: true
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
                progressController.plotCardio(exercise_id, startDate.value, endDate.value, selectedIndex);
            }
        }
        
        
        
        DropDown {
            id: criteriaStrength
            visible: category == 2
            title: qsTr("Criteria")
            
            options: [
                Option {
                    text: qsTr("Max weight")
                    value: 0
                    selected: true
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
                progressController.plotStrength(exercise_id, startDate.value, endDate.value, selectedIndex);
            }
        }

    }
    
    onExercise_idChanged: {
        switch(category) {
            
            case 1:
                progressController.loadCardioHistory(exercise_id);
                progressController.plotCardio(exercise_id, startDate.value, endDate.value, criteriaCardio.selectedIndex);
                break;
            
            case 2:
                progressController.loadStrengthHistory(exercise_id);
                progressController.plotStrength(exercise_id, startDate.value, endDate.value, criteriaStrength.selectedIndex);
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
