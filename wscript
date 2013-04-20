# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def options (opt):
    opt.load ('compiler_c compiler_cxx')
    opt.load ('boost')

def configure (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

    env.check(header_name='string.h')
    env.check(function_name='memmem', header_name='string.h', mandatory=False)
    env.check(function_name='stpncpy', header_name='string.h', mandatory=False)

# comment out the following two lines if compiling not on lnxsrv
#    env.options.boost_includes = '/u/cs/grad/yingdi/boost/include'
#    env.options.boost_libs     = '/u/cs/grad/yingdi/boost/lib'
#

    env.check_boost(lib='system thread')
    env.add_supported_cxxflags (cxxflags = ['-O0', '-g3', '-Wall', 
                                            '-Wno-unused-local-typedefs', '-Wno-unused-private-field', 
                                            '-fcolor-diagnostics', '-Qunused-arguments', 
                                            '-Werror'])

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

from waflib import Configure
@Configure.conf
def add_supported_cxxflags(self, cxxflags):
    """
    Check which cxxflags are supported by compiler and add them to env.CXXFLAGS variable
    """
    self.start_msg('Checking allowed flags for c++ compiler')

    supportedFlags = []
    for flag in cxxflags:
        if self.check_cxx (cxxflags=[flag], mandatory=False):
            supportedFlags += [flag]

    self.end_msg (' '.join (supportedFlags))
    self.env.CXXFLAGS += supportedFlags
