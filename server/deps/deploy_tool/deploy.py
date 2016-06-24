import os
import os.path
import shutil

self_ip     = '192.168.11.21'
base_port   = 20001
binary_dir  = "/zyzr_deploy/deploy/release"
config_dir  = "/zyzr_deploy/deploy/config"
deploy_dir  = "/zyzr_deploy/zz0101"
server_id   = '1000'
area_id     = '10'
vrpublic_port = 9200
guid_port = 9900

#guid

#vrpublic gate
port_vrpublic_gate_client    = 0
port_vrpublic_gate_server   = 10

#vrpublic

#vr gate
port_vr_gate_client   = 20
port_vr_gate_server   = 30
# connect to vr

#vr

#tr
port_tr_vr        = 40
port_tr_battle    = 50

#battle gate
port_battle_gate_client     = 60
port_battle_gate_server     = 70

#battle

#friend
port_friend_vr      = 80

working_dir = os.getcwd()

def deploy_config():
    pass

def getPort(port):
    return str(base_port + port)

def make_vr_gate_config():
    read_fs = open(working_dir + '/gate_config.xml', 'r')
    content = read_fs.read()
    content = content.replace('$client_ip', self_ip)
    content = content.replace('$client_port', getPort(port_vr_gate_client))
    content = content.replace('$server_ip', self_ip)
    content = content.replace('$server_port', getPort(port_vr_gate_server))
    return content

def deploy_vr_gate_config():
    sc = make_vr_gate_config()
    fs = open('gate_config.xml', 'w+')
    fs.write(sc)
    fs.close()

def deploy_vr_gate_bin():
    shutil.copy(binary_dir + '/gatesys', './')

def deploy_vr_gate():
    os.chdir(deploy_dir + '/vr/gate')
    deploy_vr_gate_config()
    deploy_vr_gate_bin()
    pass
    
def make_vr_config():
    read_fs = open(working_dir + '/vr_config.xml', 'r')
    vr_content = read_fs.read()
    vr_content = vr_content.replace('$ip_addr', self_ip)
    vr_content = vr_content.replace('$area_id', str(area_id))
    vr_content = vr_content.replace('$server_id', str(server_id))
    vr_content = vr_content.replace('$gate_ip', self_ip)
    vr_content = vr_content.replace('$gate_port', getPort(port_vr_gate_server))
    vr_content = vr_content.replace('$ip_addr', self_ip)
    vr_content = vr_content.replace('$tr_port', getPort(port_tr_vr))
    vr_content = vr_content.replace('$vrpublic_port', str(vrpublic_port))
    vr_content = vr_content.replace('$guid_port', str(guid_port))
    vr_content = vr_content.replace('$friend_port', getPort(port_friend_vr))
    return vr_content
    
def deploy_vr_config():
    sc = make_vr_config()
    fs = open('vr_config.xml', 'w+')
    fs.write(sc)
    fs.close()
    pass

def deploy_vr_bin():
    shutil.copy(binary_dir + '/vrsys', './')
    shutil.copy(working_dir + '/plugin.xml', './')
    pass

def deploy_vr():
    os.chdir(deploy_dir + '/vr/wr')
    deploy_vr_config()
    deploy_vr_bin()
    pass

def make_tr_config():
    read_fs = open(working_dir + '/tr_config.xml', 'r')
    content = read_fs.read()
    content = content.replace('$ip_addr', self_ip)
    content = content.replace('$vr_port', getPort(port_tr_vr))
    content = content.replace('$battle_port', getPort(port_tr_battle))
    return content
    
def deploy_tr_config():
    sc = make_tr_config()
    fs = open('tr_config.xml', 'w+')
    fs.write(sc)
    fs.close()
    pass

def deploy_tr_bin():
    shutil.copy(binary_dir + '/trsys', './')
    pass

def deploy_tr():
    os.chdir(deploy_dir + '/tr/wr')
    deploy_tr_config()
    deploy_tr_bin()
    #shutil.copytree(working_dir + '/tr_cfg/', deploy_dir + '/tr/wr/config/')
    os.chdir('../..')
    pass

