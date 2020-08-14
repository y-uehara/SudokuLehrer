import QtQuick 2.0

Rectangle
{
    id: root

    property int setNumber: 0

    property int num: 0
    property int column: 0
    property int row: 0
    property bool legal: true

    property int imageWidth: 36
    property int imageHeight: 36

    color: "black"
    width: imageWidth + 2
    height: imageHeight + 2

    Rectangle
    {
        color: "white"
        width: root.imageWidth
        height: root.imageWidth
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Image
        {
            id: numberImage
            anchors.fill: parent

            width: root.imageWidth
            height: root.imageHeight

            source: root.legal ? "qrc:///image/number3_4" + root.num + ".png" : "qrc:///image/number3_" + root.num + ".png"
            opacity: root.num === 0 ? 0 : 1
        }
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            SudokuBoard.changeNumber(column, row, root.setNumber)
        }
    }


    //    SudokuBoard.onHoge: { root.num = SudokuBoard.getValue(root.column, root.row) }
    Connections
    {
        target: SudokuBoard
        onNumberChangeRequest:
        {
            if(x === column && y === row)
            {
                root.num = num
            }
        }
        onLegalChangeRequest:
        {
            if(x === column && y === row)
            {
                root.legal = legal
            }
        }
    }
}
