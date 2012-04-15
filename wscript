# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def options (opt):
    opt.load ('compiler_c compiler_cxx')
    opt.load ('boost')

def configure (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')
    env.env.append_value('CXXFLAGS', ['-O0', '-g3'])

def build (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

    server = env.program (
        target = "http-proxy",
        features = ["cxx", "cxxprogram"],
        use = 'BOOST BOOST_REGEX',
        source = [
            "http-proxy.cc",
            "http-headers.cc",
            "http-request.cc",
            ]
        )

    client = env.program (
        target = "http-get",
        features = ["cxx", "cxxprogram"],
        use = 'BOOST BOOST_REGEX',
        source = [
            "http-get.cc",
            "http-headers.cc",
            "http-request.cc",
            ]
        )
