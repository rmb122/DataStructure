import sys
from subprocess import Popen

from PIL import Image
from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setFixedSize(553, 52)
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
        self.act_setting = QtWidgets.QAction(MainWindow)
        self.act_setting.setObjectName("act_setting")
        self.act_binary = QtWidgets.QAction(MainWindow)
        self.act_binary.setObjectName("act_binary")
        self.toolBar.addAction(self.act_choosePic)
        self.toolBar.addAction(self.act_origin)
        self.toolBar.addAction(self.act_gray)
        self.toolBar.addAction(self.act_sharpfy)
        self.toolBar.addAction(self.act_smoothfy)
        self.toolBar.addAction(self.act_binary)
        self.toolBar.addAction(self.act_edgeDetect)
        self.toolBar.addAction(self.act_setting)
        self.toolBar.addAction(self.act_savePic)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "picTransfer"))
        self.toolBar.setWindowTitle(_translate("MainWindow", "toolBar"))
        self.act_choosePic.setText(_translate("MainWindow", "选择"))
        self.act_choosePic.setToolTip(_translate("MainWindow", "选择"))
        self.act_origin.setText(_translate("MainWindow", "原图"))
        self.act_gray.setText(_translate("MainWindow", "灰度"))
        self.act_gray.setToolTip(_translate("MainWindow", "灰度"))
        self.act_sharpfy.setText(_translate("MainWindow", "锐化"))
        self.act_smoothfy.setText(_translate("MainWindow", "平滑"))
        self.act_savePic.setText(_translate("MainWindow", "保存"))
        self.act_savePic.setToolTip(_translate("MainWindow", "保存"))
        self.act_edgeDetect.setText(_translate("MainWindow", "边缘"))
        self.act_edgeDetect.setToolTip(_translate("MainWindow", "边缘"))
        self.act_setting.setText(_translate("MainWindow", "设置"))
        self.act_setting.setToolTip(_translate("MainWindow", "设置"))
        self.act_binary.setText(_translate("MainWindow", "二值"))


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.setFixedSize(200, 159)
        self.btn_color = QtWidgets.QPushButton(Dialog)
        self.btn_color.setGeometry(QtCore.QRect(90, 10, 91, 31))
        self.btn_color.setObjectName("btn_color")
        self.lab_colorNow = QtWidgets.QLabel(Dialog)
        self.lab_colorNow.setGeometry(QtCore.QRect(20, 10, 65, 31))
        self.lab_colorNow.setObjectName("lab_colorNow")
        self.line_width = QtWidgets.QLineEdit(Dialog)
        self.line_width.setGeometry(QtCore.QRect(90, 50, 91, 34))
        self.line_width.setObjectName("line_width")
        self.line_width.setText("5")
        self.btn_confirm = QtWidgets.QPushButton(Dialog)
        self.btn_confirm.setGeometry(QtCore.QRect(50, 120, 91, 31))
        self.btn_confirm.setObjectName("btn_confirm")
        self.lab_LineWidth = QtWidgets.QLabel(Dialog)
        self.lab_LineWidth.setGeometry(QtCore.QRect(20, 50, 71, 31))
        self.lab_LineWidth.setObjectName("lab_LineWidth")
        self.checkBox = QtWidgets.QCheckBox(Dialog)
        self.checkBox.setGeometry(QtCore.QRect(40, 90, 111, 24))
        self.checkBox.setObjectName("checkBox")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Settings"))
        self.btn_color.setText(_translate("Dialog", "设置颜色"))
        self.lab_colorNow.setText(_translate("Dialog", "当前颜色"))
        self.btn_confirm.setText(_translate("Dialog", "确定"))
        self.lab_LineWidth.setText(_translate("Dialog", "笔画粗细"))
        self.checkBox.setText(_translate("Dialog", "马赛克效果"))


