import bpy,sys


def export_fbx(output_path='./',scene=False, types={'MESH'}):
    # {'EMPTY', 'CAMERA', 'LAMP', 'ARMATURE', 'MESH', 'OTHER' }
    bpy.ops.export_scene.fbx(filepath=output_path,
                             check_existing=False,
                             axis_forward='-Z',
                             axis_up='Y',
                             filter_glob="*.fbx",
                             version='BIN7400',
                             ui_tab='MAIN',
                             use_selection=scene,
                             global_scale=1,
                             apply_unit_scale=True,
                             bake_space_transform=False,
                             object_types=types,
                             use_mesh_modifiers=True,
                             mesh_smooth_type='OFF',
                             use_mesh_edges=False,
                             use_tspace=False,
                             use_custom_props=False,
                             add_leaf_bones=True,
                             primary_bone_axis='Y',
                             secondary_bone_axis='X',
                             use_armature_deform_only=False,
                             armature_nodetype='NULL',
                             bake_anim=True,
                             bake_anim_use_all_bones=True,
                             bake_anim_use_nla_strips=True,
                             bake_anim_use_all_actions=True,
                             bake_anim_force_startend_keying=True,
                             bake_anim_step=1,
                             bake_anim_simplify_factor=1,
                             use_anim=True,
                             use_anim_action_all=True,
                             use_default_take=True,
                             use_anim_optimize=True,
                             anim_optimize_precision=6,
                             path_mode='AUTO',
                             embed_textures=False,
                             batch_mode='OFF' if scene else 'GROUP',
                             use_batch_own_dir=False,
                             use_metadata=True)


args = sys.argv[sys.argv.index('--'):]
bpy.ops.object.mode_set(mode='OBJECT')
bpy.ops.object.select_all(action="DESELECT")
if '--scene' in args:
    for obj in bpy.data.objects:
        obj.select = not obj.is_duplicator
    export_fbx("scene.fbx",True)
else:
    export_fbx()    


