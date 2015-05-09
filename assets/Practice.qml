import bb.cascades 1.3
import bb.system 1.0
import Utility.PracticeController 1.0
import Utility.AppSettings 1.0

Page {
    id: practicePage
    property string title
    property int    exercise_id
    property bool   playlistMode
    property int    category
    
    property variant editPracticePage
    
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: DockLayout { }
                leftPadding: 10
                rightPadding: 10
                
                Label {
                    id: folder
                    text: practicePage.title
                    textStyle {
                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                    }
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }            
            }
        }
    }
    
    
    Container {
        layout: StackLayout { 
            orientation: LayoutOrientation.TopToBottom
        }
        
        SegmentedControl {
            horizontalAlignment: HorizontalAlignment.Fill
            id: actionSelector
            
            Option {
                id: option1
                text: qsTr("New")
                value: 0
            }
            Option {
                id: option2
                text: qsTr("History")
                value: 1
            }
            Option {
                id: option3
                text: qsTr("Graph")
                value: 2
            }
            
            onSelectedIndexChanged: {
                scrollView.scrollToPoint(DisplayInfo.width*selectedIndex, scrollView.viewableArea.y);
            }
        }
        
        
        ScrollView {
            id: scrollView
            scrollViewProperties.scrollMode: ScrollMode.Both
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            onViewableAreaChanged: {
                if(viewableArea.x % DisplayInfo.width != 0) {
                    scrollView.scrollToPoint(Math.floor(viewableArea.x/DisplayInfo.width+0.5)*DisplayInfo.width, scrollView.viewableArea.y);
                    actionSelector.selectedIndex = Math.floor(viewableArea.x/DisplayInfo.width+0.5);
                }
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                // ------------------------------------------------------------------------------------------------------------------------------------------------------
                // new entry
                
                Container {
                    minWidth: DisplayInfo.width
                    maxWidth: DisplayInfo.width
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    
                    Container {
                        layout: DockLayout { }
                        horizontalAlignment: HorizontalAlignment.Fill
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            ImageView {
                                imageSource: "asset:///images/calendar.png"
                                preferredHeight: ui.du(7)
                                preferredWidth: ui.du(7)
                                verticalAlignment: VerticalAlignment.Center
                            }
                            
                            Label {
                                text: practiceController.date
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                        
                        Container {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            
                            ImageView {
                                imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/clock_white.png" : "asset:///images/clock.png"
                                preferredHeight: ui.du(5)
                                preferredWidth: ui.du(5)
                                verticalAlignment: VerticalAlignment.Center
                            }
                            
                            Label {
                                text: practiceController.time
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                    }
                    
                    Container {
                        preferredHeight: ui.du(1)
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
                            inputMode: TextFieldInputMode.NumbersAndPunctuation
                            
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
                            inputMode: TextFieldInputMode.NumbersAndPunctuation
                            
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
                            inputMode: TextFieldInputMode.NumbersAndPunctuation
                            
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
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        
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
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        
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
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        
                        onTextChanged: {
                            practiceController.calories = parseInt(text, 10);
                        }
                    }
                    
                    Label {
                        id: labelStrength1
                        visible: category == 2
                        text: qsTr("Set: ") + practiceController.sets
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    Divider { 
                        id: dividerStrength1
                        visible: category == 2 
                    }
                    
                    Container {
                        id: inputStrength1
                        visible: category == 2
                        layout: GridLayout {
                            columnCount: 5
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        TextField {
                            id: repetitionInput
                            hintText: ""
                            text: practiceController.repetition
                            textStyle.fontSize: FontSize.Small
                            textStyle.textAlign: TextAlign.Center
                            preferredWidth: ui.du(10)
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            clearButtonVisible: false
                            inputMode: TextFieldInputMode.NumbersAndPunctuation
                            
                            onTextChanged: {
                                practiceController.repetition = parseInt(text,10);
                            }
                            
                            input {
                                submitKey: SubmitKey.Send
                                onSubmitted: {
                                    
                                    if(practiceController.repetition == 0) {
                                        errorNoRepetition.show();
                                    } else {
                                        practiceController.saveSet(notes.text);
                                        notes.text = "";
                                        practiceController.repetition = 0;
                                        repetitionInput.requestFocus();
                                    }
                                }
                            }
                        }
                        
                        Label {
                            text: "REPS @"
                            textStyle.textAlign: TextAlign.Center
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: ui.du(16)
                        }
                        
                        
                        TextField {
                            hintText: ""
                            text: practiceController.weight
                            textStyle.fontSize: FontSize.Small
                            textStyle.textAlign: TextAlign.Center
                            preferredWidth: ui.du(13)
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            clearButtonVisible: false
                            inputMode: TextFieldInputMode.NumbersAndPunctuation
                            
                            onTextChanged: {
                                practiceController.weight = parseFloat(text);
                            }
                            
                            input {
                                submitKey: SubmitKey.Send
                                onSubmitted: {
                                    
                                    if(practiceController.repetition == 0) {
                                        errorNoRepetition.show();
                                    } else {
                                        practiceController.saveSet(notes.text);
                                        notes.text = "";
                                        practiceController.repetition = 0;
                                        repetitionInput.requestFocus();
                                        
                                    }
                                }
                            }
                        }
                        
                        
                        Label {
                            text: appSettings.unit == 2 ? qsTr("Kg") : qsTr("lbs")
                            textStyle.textAlign: TextAlign.Center
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: ui.du(12)
                        }
                        
                        ImageButton {
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_add_white.png" : "asset:///images/icon_add.png"
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            
                            onClicked: {
                                if(practiceController.repetition == 0) {
                                    errorNoRepetition.show();
                                } else {
                                    practiceController.saveSet(notes.text);
                                    notes.text = "";
                                    practiceController.repetition = 0;
                                }
                            }
                        }
                        
                        attachedObjects: [
                            SystemToast {
                                id: errorNoRepetition
                                body: qsTr("You have to perform at least one repetition!")
                            }
                        ]
                        
                        ImageButton {
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            margin.bottomOffset: ui.du(3)
                            margin.topOffset: ui.du(3)
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_up_rounded_white.png" : "asset:///images/icon_up_rounded_black.png"
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                practiceController.repetition++;
                            }
                        }
                        
                        Container {
                        
                        }
                        
                        ImageButton {
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            margin.bottomOffset: ui.du(3)
                            margin.topOffset: ui.du(3)
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_up_rounded_white.png" : "asset:///images/icon_up_rounded_black.png"
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                practiceController.weight++;
                            }
                        
                        }
                        
                        Container {
                        
                        }
                        
                        Container {
                        
                        }
                        
                        ImageButton {
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_down_rounded_white.png" : "asset:///images/icon_down_rounded_black.png"
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                practiceController.repetition = practiceController.repetition-1 >= 0 ? practiceController.repetition-1 : 0;
                            }
                        }
                        
                        Container {
                        
                        }
                        
                        ImageButton {
                            preferredHeight: ui.du(6)
                            preferredWidth: ui.du(6)
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_down_rounded_white.png" : "asset:///images/icon_down_rounded_black.png"
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                practiceController.weight = practiceController.weight-1 >= 0 ? practiceController.weight-1 : 0;
                            }
                        
                        }
                    
                    }
                    
                    Label {
                        text: qsTr("Notes")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    Divider { }
                    
                    TextField {
                        id: notes
                        textStyle.fontSize: FontSize.Small
                        textStyle.textAlign: TextAlign.Center
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Fill
                        clearButtonVisible: false
                    }
                    
                    Label {
                        id: labelStrength2
                        text: qsTr("Completed")
                        visible: category == 2
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    
                    ListView {
                        id: accomplishedExerciseList
                        visible: category == 2
                        
                        dataModel: GroupDataModel {
                            id: theModel    
                            grouping: ItemGrouping.None                
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
                                        text: qsTr("SET: ") + ListItemData.id + "    " + ListItemData.repetition + " REPS @ " + ListItemData.weight + listItemContainer.ListItem.view.getUnitStr();
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Left
                                    }
                                    
                                    Label {
                                        text:  ListItemData.note
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Right
                                    }
                                    
                                    Divider { }
                                
                                }
                            }
                        ]
                        
                        function getUnitStr() {
                            return (appSettings.unit == 2 ? qsTr(" Kg") :  qsTr(" lbs"));
                        }
                    
                    }
                }
                
                // ------------------------------------------------------------------------------------------------------------------------------------------------------
                // List
                
                
                Container {
                    minWidth: DisplayInfo.width
                    maxWidth: DisplayInfo.width
                    preferredHeight: DisplayInfo.height
                    
                    layout: DockLayout {}
                    
                    Container {  
                        id: dataEmptyLabel
                        visible: histModel.empty //model.isEmpty() will not work  
                        horizontalAlignment: HorizontalAlignment.Center  
                        verticalAlignment: VerticalAlignment.Center
                        
                        layout: DockLayout {}
                        
                        Label {
                            text: qsTr("No history available. You need to do some excercise!")
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    
                    ListView {
                        id: historyList
                        
                        dataModel: GroupDataModel {
                            id: histModel    
                            grouping: ItemGrouping.ByFullValue
                            sortingKeys: ["time"]      
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
                                    title: ListItem.view.indexToCatName(parseInt(ListItemData, 10))
                                }
                            },
                            ListItemComponent {
                                type: "item"
                                
                                Container {
                                    id: mainContainerHistListView
                                    preferredHeight: ui.du(12)
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Center
                                    layout: DockLayout {
                                    }
                                    
                                    Label {
                                        visible: mainContainerHistListView.ListItem.view.getCategory() == 1
                                        text: qsTr(" ") + ListItemData.distance +  mainContainerHistListView.ListItem.view.getUnit() + qsTr(" in ") + ListItemData.duration + " min";
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Left
                                    }
                                    
                                    
                                    Label {
                                        visible: mainContainerHistListView.ListItem.view.getCategory() == 2
                                        text: qsTr("SET: ") + ListItemData.rep_id + "    " + ListItemData.repetition + " REPS @ " + ListItemData.weight + mainContainerHistListView.ListItem.view.getUnitStr();
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Left
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
                                                    mainContainerHistListView.ListItem.view.editEntry(ListItemData.id);
                                                }
                                            }
                                            
                                            DeleteActionItem {
                                                title: qsTr("Delete")
                                                onTriggered: {
                                                    mainContainerHistListView.ListItem.view.deleteEntry(ListItemData.id);
                                                }
                                            }
                                        }
                                    ]
                                
                                }
                            }
                        ]
                        
                        function editEntry(id) {
                            if(!editPracticePage)
                                editPracticePage = editPractice.createObject();
                            
                            editPracticePage.category = category;
                            editPracticePage.id = id;
                            nav.push(editPracticePage);
                            
                        }
                        
                        function deleteEntry(id) {
                            practiceController.deletePracticeEntry(id, category, exercise_id);
                        }
                        
                        function getUnit() {
                            return appSettings.unit == 2 ? qsTr("m") :  qsTr("ft");
                        }
                        
                        function getUnitStr() {
                            return (appSettings.unit == 2 ? qsTr(" Kg") :  qsTr(" lbs"));
                        }
                        
                        function getCategory() {
                            return category;
                        }
                        
                        function indexToCatName(time) {
                            return practiceController.getDateFromTime(time);
                        }
                    }
                }
                
                // ------------------------------------------------------------------------------------------------------------------------------------------------------
                // Graph
                
                Container {
                    minWidth: DisplayInfo.width
                    maxWidth: DisplayInfo.width
                    preferredHeight: DisplayInfo.height
                    
                    layout: StackLayout {}
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        DateTimePicker {
                            id: startDate
                            title: qsTr("Start")
                            
                            onValueChanged: {
                                if(category == 1)
                                    practiceController.plotCardio(exercise_id, value, endDate.value, criteriaCardio.selectedIndex);
                                else
                                    practiceController.plotStrength(exercise_id, value, endDate.value, criteriaStrength.selectedIndex);
                                     
                            }
                        }
                        
                        DateTimePicker {
                            id: endDate
                            title: qsTr("End")
                            
                            onValueChanged: {
                                if(category == 1)
                                    practiceController.plotCardio(exercise_id, startDate.value, value, criteriaCardio.selectedIndex);
                                else
                                    practiceController.plotStrength(exercise_id, startDate.value, value, criteriaStrength.selectedIndex);
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
                    
                    WebView {
                        id: historyWeb                        
                        settings.textAutosizingEnabled: false
                        settings.zoomToFitEnabled: false
                        settings.background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.White
                        
                        url: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "local:///assets/render_graph_black.html" : "local:///assets/render_graph.html"
                        
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
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
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes.text);
                            nav.pop();
                        }
                    } else {
                        practiceController.pushToDB(practicePage.exercise_id);
                        nav.pop();
                    }
                } else {
                    if(category == 1) {
                        if(practiceController.duration != 0)
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes.text);
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
                            practiceController.pushCardioToDB(practicePage.exercise_id, notes.text);
                    } else {
                        if(practiceController.sets != 0)
                            practiceController.pushToDB(practicePage.exercise_id);
                    }
                    
                    routineController.prev();
                }
            }
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
        criteriaCardio.visible = category == 1;
        
        labelStrength1.visible =  category == 2;
        dividerStrength1.visible= category == 2;
        inputStrength1.visible =  category == 2;
        labelStrength2.visible =  category == 2;
        accomplishedExerciseList.visible = category == 2;
        criteriaStrength.visible = category == 2;
    }
    
    onCreationCompleted: {
        practiceController.setListView(accomplishedExerciseList);
        practiceController.setHistoryListView(historyList);    
        practiceController.setHistoryWebView(historyWeb); 
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
        }
        scrollView.requestFocus();
        
        if(category == 1)
            practiceController.loadHistory(exercise_id);
        else 
            practiceController.loadStrengthHistory(exercise_id);
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
        }
    ]
}
