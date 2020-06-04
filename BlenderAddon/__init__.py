# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

bl_info = {
    "name" : "Test",
    "author" : "Jon Freer",
    "description" : "",
    "blender" : (2, 80, 0),
    "version" : (0, 0, 1),
    "location" : "View3D",
    "warning" : "",
    "category" : "Generic"
}

import bpy

from . test_op import Test_OT_Operator ,SetDomain ,FrameProps

from . test_panel import Sim_PT_Panel


classes = (Test_OT_Operator,Sim_PT_Panel,SetDomain,FrameProps)
reg,unreg = bpy.utils.register_classes_factory(classes)

def register():
    print("registering")
    reg()
    # bpy.utils.register_classes_factory(classes)
    bpy.types.Scene.FrameProps = bpy.props.PointerProperty(type=FrameProps)

def unregister():
    unreg() 
    bpy.utils.register_classes_factory(classes)
    # $ delete QueryProps on unregister
    del(bpy.types.Scene.FrameProps)

if __name__ == "__main__":
    register()