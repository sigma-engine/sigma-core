import sys
import re
import os
import argparse
import bpy
import bmesh
import subprocess
import json
import math


def convert_3d(value, invert_y=True):
    if invert_y:
        return [value[0], value[2], -value[1]]
    else:
        return [value[0], value[2], value[1]]


def triangulate_object(obj):
    me = obj.data
    # Get a BMesh representation
    bm = bmesh.new()
    bm.from_mesh(me)

    bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)

    # Finish up, write the bmesh back to the mesh
    bm.to_mesh(me)
    bm.free()


parser = argparse.ArgumentParser(description='Convert blender files.')
parser.add_argument('-H', '--source-directory', type=str,
                    help='The top level package directory.', required=True)
parser.add_argument('-B', '--build-directory', type=str,
                    help='The top level project build directory.', required=True)
parser.add_argument('-M', '--dependency', action='store_true',
                    help='List dependencies of the source file.')
parser.add_argument('-c', '--source-file', type=str,
                    help='The blender file to convert.', required=True)
arguments = parser.parse_args(sys.argv[sys.argv.index('--') + 1:])

source_file = os.path.realpath(arguments.source_file)
source_directory = os.path.realpath(arguments.source_directory)
build_directory = os.path.realpath(arguments.build_directory)
package_directory, source_file_ext = os.path.splitext(
    os.path.relpath(source_file, source_directory))
rid = os.path.join('blueprint', package_directory)
output_file = os.path.join(build_directory, 'data', rid)

def export_static_mesh(source_directory, build_directory, mesh_objects, export_file, rid):
    bpy.ops.object.select_all(action='DESELECT')
    for obj in mesh_objects:
        bpy.context.scene.objects.active = obj
        obj.select = True

    export_file = export_file + '.dae'
    bpy.ops.wm.collada_export(filepath=export_file, check_existing=False, apply_modifiers=True,
                              selected=True, triangulate=True, use_blender_profile=True, use_texture_copies=False)
    if subprocess.call([os.path.join(build_directory, 'bin', 'mdlcc'), '-H', source_directory, '-B', build_directory, '-c', export_file, '--rid', rid]) != 0:
        sys.exit(-1)


if arguments.dependency:
    dep = open(output_file + source_file_ext + '.dependency', 'w')
    dep.write('set(' + re.sub('[^a-zA-Z0-9]', '_', rid) + '_DEPENDS\n')
    dep.write(')\n')
else:
    try:
        bpy.ops.object.mode_set(mode='OBJECT', toggle=False)

        scene = {}
        materials = {}
        exported_objects = []

        # create the temporary export folder
        export_directory = os.path.join(source_directory, os.path.dirname(
            source_file), '.' + os.path.split(os.path.splitext(source_file)[0])[1])
        if not os.path.exists(export_directory):
            os.makedirs(export_directory)

        # Make sure every object is visable on every layer
        for i in range(len(bpy.context.object.layers)):
            for obj in bpy.data.objects:
                obj.layers[i] = True

        # Export correct material names because collada does not allow '/'
        # in material names
        for mtl in bpy.data.materials:
            materials[re.sub('[^a-zA-Z0-9\-]', '_', mtl.name)] = mtl.name

        material_file = open(os.path.join(export_directory, 'materials'), 'w')
        material_file.write(json.dumps(materials, indent=4, sort_keys=True) + '\n')
        material_file.close()

        # Export groups out as static meshes
        for grp in bpy.data.groups:
            exported_objects += [obj for obj in grp.objects]
            if grp.library is None and not grp.name.endswith('_high'):
                group_meshes = [obj for obj in grp.objects if obj.type == 'MESH' and not obj.name.endswith('_high')]
                if len(group_meshes) > 0:
                    group_dae_file = os.path.join(export_directory, grp.name)
                    rid = os.path.join(package_directory, grp.name)
                    export_static_mesh(source_directory, build_directory,
                                       group_meshes, group_dae_file, rid)

        for obj in bpy.data.objects:
            if obj.name.endswith('_high') or obj in exported_objects:
                continue

            scene[obj.name] = {
                'sigma::transform': {
                    'position': convert_3d(obj.matrix_world.to_translation()),
                    'rotation': convert_3d([math.degrees(x) for x in obj.matrix_world.to_euler('XYZ')]),
                    'scale': convert_3d(obj.matrix_world.to_scale(), False)
                }
            }

            if obj.dupli_group is None and obj.library is None:
                if obj.type == 'MESH':
                    object_dae_file = os.path.join(export_directory, obj.name)
                    rid = os.path.join(package_directory, obj.name)
                    obj.matrix_world.identity()
                    export_static_mesh(source_directory, build_directory, [obj], object_dae_file, rid)

                    scene[obj.name]['sigma::graphics::static_mesh_instance'] = {
                        'mesh_id': os.path.join('static_mesh', rid),
                        'cast_shadows': True
                    }
                elif obj.type == 'LAMP':
                    if obj.data.type == 'SUN':
                        scene[obj.name]['sigma::graphics::directional_light'] = {
                            'color': list(obj.data.color),
                            'intensity': obj.data.energy,
                            'cast_shadows': True
                        }
                    elif obj.data.type == 'POINT':
                        scene[obj.name]['sigma::graphics::point_light'] = {
                            'color': list(obj.data.color),
                            'intensity': obj.data.energy,
                            'cast_shadows': True
                        }
                    else:
                        print("WARNING: Can not export light type '{}'".format(obj.data.type))
                else:
                    print("WARNING: Can not export object type '{}'".format(obj.type))
            elif obj.dupli_group is not None and obj.dupli_group.library is None:
                scene[obj.name]['sigma::graphics::static_mesh_instance'] = {
                    'mesh_id': os.path.join('static_mesh', package_directory, obj.dupli_group.name),
                    'cast_shadows': True
                }
            elif obj.dupli_group is not None and obj.dupli_group.library is not None:
                group_package_directory = obj.dupli_group.library.filepath
                if group_package_directory.startswith("//"):
                    group_package_directory = os.path.join(os.path.dirname(source_file), group_package_directory[2:])
                group_package_directory = os.path.splitext(os.path.relpath(group_package_directory, source_directory))[0]

                scene[obj.name]['sigma::graphics::static_mesh_instance'] = {
                    'mesh_id': os.path.join('static_mesh', group_package_directory, obj.dupli_group.name),
                    'cast_shadows': True
                }

        blueprint_file = open(output_file, 'w')
        blueprint_file.write(json.dumps(scene, indent=4, sort_keys=True) + '\n')
        blueprint_file.close()
    except BaseException as e:
        print(e)
        sys.exit(-1)