class settings(QtWidgets.QDialog, Ui_Dialog):
    def __init__(self):
        QtWidgets.QDialog.__init__(self)
        Ui_Dialog.__init__(self)
        self.setupUi(self)
        self.btn_color.clicked.connect(self.getColor)
        self.btn_confirm.clicked.connect(self.hide)
        self.penColor = QtGui.QColor(0, 160, 230)
        self.lab_colorNow.setStyleSheet("background-color: rgb({R}, {G}, {B});".format(R=self.penColor.red(), G=self.penColor.green(), B=self.penColor.blue()))
        

    def getColor(self):
        color = QtWidgets.QColorDialog()
        color = color.getColor()
        self.lab_colorNow.setStyleSheet("background-color: rgb({R}, {G}, {B});".format(R=color.red(), G=color.green(), B=color.blue()))
        self.update()
        self.penColor = color


class Gui(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.act_choosePic.triggered.connect(self.loadImg)
        self.act_gray.triggered.connect(self.graify)
        self.act_sharpfy.triggered.connect(self.sharpfy)
        self.act_smoothfy.triggered.connect(self.smoothfy)
        self.act_origin.triggered.connect(self.origin)
        self.act_savePic.triggered.connect(self.savePic)
        self.act_binary.triggered.connect(self.binary)
        self.act_edgeDetect.triggered.connect(self.edgeDedect)
        self.act_setting.triggered.connect(self.showSettings)
        self.currPath = None
        self.isGray = False
        self.penWidth = 5
        self.penColor = QtGui.QColor(0, 160, 230)
        self.setModeLine()
        self.settings = settings()
        self.settings.hideEvent = self.reConfiguration #设置隐藏时的回调函数
        

    def showSettings(self):
        self.settings.show()


    def reConfiguration(self, event):
        self.penColor = self.settings.penColor
        self.penWidth = int(self.settings.line_width.text())
        if self.settings.checkBox.isChecked():
            self.setModeMosaic()
        else:
            self.setModeLine()


    def getPainter(self): #返回绘图器
        pic = self.label.pixmap()
        painter = QtGui.QPainter(pic)
        pen = QtGui.QPen(self.penColor)
        pen.setWidth(self.penWidth)
        pen.setStyle(QtCore.Qt.SolidLine)
        pen.setCapStyle(QtCore.Qt.RoundCap)
        pen.setJoinStyle(QtCore.Qt.RoundJoin)
        painter.setPen(pen)
        painter.setRenderHint(painter.Antialiasing, True)
        return painter


    def drawMosaic(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            x = event.x()
            y = event.y()
            painter = self.getPainter()
            w = self.penWidth
            l = self.penWidth * 2 + 1
            x = x - self.penWidth
            y = y - self.penWidth
            for xShift in range(-1, 2):
                for yShift in range(-1, 2):
                    color = self.label.pixmap().toImage().pixelColor(x + xShift * l + w, y + yShift * l + w)
                    painter.fillRect(x + xShift * l, y + yShift * l , l, l, color)
            self.update()


    def doNothing(self, event):
        pass


    def setStartPoint(self, event):  #设置初始点
        if event.button() == QtCore.Qt.LeftButton:
            self.getPainter().drawPoint(event.x(), event.y())
            self.lastPoint = (event.x(), event.y())
            self.update()


    def drawLine(self, event):  #对鼠标拖动做出响应
        if event.buttons() == QtCore.Qt.LeftButton:
            x = event.x()
            y = event.y()
            painter = self.getPainter()
            painter.drawPoint(x, y)
            painter.drawLine(self.lastPoint[0], self.lastPoint[1], x, y)
            self.lastPoint = (x, y)
            self.update()
        
        
    def setModeLine(self):
        self.label.mousePressEvent = self.setStartPoint
        self.label.mouseMoveEvent = self.drawLine


    def setModeMosaic(self):
        self.label.mousePressEvent = self.drawMosaic
        self.label.mouseMoveEvent = self.doNothing


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
                "6": (self.getPath(".tempGray.png"), self.getPath(".tempBinary.png")),
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


    def binary(self):
        if self.currPath is None:
            return
        pic = QtGui.QPixmap(self.getPath(".tempBinary.png"))
        self.label.setPixmap(pic)


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
