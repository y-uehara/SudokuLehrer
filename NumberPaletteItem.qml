import QtQuick 2.0

Item
{
    id: root

    width: 36
    height: 36

    property int num: 0
    property int selectedNumber: 0

    signal select(int num)

    Image
    {
        id: fixedImage
        anchors.fill: parent
        source: (root.selectedNumber == num) ? "qrc:///image/number3_" + root.num + ".png" : "qrc:///image/number3_4" + root.num + ".png"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: { root.select(root.selectedNumber == num ? 0 : num) }
    }
}
