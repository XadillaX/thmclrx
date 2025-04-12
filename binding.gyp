{
    "targets": [
        {
            "target_name": "xmempool",
            "type": "static_library",
            "sources": [
                "./deps/byakuren/third-party/xmempool/xmempool.c"
            ],
            "cflags": [
                "-std=c99"
            ]
        },
        {
            "target_name": "byakuren",
            "type": "static_library",
            "dependencies": [
                "xmempool"
            ],
            "sources": [
                "./deps/byakuren/byakuren.c",
                "./deps/byakuren/bkr_common.c",
                "./deps/byakuren/const/palette.c",
                "./deps/byakuren/lib/mindiff.c",
                "./deps/byakuren/lib/mix.c",
                "./deps/byakuren/lib/octree.c"
            ],
            "cflags": [
                "-std=c99"
            ]
        },
        {
            "target_name": "thmclrx",
            "dependencies": [
                "byakuren"
            ],
            "sources": [
                "./src/thmclrx.cc",
                "./src/common.cc"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "deps",
                "deps/byakuren/third-party"
            ]
        }
    ]
}
