import os
import sys
import commands

cmake_debug='-DCMAKE_BUILD_TYPE=Debug'
cmake_release='-DCMAKE_BUILD_TYPE=Release'
build_module=cmake_debug

working_path=os.getcwd()
lib_list = ['simplexml', 'logger', 'netcore', 'plugin', 'pool', 'utility', 'dynamicparse']
plugin_list = ['../plugins/PlayerLoginVR', '../plugins/TrSystem', '../plugins/FRSystemVR']
project_list = ['FrameRobot', 'RobotConfig', '../guidsys', '../gate_server', '../vrpublic', '../battle_server', '../vr_server', '../tr_server']

def execute(cmd_str):
    print 'start to execute : ' + cmd_str + '*******************'
    ret = commands.getstatusoutput(cmd_str)
    if ret[0] == 0:
        return True
    print 'failed to execute : ' + cmd_str
    return False

def buildProject(project_path):
    project_path = working_path + '/' + project_path
    os.chdir(project_path)
    if not execute('cmake . ' + build_module):
	return False
    if not execute('make'):
        return False
    return True

def buildLib(project_path):
    if buildProject(project_path):
        execute('cp -f ' + working_path + '/' + project_path + '/*.so' + ' ../libs')
        return True
    else:
        return False

def buildPlugin(project_path):
    if buildProject(project_path):
        execute('cp -f ' + working_path + '/' + project_path + '/*.so' + ' ../../deps/libs')
        return True
    else:
        return False

def cleanProject(project_path):
    project_path = working_path + '/' + project_path
    print ''
    print 'clean project ' + project_path
    os.chdir(project_path)
    if not execute('cmake . ' + build_module):
        return False
    if not execute('make clean'):
        return False
    return True

def cleanLib(project_path):
    execute('rm -rf ./libs/*.so')
    project_path = working_path + '/' + project_path
    print ''
    print 'clean lib ' + project_path
    os.chdir(project_path)
    if not execute('cmake . ' + build_module):
        return False
    if not execute('make clean'):
        return False
    return True

if __name__ == '__main__':
    execute("rm -rf ./libs/*.so")
    if len(sys.argv) < 2:
        print 'please input build module, 1 : debug, 2 : release'
        exit
    if '2' == sys.argv[1]:
        build_module=cmake_release 
    if '3' == sys.argv[1]:
        for lib in lib_list:
            cleanLib(lib)
        for plugin in plugin_list:
            cleanLib(lib)
        for project in project_list:
            cleanProject(project)
    else:
        for lib in lib_list:
            print 'start build lib : ' + lib + '----------------------------------------'
            if not buildLib(lib):
                print 'failed to build lib : ' + lib
            print 'end to build lib : ' + lib + '-----------------------------------------'
            print ''
        for plugin in plugin_list:
            print 'start build plugin : ' + lib + '----------------------------------------'
            if not buildPlugin(plugin):
                print 'failed to build plugin : ' + lib
            print 'end to build lib : ' + lib + '-----------------------------------------'
            print ''
        for project in project_list:
            print 'start build project : ' + project + '----------------------------------------'
            if not buildProject(project):
                print 'failed to build project : ' + project
            print 'end build project : ' + project + '----------------------------------------'
            print ''
