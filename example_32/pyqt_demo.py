# -*- coding: utf-8 -*-
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

#Class is the basic GUI Format
class Form(QWidget):
    #Initalize the Form
    def __init__(self, parent=None):
        super(Form, self).__init__(parent)
        #Create a QT label and button
        nameLabel = QLabel("Name:")
        self.nameLine = QLineEdit()
        self.submitButton = QPushButton("Submit")
 
        #Create the button and text fields
        buttonLayout1 = QVBoxLayout()
        buttonLayout1.addWidget(nameLabel)
        buttonLayout1.addWidget(self.nameLine)
        buttonLayout1.addWidget(self.submitButton)
        #Set action of the bution, to submit Contact call submitContact function
        self.submitButton.clicked.connect(self.submitContact)
 
        #Set main layout 
        mainLayout = QGridLayout()
        mainLayout.addLayout(buttonLayout1, 0, 1)
 
        self.setLayout(mainLayout)
        self.setWindowTitle("Hello Qt")
 
    def submitContact(self):
        name = self.nameLine.text()
        #Check if name is empty and return error if true else return success
        if name == "":
            QMessageBox.information(self, "Empty Field",
                                    "Please enter a name and address.")
            return
        else:
            QMessageBox.information(self, "Success!",
                                    "Hello %s!" % name)
 
if __name__ == '__main__':
    import sys
    #Create Qt applciation 
    app = QApplication(sys.argv)
    #Create and display the form 
    screen = Form()
    screen.show()
    #Exit
    sys.exit(app.exec_())

 