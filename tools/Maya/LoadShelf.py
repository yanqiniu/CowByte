import maya.cmds as mc
from CowShelf import _shelf


maya_tools_path = os.path.dirname(os.path.realpath(__file__))

def PrintPath():
    dir_path = os.path.dirname(os.path.realpath(__file__))

print(maya_tools_path)

class Shelf(_shelf):
    def __init__(self,name):
        _shelf.__init__(self, name, "")

    def build(self):
        self.addButon(label="button1")
        self.addButon("button2")

PrintPath()
# s = Shelf("CowByteEngine")

