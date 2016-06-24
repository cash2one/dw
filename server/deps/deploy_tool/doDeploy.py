import os
import deploy

set_self_ip     = '192.168.11.21'
set_binary_dir  = "/home/zy2_deploy/deploy_tool/release"
set_config_dir  = "/home/zy2_deploy/deploy_tool/config"

def make_test_1():
    base_port   = 20001
    deploy_dir  = "/zyzr_deploy/zz0101"
    server_id   = '1000'
    area_id     = '100'
    doMaking(set_self_ip, base_port, set_binary_dir, set_config_dir, deploy_dir, server_id, area_id)

def make_test_2():
    base_port   = 21001
    deploy_dir  = "/zyzr_deploy/zz0102"
    server_id   = '1001'
    area_id     = '101'
    deploy.doMaking(set_self_ip, base_port, set_binary_dir, set_config_dir, deploy_dir, server_id, area_id)

def makeServer(base_port, server_id, area_id):
    deploy_dir = '/home/zy2_deploy/zz' + str(area_id)
    deploy.doMaking(set_self_ip, base_port, set_binary_dir, set_config_dir, deploy_dir, server_id, area_id)

if __name__ == '__main__':
    makeServer(11000, '1', '1')
    #makeServer(21000, '2000', '200')
    #makeServer(21100, '2000', '201')
    #makeServer(21200, '2002', '202')
    #makeServer(21300, '2003', '203')
    #makeServer(21400, '2004', '204')
    #makeServer(21500, '2005', '205')
    #makeServer(22000, '3000', '300')
    #makeServer(22100, '3001', '301')
    #makeServer(22200, '3002', '302')
    #makeServer(22300, '3003', '303')
    #makeServer(22400, '3004', '304')
    #makeServer(22500, '3005', '305')
    #makeServer(22600, '3006', '306') 
    #makeServer(23000, '4000', '400')
    #makeServer(23100, '4001', '401')
    #makeServer(23200, '4002', '402')
    #makeServer(23300, '4003', '403')
    #makeServer(23400, '4004', '404')
    #makeServer(23500, '4005', '405')

