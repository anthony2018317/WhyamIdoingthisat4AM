subdir('ActorGraph')


link_predictor_exe = executable('linkpredictor.cpp.executable', 
    sources: ['linkpredictor.cpp'],
    dependencies : [actor_graph_dep],
    install : true)


pathfinder_exe = executable('pathfinder.cpp.executable', 
    sources: ['pathfinder.cpp'],
    dependencies : [actor_graph_dep],
    install : true)



movie_traveler_exe = executable('movietraveler.cpp.executable', 
    sources: ['movietraveler.cpp'],
    dependencies : [actor_graph_dep],
    install : true)