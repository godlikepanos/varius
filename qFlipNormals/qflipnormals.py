#/usr/bin/python
import sys
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from MainWdg import *

app = QApplication(sys.argv)
mainWdg = MainWdg()
mainWdg.show()

app.exec_()

