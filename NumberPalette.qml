import QtQuick 2.0

Item
{
    id: root

    property int selectedNumber: 0

    signal select(int num)

    width: 9 * 36
    height: 36

    Repeater
    {
        model: 9
        NumberPaletteItem
        {
            num: index + 1
            x: index * width
            y: 0
            selectedNumber: root.selectedNumber
            onSelect: { root.selectedNumber = num }
        }
    }

    onSelectedNumberChanged: { root.select(selectedNumber) }
}

