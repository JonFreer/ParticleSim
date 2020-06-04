import bpy
import ctypes
from ctypes import *


class Test_OT_Operator(bpy.types.Operator):
    bl_idname = "sim.calc"
    bl_label= "Calculate Sim"
    bl_description = "Calculate Sim"
    simDLL = ctypes.CDLL("C:\\Users\\GuildTV\\Desktop\\Jon\\SImulation\\SimDLL\\x64\\Debug\\SimDLL.dll")

    def execute(self, context):
        print(self.simDLL.sim_check())
        

        bpy.ops.object.select_all(action='DESELECT')
        if(SetDomain.domain.select_get() is False):
            SetDomain.domain.select_set(True)
        bpy.context.view_layer.objects.active = SetDomain.domain
        bpy.ops.object.transform_apply(location = False, scale = True, rotation = False)
        xSpan = SetDomain.domain.bound_box[4][0] - SetDomain.domain.bound_box[0][0]
        zSpan = SetDomain.domain.bound_box[6][1] - SetDomain.domain.bound_box[0][1]
        ySpan = SetDomain.domain.bound_box[1][2] - SetDomain.domain.bound_box[0][2]
        frames = bpy.context.scene.FrameProps.Frames
        count = bpy.context.scene.FrameProps.Count
        temp = bpy.context.scene.FrameProps.Temporal
        xCenter = SetDomain.domain.location.x - (xSpan/2)
        yCenter = SetDomain.domain.location.y - (ySpan/2)
        zCenter = SetDomain.domain.location.z - (zSpan/2)
        data1 = POINTER(XYZ)()
        self.simDLL.sim_init(count,c_int(frames),c_int(temp),c_float(xSpan),c_float(zSpan),c_float(ySpan),byref(data1))
        print("collected")
        objs = []
        for i in range(count):
            bpy.ops.mesh.primitive_uv_sphere_add(segments=10, ring_count=10, radius=0.05, enter_editmode=False,
            location=(data1[i].x[0]+xCenter, data1[i].z[0]+zCenter,data1[i].y[0]+yCenter))
            objs.append(bpy.context.view_layer.objects.active)
            #bpy.context.view_layer.objects.active = SetDomain.domain
            for j in range(frames):
                bpy.context.scene.frame_set(j)
                objs[i].location = (data1[i].x[j*temp]+xCenter, data1[i].z[j*temp]+zCenter,data1[i].y[j*temp]+yCenter)
                
                bpy.ops.anim.keyframe_insert_menu(type='Location')

        return {"FINISHED"}

# class XYZ(Structure):
#     _fields_ = [("x", c_float),("y", c_float),("z", c_float)]

class XYZ(Structure):
    _fields_ = [("x", POINTER(c_float)),("y", POINTER(c_float)),("z", POINTER(c_float))]

class SetDomain(bpy.types.Operator):
    bl_label = "Set Domain"
    bl_description = "Set Domain"
    bl_idname = "sim.set_domain"
    domain = None
    def __init__(self):
        SetDomain.domain = None
    def execute(self, context):
        SetDomain.domain = context.active_object
        SetDomain.domain.display_type = 'WIRE'
        return {"FINISHED"}


class FrameProps(bpy.types.PropertyGroup):

    Frames: bpy.props.IntProperty(default=50,max=300,min=1)
    Temporal: bpy.props.IntProperty(default=10,max=50,min=1)
    Count: bpy.props.IntProperty(default=10,max=500,min=1)



    