import bb.cascades 1.3
import Utility.PracticeController 1.0
import Utility.Graph 1.0

Page {
    
    property string caption
    property int category
    property int exercise_id
    
    onCaptionChanged: {
        folder.text = caption;
    }
    
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
                text: caption
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
        }
        
        
        
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                DateTimePicker {
                    id: startDate
                    title: qsTr("Start")
                    value: practiceController.initDate();
                    onValueChanged: {
                        switch(category) {
                            
                            case 1:
                                practiceController.plotCardio(exercise_id, value, endDate.value, criteriaCardio.selectedIndex);
                                break;
                            
                            case 2:
                                practiceController.plotStrength(exercise_id, value, endDate.value, criteriaStrength.selectedIndex);
                                break;
                            
                            case 3:
                                balanceController.plotBodyWeights(value, endDate.value);
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
                                practiceController.plotCardio(exercise_id, startDate.value, value, criteriaCardio.selectedIndex);
                                break;
                            
                            case 2:
                                practiceController.plotStrength(exercise_id, startDate.value, value, criteriaStrength.selectedIndex);
                                break;
                            
                            case 3:
                                balanceController.plotBodyWeights(startDate.value, value);
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
                    practiceController.plotCardio(exercise_id, startDate.value, endDate.value, selectedIndex);
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
                    practiceController.plotStrength(exercise_id, startDate.value, endDate.value, selectedIndex);
                }
            }
            
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
    }
    
    onExercise_idChanged: {
        switch(category) {
            
            case 1:
                practiceController.plotCardio(exercise_id, startDate.value, endDate.value, criteriaCardio.selectedIndex);
                break;
            
            case 2:
                practiceController.plotStrength(exercise_id, startDate.value, endDate.value, criteriaStrength.selectedIndex);
                break;
            
            case 3:
                balanceController.plotBodyWeights(startDate.value, endDate.value);
                break;
        }
    
    }
    
    onCreationCompleted: {
        graphController.setContainer(graphContainer);
        //graphController.draw();
        
        if(typeof balanceController === 'undefined'){
            practiceController.setGraph(graphController);
        } else {
            balanceController.setGraph(graphController);
            balanceController.plotBodyWeights(startDate.value, endDate.value);
        }
        
    }
    
    attachedObjects: [
        PracticeController {
            id: practiceController
        },
        Graph {
            id: graphController
            width: DisplayInfo.width-20
            height: DisplayInfo.height/2
        }
    ]
}
