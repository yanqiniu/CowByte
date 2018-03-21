# Load CowByte Engine custom Maya tool shelf.

import importlib
import maya.cmds as mc

shelf = input('ShelfBase.py')
importlib.import_module(shelf)

# null placeholder
def _null(*args):
    pass


class Shelf(_shelf):
    def build(self):
        self.addButon(label="button1")
        self.addButon("button2")
        self.addButon("popup")
        p = mc.popupMenu(b=1)
        self.addMenuItem(p, "popupMenuItem1")
        self.addMenuItem(p, "popupMenuItem2")
        sub = self.addSubMenu(p, "subMenuLevel1")
        self.addMenuItem(sub, "subMenuLevel1Item1")
        sub2 = self.addSubMenu(sub, "subMenuLevel2")

s = Shelf()
