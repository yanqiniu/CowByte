import maya.cmds as cmds
import pymel.core as pm

cowbyte_folder = "F:\\Projects\\CowByte\\"
tools_folder = cowbyte_folder + "tools\\Maya\\"
# export_folder = cowbyte_folder + "assets\\AssetsOut\\"
export_folder = cowbyte_folder + "assets\\meshes\\"

def _null(*args):
    pass

def RemoveShelf(name):
    if cmds.shelfLayout(name, ex=1):
        print("Shelf {0} exists.\n".format(name))
        if cmds.shelfLayout(name, ca=1, q = 1):
            for each in cmds.shelfLayout(name, ca=1, q = 1):
                print("Deleting {0}...\n".format(each))
                cmds.deleteUI(each)
            cmds.deleteUI(name)
    else:
        print("Shelf {0} does not exist.\n".format(name))

# modified from shelfBase.py by Vasil Shotarov
# https://gist.github.com/vshotarov/1c3176fe9e38dcaadd1e56c2f15c95d9#file-shelfbase-py
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
        cmds.shelfButton(width=32, height=32, image=icon, l=label, command=command, dcc=doubleCommand, olb=self.labelBackground, olc=self.labelColour)

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
# GUI functions
def show_message_popup(msgtitle, msg):
    pm.window( title=msgtitle, widthHeight=(350, 125), sizeable = False)
    pm.rowLayout( numberOfColumns=1, columnWidth1=(350), columnAlign=(1, 'center'))
    pm.text(msg, width = 350, wordWrap = True)
    pm.showWindow()

###################################################################################
# CowByte functions

def export_meshfile(meshname, filepath):
    outfile = open(filepath, 'w+')
    outfile.write("MESH\n")
    outfile.write(meshname + "_pos.bufa\n")
    outfile.write(meshname + "_index.bufa\n")
    outfile.write(meshname + "_normal.bufa\n")
    outfile.write(meshname + "_uv.bufa\n")
    outfile.close()

def export_pos_buf(faces, filepath):
    print("Exporting [{0}]...".format(filepath))
    outfile = open(filepath, 'w+')
    outfile.write("POS_BUF\n")
    outfile.write("{0}\n".format(len(faces)*3))
    for f in faces:
        vertices = f.getPoints()
        assert len(vertices) == 3
        # flip z because maya uses right-handed system
        outfile.write("{0} {1} {2}\n".format(vertices[0].x, vertices[0].y, -vertices[0].z))
        outfile.write("{0} {1} {2}\n".format(vertices[1].x, vertices[1].y, -vertices[1].z))
        outfile.write("{0} {1} {2}\n".format(vertices[2].x, vertices[2].y, -vertices[2].z))
    outfile.close()


def export_index_buf(faces, filepath):
    print("Exporting [{0}]...".format(filepath))
    outfile = open(filepath, 'w+')
    outfile.write("INDEX_BUF\n")
    outfile.write("{0}\n".format(len(faces)))
    i = 0
    for f in faces:
        # flip winding order cuz we flipped z
        outfile.write("{0} {1} {2}\n".format(i + 2, i + 1, i))
        i = i + 3
    outfile.close()


def export_normal_buf(faces, filepath):
    print("Exporting [{0}]...".format(filepath))
    outfile = open(filepath, 'w+')
    outfile.write("NORMAL_BUF\n")
    outfile.write("{0}\n".format(len(faces)*3))
    for f in faces:
        normal = f.getNormal() # use face normals for sharp edge?
        outfile.write("{0} {1} {2}\n".format(normal.x, normal.y, -normal.z))
        outfile.write("{0} {1} {2}\n".format(normal.x, normal.y, -normal.z))
        outfile.write("{0} {1} {2}\n".format(normal.x, normal.y, -normal.z))

def export_uv(faces, filepath):
    print("Exporting [{0}]...".format(filepath))
    outfile = open(filepath, 'w+')
    outfile.write("UV_BUF\n")
    outfile.write("{0}\n".format(len(faces)*3))
    for f in faces:
        uvs = f.getUVs()
        outfile.write("{0} {1}\n".format(uvs[0][0], uvs[1][0]))
        outfile.write("{0} {1}\n".format(uvs[0][1], uvs[1][1]))
        outfile.write("{0} {1}\n".format(uvs[0][2], uvs[1][2]))



def export_selected_mesh(meshname):
    print("Exporting [{0}]...".format(meshname))
    selected = pm.ls(selection=True)
    if len(selected) != 1:
        gui_message = "CowByte: Failed to export - nothing/or too many selected! One mesh at a time!"
        show_message_popup("Export Failure", gui_message)
        print(gui_message)
        return False

    # TODO: error checking, this is assuming selected has first child as Mesh.
    mesh = pm.listRelatives(selected[0])[0]
    pm.polyTriangulate(mesh) # triangulate the mesh first, since CBE only takes triangles.
    faces = mesh.faces
    export_meshfile(meshname, export_folder + meshname + ".mesha")
    export_pos_buf(faces, export_folder + meshname + "_pos.bufa")
    export_index_buf(faces, export_folder + meshname + "_index.bufa")
    export_normal_buf(faces, export_folder + meshname + "_normal.bufa")
    export_uv(faces, export_folder + meshname + "_uv.bufa")

    print("Finished exporting [{0}].".format(meshname))
    show_message_popup("Export Success", "Files have been exported to {0}.".format(export_folder))
    return True

###################################################################################
# GUI functions
def command_export():
    mesh_name = pm.textField("mesh_name_field", q = True, tx = True)
    print("MESHNAME: {0}".format(mesh_name))
    export_selected_mesh(mesh_name)

def show_exporter_window():
    window = pm.window("CowByte Mesh Exporter", sizeable = False)
    pm.rowColumnLayout( numberOfColumns=2, columnAttach=(1, 'right', 0), columnWidth=[(1, 100), (2, 250)] )
    pm.text( label='Enter Mesh Name: ' )
    mesh_name_field = pm.textField("mesh_name_field")
    pm.button( label='Export!', command= "command_export()" )

    pm.showWindow()



###################################################################################
class CowShelf(_shelf):
    def __init__(self):
        _shelf.__init__(self, "CowByteEngine", "{0}\\art\\".format(tools_folder))


    def build(self):
        self.addButon(label="Export", icon = "export.png", command = "show_exporter_window()")
        # p = cmds.popupMenu(b=1)
        # self.addMenuItem(p, "Export Selected Mesh", command = "ShowExporterWindow()")
shelf = CowShelf()
