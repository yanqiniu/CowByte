# modified from shelfBase.py by Vasil Shotarov
# https://gist.github.com/vshotarov/1c3176fe9e38dcaadd1e56c2f15c95d9#file-shelfbase-py

import maya.cmds as cmds

def _null(*args):
    pass


class _shelf():

    def __init__(self, name="customShelf", iconPath=""):
        self.name = name
        self.iconPath = iconPath
        self.labelBackground = (0, 0, 0, 0)
        self.labelColour = (0.6, 0.8, 1.0)


        self._removeOlderVersion()
        cmds.shelfLayout(self.name, p="ShelfLayout")
        cmds.setParent(self.name)
        self.build()

    def build(self):
        ''' to be overwritten by child classes'''
        pass

    def addButon(self, label, icon="commandButton.png", command=_null, doubleCommand=_null):
        '''Adds a shelf button with the specified label, command, double click command and image.'''
        cmds.setParent(self.name)
        if icon:
            icon = self.iconPath + icon
        cmds.shelfButton(width=37, height=37, image=icon, l=label, command=command, dcc=doubleCommand, imageOverlayLabel=label, olb=self.labelBackground, olc=self.labelColour)

    def addMenuItem(self, parent, label, command=_null, icon=""):
        '''Adds a shelf button with the specified label, command, double click command and image.'''
        if icon:
            icon = self.iconPath + icon
        return cmds.menuItem(p=parent, l=label, c=command, i="")

    def addSubMenu(self, parent, label, icon=None):
        '''Adds a sub menu item with the specified label and icon to the specified parent popup menu.'''
        if icon:
            icon = self.iconPath + icon
        return cmds.menuItem(p=parent, l=label, i=icon, subMenu=1)

    def _removeOlderVersion(self):
        '''Check if the shelf exists and nuc it if so.'''
        if cmds.shelfLayout(self.name, ex=1):
            print("Shelf {0} exists.\n".format(self.name))
            if cmds.shelfLayout(self.name, ca=1, q = 1):
                for each in cmds.shelfLayout(self.name, ca=1, q = 1):
                    print("Deleting {0}...\n".format(each))
                    cmds.deleteUI(each)
                cmds.deleteUI(self.name)
        else:
            print("Shelf {0} does not exist yet.\n".format(self.name))

###################################################################################
'''This is an example shelf.'''
# class customShelf(_shelf):
#     def build(self):
#         self.addButon(label="button1")
#         self.addButon("button2")
#         self.addButon("popup")
#         p = cmds.popupMenu(b=1)
#         self.addMenuItem(p, "popupMenuItem1")
#         self.addMenuItem(p, "popupMenuItem2")
#         sub = self.addSubMenu(p, "subMenuLevel1")
#         self.addMenuItem(sub, "subMenuLevel1Item1")
#         sub2 = self.addSubMenu(sub, "subMenuLevel2")
#         self.addMenuItem(sub2, "subMenuLevel2Item1")
#         self.addMenuItem(sub2, "subMenuLevel2Item2")
#         self.addMenuItem(sub, "subMenuLevel1Item2")
#         self.addMenuItem(p, "popupMenuItem3")
#         self.addButon("button3")
# customShelf()
###################################################################################
