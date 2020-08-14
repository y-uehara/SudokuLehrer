import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

Window
{
    visible: true
    width: 640
    height: 480

    Rectangle {
        id: root
        anchors.fill: parent

        states: [
            State
            {
                name: "illegal"
                PropertyChanges { target: statusMessage; text: "Illegal placement" }
                PropertyChanges { target: solveButton; enabled: false }
                PropertyChanges { target: nextButton; enabled: false }
            },
            State
            {
                name: "solved"
                PropertyChanges { target: statusMessage; text: "Solved" }
                PropertyChanges { target: solveButton; enabled: false }
                PropertyChanges { target: nextButton; enabled: false }
            }
        ]

        Connections
        {
            target: SudokuBoard

            onStateIllegal: { root.state = "illegal" }
            onStateSolved: { root.state = "solved" }
            onStateRegular: { root.state = "" }
        }




        GameBoard
        {
            id: board
            selectedNumber: 0

            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: 2
        }

        NumberPalette
        {
            anchors.left: parent.left
            anchors.bottom: progressBar.top

            onSelect: { board.selectedNumber = num }
        }

        Button
        {
            id: solveButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            text: "solve"
            enabled: true
        }


        Button
        {
            id: nextButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: solveButton.bottom
            anchors.topMargin: 10

            text: "next"
            enabled: true
            onClicked: { SudokuBoard.calcNextHand() }
        }

        Button
        {
            id: clearButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: nextButton.bottom
            anchors.topMargin: 10

            text: "clear"
            enabled: true
            onClicked: { SudokuBoard.clearBoard() }
        }

        Text
        {
            id: statusMessage
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20

            text: "please place number"
        }


        ProgressBar
        {
            id: progressBar
            width: parent.width
            anchors.bottom: parent.bottom
            value: 0
        }

    }
}
