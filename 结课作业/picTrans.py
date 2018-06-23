import sys
from subprocess import Popen

from PIL import Image
from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setFixedSize(542, 52)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(0, 0, 91, 16))
        self.label.setText("")
        self.label.setObjectName("label")
        MainWindow.setCentralWidget(self.centralwidget)
        self.toolBar = QtWidgets.QToolBar(MainWindow)
        self.toolBar.setMovable(False)
        self.toolBar.setObjectName("toolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.act_choosePic = QtWidgets.QAction(MainWindow)
        self.act_choosePic.setObjectName("act_choosePic")
        self.act_origin = QtWidgets.QAction(MainWindow)
        self.act_origin.setObjectName("act_origin")
        self.act_gray = QtWidgets.QAction(MainWindow)
        self.act_gray.setObjectName("act_gray")
        self.act_sharpfy = QtWidgets.QAction(MainWindow)
        self.act_sharpfy.setObjectName("act_sharpfy")
        self.act_smoothfy = QtWidgets.QAction(MainWindow)
        self.act_smoothfy.setObjectName("act_smoothfy")
        self.act_savePic = QtWidgets.QAction(MainWindow)
        self.act_savePic.setObjectName("act_savePic")
        self.act_edgeDetect = QtWidgets.QAction(MainWindow)
        self.act_edgeDetect.setObjectName("act_edgeDetect")
        self.toolBar.addAction(self.act_choosePic)
        self.toolBar.addAction(self.act_origin)
        self.toolBar.addAction(self.act_gray)
        self.toolBar.addAction(self.act_sharpfy)
        self.toolBar.addAction(self.act_smoothfy)
        self.toolBar.addAction(self.act_edgeDetect)
        self.toolBar.addAction(self.act_savePic)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "picTransfer"))
        self.toolBar.setWindowTitle(_translate("MainWindow", "toolBar"))
        self.act_choosePic.setText(_translate("MainWindow", "选择图片"))
        self.act_origin.setText(_translate("MainWindow", "原图"))
        self.act_gray.setText(_translate("MainWindow", "灰度图"))
        self.act_sharpfy.setText(_translate("MainWindow", "锐化"))
        self.act_smoothfy.setText(_translate("MainWindow", "平滑"))
        self.act_savePic.setText(_translate("MainWindow", "保存图片"))
        self.act_edgeDetect.setText(_translate("MainWindow", "边缘检测"))


class Gui(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.act_choosePic.triggered.connect(self.loadImg)
        self.act_gray.triggered.connect(self.graify)
        self.act_sharpfy.triggered.connect(self.sharpfy)
        self.act_smoothfy.triggered.connect(self.smoothfy)
        self.act_origin.triggered.connect(self.origin)
        self.act_savePic.triggered.connect(self.savePic)
        self.act_edgeDetect.triggered.connect(self.edgeDedect)
        self.currPath = None
        self.isGray = False
        self.label.mousePressEvent = self.setStartPoint  #设置回调函数
        self.label.mouseMoveEvent = self.draw
        

    def getPainter(self): #返回绘图器
        pic = self.label.pixmap()
        painter = QtGui.QPainter(pic)
        pen = QtGui.QPen(QtGui.QColor(0, 160, 230))
        pen.setWidth(1)
        pen.setStyle(QtCore.Qt.SolidLine)
        pen.setCapStyle(QtCore.Qt.RoundCap)
        pen.setJoinStyle(QtCore.Qt.RoundJoin)
        painter.setPen(pen)
        return painter


    def setStartPoint(self, event): #设置初始点
        self.getPainter().drawPoint(event.x(), event.y())
        self.lastPoint = (event.x(), event.y())
        self.update()


    def draw(self, event): #对鼠标拖动做出响应
        x = event.x()
        y = event.y()
        painter = self.getPainter()
        painter.drawPoint(x, y)
        painter.drawLine(self.lastPoint[0], self.lastPoint[1], x, y)
        self.lastPoint = (x, y)
        print(event.x(), event.y())
        self.update()
        
        
    def getPath(self, name): #返回绝对路径
        return "/".join([sys.path[0], name])


    def loadImg(self): #加载图片
        path = QtWidgets.QFileDialog.getOpenFileName(self, "Select your pictures", sys.path[0], "Pictures (*.png *.jpg *.jpeg *.bmp)")
        if path and path[0]:
            self.currPath = path[0]
            pic = QtGui.QPixmap(self.currPath)
            self.setFixedSize(pic.size())
            self.label.setFixedSize(pic.size())
            self.label.setPixmap(pic)
            self.isGray = False

            targetDict = { #生成所有临时图片
                "1": (self.currPath, self.getPath(".tempGray.png")),
                "2": (self.getPath(".tempGray.png"), self.getPath(".tempGraySharpfy.png")),
                "3": (self.currPath, self.getPath(".tempSharpfy.png")),
                "4": (self.getPath(".tempGray.png"), self.getPath(".tempGraySmoothfy.png")),
                "5": (self.currPath, self.getPath(".tempSmoothfy.png")),
                "7": (self.getPath(".tempGray.png"), self.getPath(".tempEdge.png"))
            }
            for key in targetDict:
                pipe = Popen([self.getPath("DataStructure.out"), key, targetDict[key][0], targetDict[key][1]])
                pipe.wait()


    def graify(self): #灰化图片
        if self.currPath is None:
            return
        pic = QtGui.QPixmap(self.getPath(".tempGray.png"))
        self.label.setPixmap(pic)
        self.isGray = True


    def sharpfy(self):
        if self.currPath is None:
            return
        if self.isGray == True:
            pic = QtGui.QPixmap(self.getPath(".tempGraySharpfy.png"))
        else:
            pic = QtGui.QPixmap(self.getPath(".tempSharpfy.png"))
        self.label.setPixmap(pic)


    def smoothfy(self):
        if self.currPath is None:
            return
        if self.isGray == True:
            pic = QtGui.QPixmap(self.getPath(".tempGraySmoothfy.png"))
        else:
            pic = QtGui.QPixmap(self.getPath(".tempSmoothfy.png"))
        self.label.setPixmap(pic)
        

    def origin(self):
        if self.currPath is None:
            return
        pic = QtGui.QPixmap(self.currPath)
        self.label.setPixmap(pic)
        self.isGray = False


    def edgeDedect(self):
        if self.currPath is None:
            return
        pic = QtGui.QPixmap(self.getPath(".tempEdge.png"))
        self.label.setPixmap(pic)


    def copy(self, src, target):
        f = open(src, "rb").read()
        open(target, "wb").write(f)


    def savePic(self):
        if self.currPath is None:
            return            
        path = QtWidgets.QFileDialog.getSaveFileName(self, "Select your save place", sys.path[0], "Pictures (*.png *.jpg *.jpeg *.bmp)")
        if path:
            savePath = path[0]
            self.label.pixmap().save(savePath)


app = QtWidgets.QApplication(sys.argv)
mygui = Gui()
mygui.show()
sys.exit(app.exec_())
