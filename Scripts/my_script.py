import unreal
#import sys
# import requests
#import json

#"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\ThirdParty\Python3\Win64\python.exe" -m pip install

unreal.log("Log!")

editor_subsystem = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

actors = unreal.GameplayStatics.get_all_actors_of_class_with_tag(editor_subsystem.get_editor_world(),unreal.Actor,"Player")
player = actors[0]

#unreal.Actor.set_actor_hidden_in_game

with unreal.ScopedEditorTransaction("My Transaction Test") as trans:
    player.set_editor_property("hidden",not player.get_editor_property("hidden"))

total_frames = 100
text_label = "Working!"
with unreal.ScopedSlowTask(total_frames, text_label) as slow_task:
    slow_task.make_dialog(True)               # Makes the dialog visible, if it isn't already
    for i in range(total_frames):
        if slow_task.should_cancel():         # True if the user has pressed Cancel in the UI
            break
        slow_task.enter_progress_frame(1)     # Advance progress by one frame.
                                            # You can also update the dialog text in this call, if you want.

unreal.log_error("Ended script!")