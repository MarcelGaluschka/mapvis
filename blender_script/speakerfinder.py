# general imports
import sys
import csv
import os
import math

# blender imports
import bpy
import bmesh

#import os from bpy.props 
from bpy.props import StringProperty 
from bpy_extras.io_utils import BoolProperty 
from bpy_extras.io_utils import ImportHelper
from bpy.types import Operator 

bl_info = {
    "name": "Speaker Finder",
    "blender": (2, 80, 0),
    "category": "Object",
}


class SpeakerFinder(bpy.types.Operator, ImportHelper):
    bl_idname  = "object.speaker_finder"
    bl_label = "Find Speakers"
    #bl_idname = "test.open_filebrowser" 
    #bl_label = "Open the file browser (yay)" 
    filter_glob: StringProperty(
        default='*.csv;', 
        ) 
    filepath = bpy.props.StringProperty(subtype="FILE_PATH")
    some_boolean: BoolProperty(
        name='Do a nex thing', 
        description='Do a thing with the file you\'ve selected',
        default=True) 
    
    def execute(self, context):
        
        print(self.filepath)
        
        
        
        def distance(location1, location2):
            sum = 0
            for i in range(len(location1)):
                sum += math.pow((location1[i] - location2[i]),2)
            return math.sqrt(sum)
        
        def get_nearest_speaker(locations, new_loc):
            nearest = 0
            n_dist = distance(locations[0], new_loc)
            
            for i in range (len(locations)):
                temp_dist = distance(locations[i], new_loc)
                if (temp_dist < n_dist):
                    n_dist = temp_dist
                    nearest = i
            if (n_dist > 0.0):
                for i in range (len(locations)): 
                    if(locations[i] == (0,0,0)):
                        return i
            return nearest
        
        number_speakers = 4
        
        samplerate = 0
        blocksize = 0
        custom_framerate = bpy.context.scene.render.fps / bpy.context.scene.render.fps_base
        
        for obj in bpy.data.objects:
            bpy.data.objects.remove(obj)
               
        ths = []          
        with open (self.filepath, mode = "r") as file:
            csvFile = csv.reader(file, delimiter=",")
            line_count = 0
            tmp_row = []
            for row in csvFile:
                if line_count == 0:
                    samplerate = int(row[0])
                    blocksize = int(row[1])
                else:
                    tmp_row = [float(row[0]), float(row[1]), float(row[2])]
                    ths.append(tmp_row)
                line_count +=1 
 
        
        block_duration = (blocksize/samplerate) ## what time takes one block
        
        frame_time = (1/custom_framerate) ## what time takes one frame
        
        block_to_frame = custom_framerate / (1/block_duration)
        
        scn = bpy.context.scene                    
        scn.frame_start = 1
        scn.frame_end = int(math.ceil(1 * len(ths) * block_to_frame))
        
        ## TODO make new object beeing a sphere
        allspeakers = []
        speakerlocations = []

        for i in range (number_speakers):
            mesh = bpy.data.meshes.new('Basic_Sphere')
            basic_sphere = bpy.data.objects.new("speaker_" + str(i), mesh)

            # Add the object into the scene.
            myCollection = bpy.data.collections[0]
            myCollection.objects.link(basic_sphere)

            # Construct the bmesh cube and assign it to the blender mesh.
            bm = bmesh.new()
            bmesh.ops.create_uvsphere(bm, u_segments=32, v_segments=16, diameter=0.1)
            bm.to_mesh(mesh)
            bm.free()

            allspeakers.append(basic_sphere)
            speakerlocations.append((0,0,0))

        ## error prevention
        if basic_sphere.visible_get() == False:
            print("Error object not visible")
            return {"CANCELLED"}
        
        last_angl_x =  (math.pi/180) * ths[0][0]
        last_angl_y =  (math.pi/180) * ths[0][1]
        last_dbs = ths[0][2]
        
        for i in range(int(len(ths))):
            
            temps_ths = ths[i]
            
            curr_angl_x_tmp = (math.pi/180) * temps_ths[0]
            curr_angl_y_tmp = (math.pi/180) * temps_ths[1]
            curr_dbs = temps_ths[2]
            
            new_location = (math.sin(curr_angl_x_tmp), math.cos(curr_angl_x_tmp), math.sin(curr_angl_y_tmp))
            
            curr_speaker = get_nearest_speaker(speakerlocations, new_location)

            tempObject = allspeakers[curr_speaker]
            
                        
            curr_angl_x = (math.pi/180) * temps_ths[0]
            curr_angl_y = (math.pi/180) * temps_ths[1]
            
            
            if (curr_angl_x == last_angl_x and curr_angl_y == last_angl_y):
                curr_location = (math.sin(curr_angl_x), math.cos(curr_angl_x), math.sin(curr_angl_y))
                tempObject.location = curr_location
                tempObject.keyframe_insert(data_path='location',frame=(int(i * block_to_frame)))
                speakerlocations[curr_speaker] = curr_location
            else:
                tempObject.location = speakerlocations[curr_speaker]
                tempObject.keyframe_insert(data_path='location',frame=(int((i-1) * block_to_frame)))
  
            for j in range(len(allspeakers)):
                if (j != curr_speaker):
                    allspeakers[j].location = speakerlocations[j]
                    allspeakers[j].keyframe_insert(data_path='location',frame=(int((i-1) * block_to_frame)))
           
            last_angl_x = curr_angl_x
            last_angl_y = curr_angl_y     
        return {"FINISHED"}


def register(): 
    bpy.utils.register_class(SpeakerFinder) 
    
def unregister(): 
    bpy.utils.unregister_class(SpeakerFinder) 

if __name__ == "__main__": 
    #unregister()
    register() 
    
# test call 
#    bpy.ops.object.speaker_finder()
    
register()