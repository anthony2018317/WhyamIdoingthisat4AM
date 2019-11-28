'''inc = include_directories('.')

actor_graph = library('actor_graph', sources: ['Movie.hpp', 'Node.cpp', 'Node.hpp', 'Edge.cpp', 'Edge.hpp', 'ActorGraph.cpp', 'ActorGraph.hpp' ])


actor_graph_dep = declare_dependency(include_directories: inc, 
    link_with: actor_graph)

link_predictor = library('link_predictor', sources: ['linkpredictor.cpp'], dependencies: [actor_graph_dep])

pathfinder = library('pathfinder', sources: ['pathfinder.cpp'], dependencies: [actor_graph_dep])

link_predictor_dep = declare_dependency(include_directories: inc, 
    link_with: link_predictor)

pathfinder_dep = declare_dependency(include_directories: inc, 
    link_with: pathfinder)

actor_graph_exe = executable('actor_graph.cpp.executable', 
    sources: ['ActorGraph.cpp'],
    dependencies : [actor_graph_dep],
    install : true)


link_predictor_exe = executable('link_predictor.cpp.executable', 
    sources: ['linkpredictor.cpp'],
    dependencies : [actor_graph_dep, link_predictor_dep],
    install : true)


pathfinder_exe = executable('pathfinder.cpp.executable', 
    sources: ['pathfinder.cpp'],
    dependencies : [actor_graph_dep, pathfinder_dep],
    install : true)
'''

inc = include_directories('.')
actor_graph = library('actor_graph', sources: ['Movie.hpp', 'Edge.hpp', 'Edge.cpp', 'Node.cpp', 'ActorGraph.hpp', 'ActorGraph.cpp'])
actor_graph_dep = declare_dependency(include_directories: inc, link_with: actor_graph)


link_predictor_exe = executable('link_predictor.cpp.executable', 
    sources: ['linkpredictor.cpp'],
    dependencies : [actor_graph_dep],
    install : true)


pathfinder_exe = executable('pathfinder.cpp.executable', 
    sources: ['pathfinder.cpp'],
    dependencies : [actor_graph_dep],
    install : true)