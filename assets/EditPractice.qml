import bb.cascades 1.3
import Utility.PracticeController 1.0

Page {
    property int category
    property int id
    
    titleBar: TitleBar {
        title: qsTr("Edit")
        acceptAction: [
            ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    
                    var num = 0;
                    if(!isNaN(parseInt(hh.text, 10)))
                        num += parseInt(hh.text)*60*60;
                    if(!isNaN(parseInt(mm.text, 10)))
                        num += parseInt(mm.text, 10)*60;
                    if(!isNaN(parseInt(ss.text, 10)))
                        num += parseInt(ss.text, 10);    
                        
                    practiceController.duration = num;
                    practiceController.distance = parseInt(inputCardio2.text,10);
                    practiceController.heartRate = parseInt(inputCardio3.text, 10);
                    practiceController.calories = parseInt(inputCardio4.text, 10);
                    practiceController.notes = inputNotes.text;
                    
                    practiceController.repetition = parseInt(inputStrength1.text, 10);
                    practiceController.weight = parseFloat(inputStrength2.text);
                    
                    practiceController.updatePractice(category);
                    
                    refresh_history = true;
                    
                    nav.pop();                
                }
            }
        ]
        
        dismissAction: [
            ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    nav.pop();
                }
            }
        ]
    }
    
    ScrollView {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            Container {
                preferredHeight: ui.du(1)
            }
            horizontalAlignment: HorizontalAlignment.Fill
            
            Container {
                id: cardioContainer
                visible: category == 1
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                horizontalAlignment: HorizontalAlignment.Fill
                
                
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
                        text: Math.floor(practiceController.duration / 3600)
                        textStyle.fontSize: FontSize.Small
                        textStyle.textAlign: TextAlign.Center
                        preferredWidth: ui.du(10)
                        verticalAlignment: VerticalAlignment.Center
                        clearButtonVisible: false
                        input.keyLayout: KeyLayout.Alphanumeric
                        
                    }
                    
                    Label {
                        text: ":"
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    TextField {
                        id: mm
                        hintText: "mm"
                        text: Math.floor((practiceController.duration - Math.floor(practiceController.duration / 3600)) / 60)
                        textStyle.fontSize: FontSize.Small
                        textStyle.textAlign: TextAlign.Center
                        preferredWidth: ui.du(10)
                        verticalAlignment: VerticalAlignment.Center
                        clearButtonVisible: false
                        input.keyLayout: KeyLayout.Alphanumeric
                        
                    }
                    
                    Label {
                        text: ":"
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    TextField {
                        id: ss
                        hintText: "ss"
                        text: practiceController.duration % 60
                        textStyle.fontSize: FontSize.Small
                        textStyle.textAlign: TextAlign.Center
                        preferredWidth: ui.du(10)
                        verticalAlignment: VerticalAlignment.Center
                        clearButtonVisible: false
                        input.keyLayout: KeyLayout.Alphanumeric
                        
                       
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
                    text: practiceController.distance
                    textStyle.fontSize: FontSize.Small
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: ui.du(42)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    clearButtonVisible: false
                    input.keyLayout: KeyLayout.Alphanumeric
                    
                    onTextChanged: {
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
                    text: practiceController.heartRate
                    textStyle.fontSize: FontSize.Small
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: ui.du(42)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    clearButtonVisible: false
                    input.keyLayout: KeyLayout.Alphanumeric
                    
                    onTextChanged: {
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
                    text: practiceController.calories
                    textStyle.fontSize: FontSize.Small
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: ui.du(42)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    clearButtonVisible: false
                    input.keyLayout: KeyLayout.Alphanumeric
                    
                    onTextChanged: {
                    }
                }
        
            }
            
            Container {
                id: strengthContainer
                visible: category == 2
                horizontalAlignment: HorizontalAlignment.Fill
                
                Label {
                    id: labelStrength1
                    visible: category == 2
                    text: qsTr("REPS")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
                Divider { 
                    id: dividerStrength1
                    visible: category == 2
                }
                
                TextField {
                    id: inputStrength1
                    visible: category == 2
                    text: practiceController.repetition
                    textStyle.fontSize: FontSize.Small
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: ui.du(42)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    clearButtonVisible: false
                    input.keyLayout: KeyLayout.Alphanumeric
                    
                    onTextChanged: {
                    }
                }
                
                Label {
                    id: labelStrength2
                    visible: category == 2
                    text: qsTr("Weight")
                    textStyle.fontSize: FontSize.Large
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
                Divider { 
                    id: dividerStrength2
                    visible: category == 2
                }
                
                TextField {
                    id: inputStrength2
                    visible: category == 2
                    text: practiceController.weight
                    textStyle.fontSize: FontSize.Small
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: ui.du(42)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    clearButtonVisible: false
                    input.keyLayout: KeyLayout.Alphanumeric
                    
                    onTextChanged: {
                    }
                }
            }
            
            
            Label {
                text: qsTr("Notes")
                textStyle.fontSize: FontSize.Large
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom
            }
            Divider { 
            }
            
            TextField {
                id: inputNotes
                text: practiceController.notes
                textStyle.fontSize: FontSize.Small
                textStyle.textAlign: TextAlign.Center
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Fill
                clearButtonVisible: false
                input.keyLayout: KeyLayout.Alphanumeric
                
                onTextChanged: {
                    practiceController.calories = parseInt(text, 10);
                }
            }
        
        }
        
    }
    
    
    onIdChanged: {
        practiceController.loadPractice(id, category);
    }
    
    attachedObjects: [
        PracticeController {
            id: practiceController
        }
    ]

}
