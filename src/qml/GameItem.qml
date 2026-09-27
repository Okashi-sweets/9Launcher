pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs
import FileIO 1.0
import GameLauncher 1.0
import "main.js" as Core

Button {
    property var item: {}
    property bool isPC98: false
    property bool isInstalled: false
    property var mainWindow: null
    id: button

    function localizedTitle() {
        if (LanguageManager.language === "ja_JP") {
            return button.item.jp_title || button.item.en_title || "";
        }
        return button.item.en_title || button.item.jp_title || "";
    }

    onClicked: {
        if (isInstalled) {
            mainWindow.openGameLaunchDialog(item, isPC98);
        } else {
            gameDialog.open();
        }
    }

    function refreshInstalled() {
        const path = StandardPaths.writableLocation(StandardPaths.AppDataLocation) + "/installed.json";

        if (!fileIO.exists(path)) {
            fileIO.write(path, '{ "installed": [] }');
        }

        const contents = fileIO.read(path)
        if (!contents) {
            throw new Error("Could not read contents!");
        }

        const items = JSON.parse(contents);

        isInstalled = items.installed.some((game) => game.game_id == button.item.game_id);
    }

    Component.onCompleted: refreshInstalled()

    Connections {
        target: button.mainWindow
        function onRefreshTriggerChanged() { button.refreshInstalled() }
    }

    FileIO {
        id: fileIO
    }

    GameLauncher {
        id: gameLauncher
    }

    Image {
        id: gameImage
        width: parent.width
        height: parent.height
        source: "/nineLauncher/game-images/" + parent.item.game_id + ".png"

        layer.enabled: true
        layer.effect: ShaderEffect {
            fragmentShader: "/shaders/image_mask.frag.qsb"
            property real radius: 5
            property vector2d size: Qt.vector2d(gameImage.width, gameImage.height)
            property real grayscale: button.isInstalled ? 0.0 : 1.0
        }
    }

    Rectangle {
        id: rect
        width: parent.width
        height: parent.height
        border.color: "white"
        border.width: 1
        radius: 5
        color: "#60000000"

        anchors.centerIn: parent
        Text {
            anchors.fill: parent
            anchors.margins: 5
            text: button.localizedTitle()
            color: "white"
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
            font.bold: true
        }

        states: [
            State {
                name: "hovered"
                when: mouse.hovered
                PropertyChanges {
                    rect.color: "#80000000"
                }
            }
        ]

        transitions: Transition {
            reversible: true
            ColorAnimation {
                property: "color"
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }

        HoverHandler {
            id: mouse
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            cursorShape: Qt.PointingHandCursor
        }
    }

    FileDialog {
        id: gameDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        nameFilters: button.isPC98 ? [qsTr("PC-98 Disk Image (*.hdi)")] : [qsTr("Executable (*.exe)")]
        onAccepted: {
            const file = gameDialog.currentFile;
            const targetItem = button.item;
            const path = StandardPaths.writableLocation(StandardPaths.AppDataLocation) + "/installed.json";

            if (!fileIO.exists(path)) {
                fileIO.write(path, '{ "installed": [] }');
            }

            const currentInstalled = JSON.parse(fileIO.read(path));

            targetItem.path = file;

            currentInstalled.installed.push(targetItem);

            gameImage.source = "/nineLauncher/game-images/" + button.item.game_id + ".png";
            button.isInstalled = true

            fileIO.write(path, JSON.stringify(currentInstalled));
        }
    }

}
