{
    "targets": [
        {
            "target_name": "xmempool",
            "type": "static_library",
            "sources": [
                "./src/third-party/xmempool/xmempool.c"
            ]
        },
        {
            "target_name": "thmclrx",
            "dependencies": [
                "xmempool"
            ],
            "sources": [
                "./src/thmclrx.cpp",
                "./src/octree.cpp",
                "./src/mindiffer.cpp",
                "./src/common.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
