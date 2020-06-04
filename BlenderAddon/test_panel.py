import bpy

class Sim_PT_Panel(bpy.types.Panel):
    bl_idname="Sim_Panel"
    bl_label = "Simulation"
    bl_category = "Sim"
    bl_space_type ="VIEW_3D"
    bl_region_type = "UI"

    def draw(self, context):
        layout = self.layout

        SetFrames = layout.row()
        props = bpy.context.scene.FrameProps
        SetFrames.prop(props, 'Frames', slider=True)
        SetTempor = layout.row()
        SetTempor.prop(props, 'Temporal', slider=True)
        SetCount = layout.row()
        SetCount.prop(props, 'Count', slider=True)
        row = layout.row()
        row.operator("sim.set_domain",text="Set Domain")
        row.operator("sim.calc",text="Calculate")
