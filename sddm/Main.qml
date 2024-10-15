import QtQuick 2.0
import SddmComponents 2.0

Rectangle {
    id: container
    width: 1920
    height: 1080

    TextConstants { id: textConstants }

    Background {
        anchors.fill: parent
        source: Qt.resolvedUrl(config.background)
        fillMode: Image.PreserveAspectCrop
    }

    // Main login form
    Rectangle {
        width: 250
        height: 250
        color: "transparent"
        anchors.centerIn: parent  // Center the form in the screen

        Column {
            width: parent.width
            anchors.centerIn: parent
            spacing: 10

            // Username input with simulated placeholder
            Rectangle {
                width: parent.width
                height: 50
                color: "black"
                radius: 10

                TextBox {
                    id: name
                    width: parent.width - 7
                    height: parent.height - 7
                    text: userModel.lastUser
                    font.pixelSize: 16
                    radius: 7

                    Keys.onPressed: {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                            password.focus = true;  // Move focus to the password field
                            event.accepted = true;
                        }
                    }

                    KeyNavigation.backtab: password; KeyNavigation.tab: password
                    anchors.horizontalCenter: parent.horizontalCenter  // Center the TextBox horizontally
                    anchors.verticalCenter: parent.verticalCenter
                }

                // Placeholder text for Username
                Text {
                    text: textConstants.userName
                    color: "gray"
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    visible: name.text.length === 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // Password input with simulated placeholder
            Rectangle {
                width: parent.width
                height: 50
                color: "black"
                radius: 10

                PasswordBox {
                    id: password
                    width: parent.width - 7
                    height: parent.height - 7
                    font.pixelSize: 16
                    radius: 7

                    Keys.onPressed: {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                            sddm.login(name.text, password.text, 0)  // Trigger login on Enter
                            event.accepted = true;
                        }
                    }

                    KeyNavigation.backtab: name; KeyNavigation.tab: name
                    anchors.horizontalCenter: parent.horizontalCenter  // Center the TextBox horizontally
                    anchors.verticalCenter: parent.verticalCenter
                }

                // Placeholder text for Password
                Text {
                    text: textConstants.password
                    color: "gray"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    visible: password.text.length === 0
                }
            }
        }
    }

    Component.onCompleted: {
        if (name.text == "")
            name.focus = true  // Start with focus on username
        else
            password.focus = true  // Start with focus on password if username is filled
    }
}
