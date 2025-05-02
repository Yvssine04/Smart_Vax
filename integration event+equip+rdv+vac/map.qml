import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    Plugin {
        id: mapPlugin
        name: "osm"  // Utilisation d'OpenStreetMap
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(36.8, 10.2) // Centré sur la Tunisie
        zoomLevel: 7

        Repeater {
            model: eventsModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(modelData.latitude, modelData.longitude)
                anchorPoint.x: icon.width / 2
                anchorPoint.y: icon.height
                sourceItem: Item {
                    width: 24
                    height: 24

                    Image {
                        id: icon
                        source: "qrc:/pin.png"
                        width: 24
                        height: 24
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                eventHandler.updateEventList(modelData.ville);
                            }
                        }
                    }

                    Text {
                        text: modelData.ville + " (" + modelData.count + ")"
                        color: "black"
                        font.bold: true
                        font.pixelSize: 12
                        anchors.top: icon.bottom
                        anchors.horizontalCenter: icon.horizontalCenter
                    }
                }
            }
        }

        // Gestion des gestes et interactions
        PinchHandler {
            id: pinch
            target: map
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            onRotationChanged: (delta) => {
                map.bearing -= delta
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }

        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }

        DragHandler {
            id: drag
            target: map
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }

        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }

        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }
    }
}
