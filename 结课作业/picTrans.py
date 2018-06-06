import sys
from subprocess import Popen

from PIL import Image
from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setFixedSize(430, 96)
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
        self.toolBar.addAction(self.act_choosePic)
        self.toolBar.addAction(self.act_origin)
        self.toolBar.addAction(self.act_gray)
        self.toolBar.addAction(self.act_sharpfy)
        self.toolBar.addAction(self.act_smoothfy)
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
        self.act_smoothfy.setText(_translate("MainWindow", "模糊"))
        self.act_savePic.setText(_translate("MainWindow", "保存图片"))


class Gui(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.currPath = None
        self.currStatus = None
        self.act_choosePic.triggered.connect(self.getImgPath)
        self.act_gray.triggered.connect(self.graify)
        self.act_sharpfy.triggered.connect(self.sharpfy)
        self.act_smoothfy.triggered.connect(self.smoothfy)
        self.act_origin.triggered.connect(self.origin)
        self.act_savePic.triggered.connect(self.savePic)

    def joinThePath(self, name):
        return "/".join([sys.path[0], name])

    def getImgPath(self):
        path = QtWidgets.QFileDialog.getOpenFileName(self, "Select your pictures", sys.path[0], "Pictures (*.png *.jpg *.jpeg *.bmp)")
        if path and path[0]:
            self.currPath = path[0]
            if Image.open(self.currPath).mode == "RGBA": #检测是否是彩色图片
                pic = QtGui.QPixmap(self.currPath)
                self.setFixedSize(pic.size())
                self.label.setFixedSize(pic.size())
                self.label.setPixmap(pic)
                self.currStatus = "origin"
                self.currSharp = None
            else:
                QtWidgets.QMessageBox.warning(self, "Tips", "请选择一张彩色图片")
                
    def graify(self):
        if self.currPath is None:
            return
        pipe = Popen([self.joinThePath("DataStructure.out"), "1", self.currPath, self.joinThePath(".tempGray.png")])
        pipe.wait()
        pic = QtGui.QPixmap(self.joinThePath(".tempGray.png"))
        self.setFixedSize(pic.size())
        self.label.setFixedSize(pic.size())
        self.label.setPixmap(pic)
        self.currStatus = "gray"
        self.currSharp = None

    def sharpfy(self):
        if self.currPath is None:
            return
        if self.currStatus == "gray":
            pipe = Popen([self.joinThePath("DataStructure.out"), "2", self.joinThePath(".tempGray.png"), self.joinThePath(".tempGraySharpfy.png")])
            pipe.wait()
            pic = QtGui.QPixmap(self.joinThePath(".tempGraySharpfy.png"))
        else:
            pipe = Popen([self.joinThePath("DataStructure.out"), "3", self.currPath, self.joinThePath(".tempSharpfy.png")])
            pipe.wait()
            pic = QtGui.QPixmap(self.joinThePath(".tempSharpfy.png"))
        self.setFixedSize(pic.size())
        self.label.setFixedSize(pic.size())
        self.label.setPixmap(pic)
        self.currSharp = "sharp"

    def smoothfy(self):
        if self.currPath is None:
            return
        if self.currStatus == "gray":
            pipe = Popen([self.joinThePath("DataStructure.out"), "4", self.joinThePath(".tempGray.png"), self.joinThePath(".tempGraySmoothfy.png")])
            pipe.wait()
            pic = QtGui.QPixmap(self.joinThePath(".tempGraySmoothfy.png"))
        else:
            pipe = Popen([self.joinThePath("DataStructure.out"), "5", self.currPath, self.joinThePath(".tempSmoothfy.png")])
            pipe.wait()
            pic = QtGui.QPixmap(self.joinThePath(".tempSmoothfy.png"))
        self.setFixedSize(pic.size())
        self.label.setFixedSize(pic.size())
        self.label.setPixmap(pic)
        self.currSharp = "smooth"
        
    def origin(self):
        if self.currPath is None:
            return
        pic = QtGui.QPixmap(self.currPath)
        self.setFixedSize(pic.size())
        self.label.setFixedSize(pic.size())
        self.label.setPixmap(pic)
        self.currStatus = "origin"
        self.currSharp = None

    def copy(self, src, target):
        f = open(src, "rb").read()
        open(target, "wb").write(f)

    def savePic(self):
        if self.currStatus is None:
            return            
        path = QtWidgets.QFileDialog.getSaveFileName(self, "Select your save place", sys.path[0], "Pictures (*.png *.jpg *.jpeg *.bmp)")
        if path:
            savePath = path[0]
            if self.currStatus == "gray":
                if self.currSharp is None:
                    self.copy(self.joinThePath(".tempGray.png"), savePath)
                if self.currSharp == "sharp":
                    self.copy(self.joinThePath(".tempGraySharpfy.png"), savePath)
                if self.currSharp == "smooth":
                    self.copy(self.joinThePath(".tempGraySmoothfy.png"), savePath)
            elif self.currStatus == "origin":
                if self.currSharp is None:
                    self.copy(self.joinThePath(".temp.png"), savePath)
                if self.currSharp == "sharp":
                    self.copy(self.joinThePath(".tempSharpfy.png"), savePath)
                if self.currSharp == "smooth":
                    self.copy(self.joinThePath(".tempSmoothfy.png"), savePath)

app = QtWidgets.QApplication(sys.argv)
mygui = Gui()
mygui.show()
sys.exit(app.exec_())
