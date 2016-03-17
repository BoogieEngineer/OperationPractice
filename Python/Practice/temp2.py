import sys

from PyQt4 import QtCore, QtGui


class MainWidget(QtGui.QWidget):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.scene = QtGui.QGraphicsScene()
        self.view = QtGui.QGraphicsView(self.scene)
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.view)
        self.setLayout(layout)
        self.pixmap_item = QtGui.QGraphicsPixmapItem(QtGui.QPixmap('image.png'), None, self.scene)
        self.pixmap_item.mousePressEvent = self.pixelSelect
        self.click_positions = []

    def pixelSelect(self, event):
        self.click_positions.append(event.pos())
        if len(self.click_positions) < 4:
            return
        pen = QtGui.QPen(QtCore.Qt.red)
        self.scene.addPolygon(QtGui.QPolygonF(self.click_positions), pen)
        for point in self.click_positions:
            self.scene.addEllipse(point.x(), point.y(), 2, 2, pen)
        self.click_positions = []


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    widget = MainWidget()
    widget.resize(640, 480)
    widget.show()
    sys.exit(app.exec_())