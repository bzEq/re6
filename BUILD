cc_library(
    name='re6',
    srcs=glob(['lib/*.cc']),
    hdrs=glob(['lib/*.h', 'include/**/*.h']),
    copts=[
        '-std=c++17',
        '-Wall',
        '-Werror',
        '-O2',
    ],
    includes=[
        'include',
    ],
    visibility=["//visibility:public"],
)
