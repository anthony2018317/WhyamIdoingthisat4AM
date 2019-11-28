inc = include_directories('.')

actor_graph = library('actor_graph', sources: ['ActorGraph.cpp', 'ActorGraph.hpp'])

actor_graph_dep = declare_dependency(include_directories: inc, 
    link_with: actor_graph)

actor_graph_exe = executable('actor_graph.cpp.executable', 
    sources: ['ActorGraph.cpp'],
    dependencies : [cxxopts_dep, actor_graph_dep],
    install : true)