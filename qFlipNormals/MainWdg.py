from PyQt4.QtGui import *
from PyQt4.QtCore import *
from Ui_MainWdg import *
import os.path


class MainWdg(QMainWindow, Ui_MainWdg):
	def __init__(self, parent = None):
		QMainWindow.__init__(self, parent)
		Ui_MainWdg.setupUi(self, self)
		
		self.imageOpened = 0
		self.lastAccessedFile = QString("")
				
		fileToolbar = QToolBar()
		fileToolbar.addAction(self.openAct)
		fileToolbar.addAction(self.saveAct)
		fileToolbar.addAction(self.quitAct)
		
		actionsToolbar = QToolBar()
		actionsToolbar.addAction(self.flipXYAct)
		actionsToolbar.addAction(self.flipXZAct)
		actionsToolbar.addAction(self.flipYZAct)
		
		self.addToolBar(fileToolbar)
		self.addToolBar(actionsToolbar)
		
		self.quitAct.triggered.connect(self.close)
		self.openAct.triggered.connect(self.openActTriggered)
		self.saveAct.triggered.connect(self.saveActTriggered)
		self.flipXYAct.triggered.connect(self.flipXY)
		self.flipXZAct.triggered.connect(self.flipXZ)
		self.flipYZAct.triggered.connect(self.flipYZ)
		
		
	def openActTriggered(self):
		fName = QFileDialog.getOpenFileName(self, "Open File", self.lastAccessedFile, "Images (*.png *.jpg *.tga)")
		if fName != "":
			self.originalImg.setPixmap(QPixmap(QImage(fName)))
			self.previewImg.setPixmap(QPixmap(QImage(fName)))
			self.imageOpened = 1
			self.lastAccessedFile = fName
			
			
	def saveActTriggered(self):
		if self.imageOpened == 0:
			QMessageBox.critical(self, "Error", "You have to open an image first")
			return
		
		fName = QFileDialog.getSaveFileName(self, "Save File", self.lastAccessedFile)
		if fName != "":
			ext = QFileInfo(fName).suffix().toLower()
			if ext != "png" and ext != "tga" and ext != "jpg":
				QMessageBox.critical(self, "Error", "Incorrect file extension " + ext)
			else: 
				self.previewImg.pixmap().save(fName, ext)
	
	
	def flip(self, op):
		if self.imageOpened == 0:
			QMessageBox.critical(self, "Error", "You have to open an image first")
			return
	
		inImg = self.originalImg.pixmap().toImage()
		outImg = QImage(inImg.width(), inImg.height(), QImage.Format_RGB32)
		for x in range(inImg.width()):
			for y in range(inImg.height()):
				col = QColor(inImg.pixel(x, y))
				if op == "xy":
					newCol = QColor(col.green(), col.red(), col.blue())
				elif op == "xz":
					newCol = QColor(col.blue(), col.green(), col.red())
				elif op == "yz":
					newCol = QColor(col.red(), col.blue(), col.green())
				else:
					raise RuntimeError("incorrect operation " + op)

				outImg.setPixel(x, y, newCol.rgb())
		
		self.previewImg.setPixmap(QPixmap(outImg))
				
				
	def flipXY(self):
		self.flip("xy")
		
	
	def flipXZ(self):
		self.flip("xz")
	
		
	def flipYZ(self):
		self.flip("yz")
					