def make_vrbattle_gate_config():
    read_fs = open(working_dir + '/gate_config.xml', 'r')
    content = read_fs.read()
    content = content.replace('$client_ip', self_ip)
    content = content.replace('$server_ip', self_ip)
    content = content.replace('$client_port', getPort(port_battle_gate_client))
    content = content.replace('$server_port', getPort(port_battle_gate_server))
    return content

def deploy_vrbattle_gate_config():
    sc = make_vrbattle_gate_config()
    fs = open('gate_config.xml', 'w+')
    fs.write(sc)
    fs.close()

def deploy_vrbattle_gate_bin():
    shutil.copy(binary_dir + '/gatesys', './')

def deploy_vrbattle_gate():
    os.chdir(deploy_dir + '/battle/gate')
    deploy_vrbattle_gate_config()
    deploy_vrbattle_gate_bin()
    os.chdir('../..')

def make_vrbattle_config():
    read_fs = open(working_dir + '/battle_config.xml', 'r')
    content = read_fs.read()
    content = content.replace('$ip_addr', self_ip)
    content = content.replace('$server_id', server_id)
    content = content.replace('$gate_ip', self_ip)
    content = content.replace('$gate_port', getPort(port_battle_gate_server))
    content = content.replace('$tr_port', getPort(port_tr_battle))
    return content

def deploy_vrbattle_config():
    sc = make_vrbattle_config()
    fs = open('battle_config.xml', 'w+')
    fs.write(sc)
    fs.close()
    pass

def deploy_vrbattle_bin():
    shutil.copy(binary_dir + '/battlesys', './')
    pass

def deploy_vrbattle():
    os.chdir(deploy_dir + '/battle/wr')
    deploy_vrbattle_config()
    deploy_vrbattle_bin()
    os.chdir('../../')
    pass

def make_and_check_dir(dir_path):
    if (os.path.exists(dir_path)):
        return True
    os.mkdir(dir_path)
    if (not os.path.exists(dir_path)):
        print 'the dir does not exist : ', dir_path
        return False
    else:
        return True

def make_module_dir(module_dir):
    make_and_check_dir(module_dir);
    make_and_check_dir(module_dir + '/gate');
    make_and_check_dir(module_dir + '/wr');    
    pass

def make_all_module_dir():
    #make_and_check_dir(deploy_dir + '/config')
    make_module_dir(deploy_dir + '/vr')
    make_module_dir(deploy_dir + '/tr')
    make_module_dir(deploy_dir + '/battle')

def deploy_game_config():
    shutil.copytree(config_dir, deploy_dir + '/config/')
    pass

def callCloseScript(project_dir):
    curr_working_dir = os.getcwd()
    os.chdir(project_dir)
    os.system('./close.sh')
    os.chdir(curr_working_dir)

def doMaking(set_ip, set_port, set_bin_dir, set_cfg_dir, set_dep_dir, set_ser_id, set_area_id):
    global self_ip
    global base_port
    global binary_dir
    global config_dir
    global deploy_dir
    global server_id
    global area_id
    self_ip = set_ip
    base_port = set_port
    binary_dir = set_bin_dir
    config_dir = set_cfg_dir
    deploy_dir = set_dep_dir
    server_id = set_ser_id
    area_id = set_area_id
    if os.path.exists(deploy_dir):
        callCloseScript(deploy_dir)
        shutil.rmtree(deploy_dir)
    make_and_check_dir(deploy_dir)
    make_all_module_dir()
    #deploy_game_config()
    deploy_vr()
    deploy_vr_gate()
    deploy_tr()
    deploy_vrbattle()
    deploy_vrbattle_gate()
    os.system('dos2unix ' + working_dir + '/close.sh')
    os.system('chmod +x ' + working_dir + '/close.sh')
    os.system('dos2unix ' + working_dir + '/start.sh')
    os.system('chmod +x ' + working_dir + '/start.sh')
    os.system('dos2unix ' + working_dir + '/status.sh')
    os.system('chmod +x ' + working_dir + '/status.sh')
    shutil.copy(working_dir + '/close.sh', deploy_dir)
    shutil.copy(working_dir + '/start.sh', deploy_dir)
    shutil.copy(working_dir + '/status.sh', deploy_dir)
