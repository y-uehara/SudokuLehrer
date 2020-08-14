import QtQuick 2.0

Item
{
    id: root
    property int selectedNumber : 0

    Repeater
    {
        model: 81
        SudokuNumber
        {
            column: index % 9
            row: Math.floor(index / 9)

            x: 10 + (column * width) + (Math.floor(column / 3) * 2)
            y: 10 + (row * height) + (Math.floor(row / 3) * 2)

            setNumber: root.selectedNumber
        }
    }
}
