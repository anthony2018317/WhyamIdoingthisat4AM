subdir('ActorGraph')


link_predictor_exe = executable('link_predictor.cpp.executable', 
    sources: ['linkpredictor.cpp'],
    dependencies : [actor_graph_dep],
    install : true)


pathfinder_exe = executable('pathfinder.cpp.executable', 
    sources: ['pathfinder.cpp'],
    dependencies : [actor_graph_dep],
    install : true)