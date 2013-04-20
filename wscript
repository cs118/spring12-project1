# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def options (opt):
    opt.load ('compiler_c compiler_cxx')
    opt.load ('boost')

def configure (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

    env.check(function_name='memmem', header_name='string.h', mandatory=False)
    env.check(funciton_name='strncpy', header_name='string.h', mandatory=False)

# comment out the following two lines if compiling not on lnxsrv
#    env.options.boost_includes = '/u/cs/grad/yingdi/boost/include'
#    env.options.boost_libs     = '/u/cs/grad/yingdi/boost/lib'
#

    env.check_boost(lib='system thread')
    env.env.append_value('CXXFLAGS', ['-O0', '-g3', '-Wall', '-Wno-unused-local-typedefs', '-Werror'])

def build (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

    server = env.program (
        target = "http-proxy",
        features = ["cxx", "cxxprogram"],
        use = 'BOOST BOOST_SYSTEM BOOST_THREAD',
        source = [
            # Put additional files here
            # ...
            #
            "compat.cc", # OSX 10.6 compatibility
            "http-proxy.cc", # main() function is here
            "http-headers.cc",
            "http-request.cc",
            "http-response.cc",
            ]
        )
